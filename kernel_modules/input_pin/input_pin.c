#include "input_pin.h"

static int input_pin_file_open(struct inode* inode, struct file* file) {
	printk(KERN_INFO "Opened %d\n", file_opened);
	if(file_opened) {
		printk(KERN_ERR "Fail to open file /dev/%s\n", INPUT_PIN_NAME);
		return -EBUSY;
	}
	file_opened = 1;

	return 0;
}

static int input_pin_file_close(struct inode* inode, struct file* file) {
	file_opened = 0;

	return 0;
}

static ssize_t input_pin_file_read(struct file* file, char __user* buffer, const size_t length, loff_t* offset) {
    unsigned long flags;
	int8_t i;
	buffer_length = 0;
	for(i = 0; i < input_pin_init_length; ++i) {
		spin_lock_irqsave(value_lock + i, flags);
		if(pin_value[i]) {
			input_pin_buffer[buffer_length++] = input_pin[i];
			pin_value[i] = 0;
		}
		spin_unlock_irqrestore(value_lock + i, flags);
	}
	if(buffer_length) {
		if(length < buffer_length) {
			return -EFAULT;
		}
		return_value = copy_to_user(buffer, input_pin_buffer, buffer_length);
		if(return_value > 0) {
			return -EIO;
		}
    }

    return buffer_length;
}

static irqreturn_t gpio_interrupt_handle(int irq, void* dev_id) {
	unsigned long flags;
	u_int8_t i;
	for(i = 0; i < input_pin_init_length; ++i) {
		if(irq_number[i] == irq) {
			if((time_after_eq(jiffies, last_jiffies[i] + INPUT_PIN_IRQ_TIMEOUT) || (!last_jiffies[i]))) {
				last_jiffies[i] = jiffies;
				spin_lock_irqsave(value_lock + i, flags);
				pin_value[i] = 1;
				printk(KERN_INFO "irq %d handeled\n", irq);
				spin_unlock_irqrestore(value_lock + i, flags);
			}
		}
	}

	return IRQ_HANDLED;
}

static int __init input_pin_init(void) {
	if(input_pin_init_length == 0) {
		printk(KERN_ERR "Parameter input_pin value not setted when loading the module\n");
		input_pin_exit();
		return -EINVAL;
	}
	irq_number = (int *) kmalloc(input_pin_init_length * sizeof(int), GFP_KERNEL);
	if(!irq_number) {
		printk(KERN_ERR "Failed to allocate memmry for irq_number!\nCalling kmalloc returned NULL\n");
		input_pin_exit();
		return -ENOMEM;
	}
	pin_value = (bool *) kmalloc(input_pin_init_length * sizeof(bool), GFP_KERNEL);
	if(!pin_value) {
		printk(KERN_ERR "Failed to allocate memmry for pin_value!\nCalling kmalloc returned NULL\n");
		input_pin_exit();
		return -ENOMEM;
	}
	last_jiffies = (unsigned long *) kmalloc(input_pin_init_length * sizeof(unsigned long), GFP_KERNEL);
	if(!last_jiffies) {
		printk(KERN_ERR "Failed to allocate memmry for last_jiffies!\nCalling kmalloc returned NULL\n");
		input_pin_exit();
		return -ENOMEM;
	}
	input_pin_buffer = (char *) kmalloc(input_pin_init_length * sizeof(char), GFP_KERNEL);
	if(!input_pin_buffer) {
		printk(KERN_ERR "Failed to allocate memmry for input_pin_buffer!\nCalling kmalloc returned NULL\n");
		input_pin_exit();
		return -ENOMEM;
	}
	value_lock = (spinlock_t *) kmalloc(input_pin_init_length * sizeof(spinlock_t), GFP_KERNEL);
	if(!value_lock) {
		printk(KERN_ERR "Failed to allocate memmry for value_lock!\nCalling kmalloc returned NULL\n");
		input_pin_exit();
		return -ENOMEM;
	}
	for(current_i = 0; current_i < input_pin_init_length; ++current_i) {
		free_state = 0;
		pin_value[current_i] = 0;
		last_jiffies[current_i] = 0;
		spin_lock_init(value_lock + current_i);
		return_value = gpio_request(input_pin[current_i], INPUT_PIN_NAME);
		if(IS_ERR(return_value)) {
			printk(KERN_ERR "Unable to request GPIO!\nCalling gpio_request returned %d\n", return_value);
			input_pin_exit();
			return return_value;
		}
		free_state = 1;
		return_value = gpio_direction_input(input_pin[current_i]);
		if(IS_ERR(return_value)) {
			printk(KERN_ERR "Unable to initialize GPIO!\nCalling gpio_direction_input returned %d\n", return_value);
			input_pin_exit();
			return return_value;
		}
		irq_number[current_i] = gpio_to_irq(input_pin[current_i]);
		if(IS_ERR(irq_number[current_i])) {
			printk(KERN_ERR "Unable to map GPIO to IRQ\nCalling gpio_to_irq() returned %d\n", irq_number[current_i]);
			input_pin_exit();
			return irq_number[current_i];
		}
		return_value = request_irq(irq_number[current_i], gpio_interrupt_handle, INPUT_PIN_IRQ, INPUT_PIN_NAME, NULL);
		if(IS_ERR(return_value)) {
			printk(KERN_ERR "Unable to request INTERRUP\nCalling request_irq returned %d\n", return_value);
			input_pin_exit();
			return return_value;
		}
	}
	current_i = input_pin_init_length;
	free_state = 2;
	return_value = alloc_chrdev_region(&device_numbers, device_base_number, device_minor_count, INPUT_PIN_NAME);
	if(IS_ERR(return_value)) {
       printk(KERN_ERR "Could not allocate device numbers\nCalling alloc_chrdev_region returned %d\n", return_value);
	   input_pin_exit();
       return return_value;
    }
	device_major_number = MAJOR(device_numbers);
	printk(KERN_INFO "Device major number is %d.\n", device_major_number);
	cdev = cdev_alloc();
	cdev->owner = THIS_MODULE;
	file_operations.owner = THIS_MODULE;
	file_operations.open = input_pin_file_open;
	file_operations.release = input_pin_file_close;
	file_operations.read = input_pin_file_read;
	cdev->ops = &file_operations;
	return_value = cdev_add(cdev, device_numbers, device_minor_count);
	if(IS_ERR(return_value)) {
		printk(KERN_ERR "Failed to add device numbers to struct cdev\nCalling cdev_add returned %d\n", return_value);
		input_pin_exit();
		return return_value;
	}
	file_opened = 0;

	return 0;
}

static void input_pin_exit(void) {
	int8_t i;
	const int8_t begin_value = current_i - 1;
	if(device_major_number > -1) {
		unregister_chrdev_region(device_numbers, device_minor_count);
	}
	if(cdev) {
		cdev_del(cdev);
	}
	if(current_i) {
		for(i = begin_value; i >= 0; --i) {
			if(free_state) {
				if(free_state == 2) {
					free_irq(irq_number[i], NULL);
				}
				gpio_free(input_pin[i]);
			}
			if(i == begin_value) {
				free_state = 2;
			}
		}
	}
	if(irq_number) {
		kfree(irq_number);
	}
	if(pin_value) {
		kfree(pin_value);
	}
	if(last_jiffies) {
		kfree(last_jiffies);
	}
	if(input_pin_buffer) {
		kfree(input_pin_buffer);
	}
	if(value_lock) {
		kfree(value_lock);
	}
}

module_init(input_pin_init);
module_exit(input_pin_exit);

module_param_array(input_pin, int, &input_pin_init_length, CONST_PARAM);
MODULE_PARM_DESC(input_pin, "Pin number for wich interrup will be logged (BCM Enum).");

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Borislav Stratev");
MODULE_DESCRIPTION("Basic Linux Kernel module that loggs interrups on pin 27");
MODULE_SUPPORTED_DEVICE("PIN 27");
