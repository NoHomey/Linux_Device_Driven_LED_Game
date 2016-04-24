#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/types.h>
#include <linux/stat.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm/errno.h>

#define IS_ERR(r) r < 0
#define PIN_DEBOUNCE 150
#define INPUT_PIN_IRQ IRQF_TRIGGER_FALLING | IRQF_ONESHOT

static int return_value;
static u_int8_t pin = 27;
static int irq_number = 0;
static volatile bool pin_value = 0;
static int device_major_number;
static const unsigned device_base_number = 0;
static const unsigned device_minor_count = 1;
static struct file_operations file_operations;
static dev_t device_numbers;
static struct cdev* cdev;
static bool file_opened;

static int input_pin_file_open(struct inode* inode, struct file* file) {
    printk(KERN_INFO "Opened %d\n", file_opened);
    if(file_opened) {
        printk(KERN_ERR "Fail to open file /dev/input_pin\n");
        return -EBUSY;
    }
    file_opened = 1;

    return 0;
}

static ssize_t input_pin_file_read(struct file* file, char __user* buffer, const size_t length, loff_t* offset) {
    printk(KERN_INFO "READING %d\n", pin_value);
	if(!pin_value) {
		return 0;
	}
	pin_value = 0;
	return_value = copy_to_user(buffer, (char *) &pin, 1);
    if(return_value > 0) {
        return -EIO;
    }

    return 1;
}

static int input_pin_file_close(struct inode* inode, struct file* file) {
    file_opened = 0;

    return 0;
}

static irqreturn_t gpio_interrupt_handle(int irq, void* dev_id) {
	pin_value = 1;
	printk(KERN_INFO "irq %d handeled\n", irq);
	return IRQ_HANDLED;
}

static int __init input_pin_init(void) {
	return_value = gpio_request(pin, "INPUT_PIN");
	if(IS_ERR(return_value)) {
		printk(KERN_ERR "Unable to request GPIO!\nCalling gpio_request returned %d\n", return_value);
		return return_value;
	}
	return_value = gpio_direction_input(pin);
	if(IS_ERR(return_value)) {
		printk(KERN_ERR "Unable to initialize GPIO!\nCalling gpio_direction_input returned %d\n", return_value);
		return return_value;
	}
	gpio_set_debounce(pin, PIN_DEBOUNCE);
	irq_number = gpio_to_irq(pin);
	if(IS_ERR(irq_number)) {
		printk(KERN_ERR "Unable to map GPIO to IRQ\nCalling gpio_to_irq() returned %d\n", irq_number);
		gpio_free(pin);
		return irq_number;
	}
	return_value = request_irq(irq_number, gpio_interrupt_handle, INPUT_PIN_IRQ, "INPUT_PIN", NULL);
	if(IS_ERR(return_value)) {
		printk(KERN_ERR "Unable to request INTERRUP\nCalling request_irq returned %d\n", return_value);
		gpio_free(pin);
		return return_value;
	}
	return_value = alloc_chrdev_region(&device_numbers, device_base_number, device_minor_count, "input_pin");
	if(IS_ERR(return_value)) {
       printk(KERN_ERR "Could not allocate device numbers\nCalling alloc_chrdev_region returned %d\n", return_value);
	   free_irq(irq_number, NULL);
	   gpio_free(pin);
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
		return return_value;
	}
	file_opened = 0;
	pin_value = 0;

	return 0;
}

static void __exit input_pin_exit(void) {
	unregister_chrdev_region(device_numbers, device_minor_count);
	cdev_del(cdev);
	free_irq(irq_number, NULL);
	gpio_free(pin);
}

module_init(input_pin_init);
module_exit(input_pin_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Borislav Stratev");
MODULE_DESCRIPTION("Basic Linux Kernel module that loggs interrups on pin 27");
MODULE_SUPPORTED_DEVICE("PIN 27");
