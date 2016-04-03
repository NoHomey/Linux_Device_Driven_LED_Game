#include "input_pins.h"
#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>

module_param_array(input_pins, int, &input_pins_init_length, CONST_Param);
MODULE_PARM_DESC(input_pins, "Number of pins for wich interrup will be logged (BCM Enum).");

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Borislav Stratev");
MODULE_DESCRIPTION("Basic Linux Kernel module that loggs interrups on input pins");
MODULE_SUPPORTED_DEVICE("IRQ GPIOs");

static int input_pins_file_open(struct inode* inode, struct file* file) {
    if(input_pins_file_opened == 1) {
        printk(KERN_ERR "Fail to open file /dev/input_pins\n");
        return -EBUSY;
    }
    input_pins_file_opened = 1;

    return 0;
}

static ssize_t input_pins_file_read(struct file* file, const char __user* buffer, const size_t length, loff_t* offset) {
    char buffer_length = 0;
    for(i = 0; i < input_pins_init_length; ++i) {
        if(input_pins_values[i]) {
            input_pins_buffer[buffer_length++] = input_pins[i];
            input_pins_values[i] = 0;
        }
    }
    if(!buffer_length) {
        return -EIO;
    }
    if(length < buffer_length) {
        return -EFAULT ;
    }
    copy_to_user(buffer, input_pins_buffer, buffer_length);

    return buffer_length;
}

static int input_pins_file_close(struct inode* inode, struct file* file) {
    input_pins_file_opened = 0;

    return 0;
}

static irqreturn_t input_pins_interrupt(int irq, void* dev_id) {
    char id;
    id  = *((char*) dev_id);
    id = INPUT_PINS_UNMAP(id);
    input_pins_values[id] = 1;

    return IRQ_HANDLED;
}

static int __init input_pins_init(void) {
    if(input_pins_init_length == 0) {
        printk(KERN_ERR "Parameter input_pins value not setted when loading the module\n");
        return -EINVAL;
    }
    nput_pins_ids = (char*) kmalloc(input_pins_init_length * sizeof(char), GFP_KERNEL);
    if(!input_pins_ids) {
        printk(KERN_ERR "Failed to allocate memmry!\nCalling kmalloc returned NULL\n");
        return -ENOMEM;
    }
    input_pins_irqs = (int*) kmalloc(input_pins_init_length * sizeof(int), GFP_KERNEL);
    if(!input_pins_irqs) {
        printk(KERN_ERR "Failed to allocate memmry!\nCalling kmalloc returned NULL\n");
        return -ENOMEM;
    }
    input_pins_buffer = (char*) kmalloc(input_pins_init_length * sizeof(char), GFP_KERNEL);
    if(!input_pins_buffer) {
        printk(KERN_ERR "Failed to allocate memmry!\nCalling kmalloc returned NULL\n");
        return -ENOMEM;
    }
    input_pins_values = (char*) kmalloc(input_pins_init_length * sizeof(char), GFP_KERNEL);
    if(!input_pins_values) {
        printk(KERN_ERR "Failed to allocate memmry!\nCalling kmalloc returned NULL\n");
        return -ENOMEM;
    }
    for(i = 0; i < input_pins_init_length; ++i) {
        input_pins_ids[i] = INPUT_PINS_MAP(i);
        input_pins_values[i] = 0;
        return_value = gpio_request(input_pins[i], "INPUT PIN");
        if(return_value) {
            printk(KERN_ERR "Unable to request GPIOs!\nCalling gpio_request returned %d\n", return_value);
            return return_value;
        }
        return_value = gpio_direction_input(input_pins[i]);
        if(return_value) {
            printk(KERN_ERR "Unable to initialize GPIOs!\nCalling gpio_direction_input returned %d\n", return_value);
            return return_value;
        }
        gpio_set_debounce(input_pins[i], INPUT_PINS_DEBOUNCE);
        input_pins_irqs[i] = gpio_to_irq(input_pins[i]);
        if(input_pins_irqs[i] < 0) {
            printk(KERN_ERR "Unable to map GPIO to IRQ\nCalling gpio_to_irq() returned %d\n", irqs[i]);
            return input_pins_irqs[i];
        }
        printk(KERN_INFO "irq: %d\n", input_pins_irqs[i]);
        return_value = request_irq(input_pins_irqs[i], &input_pins_interrupt, INPUT_PINS_INTERUPT, "input pins", (void*) (input_pins_id + i));
        if(return_value) {
            printk(KERN_ERR "Unable to request INTERRUP\nCalling request_irq returned %d\n", return_value);
            return return_value;
        }
    }
    return_value = alloc_chrdev_region(&input_pins_numbers, input_pins_first_minor, input_pins_minor_count, "input_pins");
    if(return_value) {
       printk(KERN_ERR "Could not allocate device numbers\nCalling alloc_chrdev_region returned %d\n", return_value);
       return return_value;
    }
    input_pins_major_number = MAJOR(input_pins_numbers);
    printk(KERN_INFO "Device major number is %d. Use $ sudo make device major=%d\n", input_pins_major_number, input_pins_major_number);
    input_pins_cdev = cdev_alloc();
    input_pins_cdev->owner = THIS_MODULE;
    input_pins_file_operations.owner = THIS_MODULE;
    input_pins_file_operations.open = input_pins_file_open;
    input_pins_file_operations.release = input_pins_file_close;
    input_pins_file_operations.read = input_pins_file_read;
    input_pins_cdev->ops = &input_pins_file_operations;
    return_value = cdev_add(input_pins_cdev, input_pins_numbers, input_pins_minor_count);
    if(return_value) {
        printk(KERN_ERR "Failed to add device numbers to struct cdev\nCalling cdev_add returned %d\n", return_value);
        return return_value;
    }
    input_pins_file_opened = 0;
    input_pins_length = 0;

    return 0;
}

static void __exit input_pins_exit(void) {
    kfree(input_pins_ids);
    kfree(input_pins_irqs);
    kfree(input_pins_buffer);
    kfree(input_pins_values);
    for(i = 0; i < input_pins_init_length; ++i) {
        gpio_free(input_pins[i]);
        free_irq(input_pins_irqs[i], (void*) (input_pins_id + i));
    }
    unregister_chrdev_region(input_pins_numbers, input_pins_minor_count);
    cdev_del(input_pins_cdev);
}

module_init(input_pins_init);
module_exit(input_pins_exit);