#include "tlc5947_led_matrix.h"
#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/gpio.h>
#include <linux/types.h>
#include <linux/stat.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <asm/errno.h>

module_param(tlc5947_chips, ushort, CONST_PARAM);
MODULE_PARM_DESC(tlc5967_chips, "Number of tlc5967 chips that are chain connected.");
module_param(tlc5947_data, ushort, CONST_PARAM);
MODULE_PARM_DESC(tlc5967_data, "Number of gpio pin on wich DATA signal is connected (BCM Enum).");
module_param(tlc5947_clock, ushort, CONST_PARAM);
MODULE_PARM_DESC(tlc5967_clock, "Number of gpio pin on wich CLOCK signal is connected (BCM Enum).");
module_param(tlc5947_latch, ushort, CONST_PARAM);
MODULE_PARM_DESC(tlc5967_latch, "Number of gpio pin on wich LATCH signal is connected (BCM Enum).");

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ivo Stratev");
MODULE_DESCRIPTION("Basic Linux Kernel module using GPIOs to drive tlc5947");
MODULE_SUPPORTED_DEVICE(TLC5947_NAME);

static int tlc5947_file_open(struct inode* inode, struct file* file) {
    if(tlc5947_file_opened == 1) {
        printk(KERN_ERR "Fail to open file /dev/%s\n", TLC5947_NAME);
        return -EBUSY;
    }
    tlc5947_file_opened = 1;

    return 0;
}

static ssize_t tlc5947_file_write(struct file* file, const char __user* buffer, size_t buffer_length, loff_t* offset) {
    char data[10];
    unsigned int i;
    unsigned long copy_ret = copy_from_user(data, buffer, buffer_length);
    if(copy_ret) {
        printk(KERN_ERR "Error while copying data\nCalling copy_from_user reurned%ld\n", copy_ret);
        return copy_ret;
    }
    data[0] -= '0';
    i = tlc5947_chips * TLC5947_LEDS - 1;
    gpio_set_value(tlc5947_latch, GPIO_LOW);
    while(1) {
        unsigned char bit = 11;
        while(1) {
            gpio_set_value(tlc5947_clock, GPIO_LOW);
            gpio_set_value(tlc5947_data, (data[0] & (1 << bit)) ? GPIO_HIGH : GPIO_LOW);
            gpio_set_value(tlc5947_clock, GPIO_HIGH);
            if(bit == 0) {
                break;
            }
            bit--;
        }
        if(i == 0) {
            break;
        }
        i--;
    }
    gpio_set_value(tlc5947_clock, GPIO_LOW);
    gpio_set_value(tlc5947_latch, GPIO_HIGH);
    gpio_set_value(tlc5947_latch, GPIO_LOW);

    return copy_ret;
}

static int tlc5947_file_close(struct inode* inode, struct file* file) {
    tlc5947_file_opened = 0;

    return 0;
}

static int is_tlc5947_param_set(ushort* tlc5947_param, const char* tlc5947_param_name) {
    int is_255 = (*tlc5947_param) == 255;
    if(is_255) {
        printk(KERN_ERR "Parameter %s value not setted when loading the module\n", tlc5947_param_name);
    }

    return is_255;
}

static int __init tlc5947_init(void) {
    int init_ret = 0;
    init_ret |= is_tlc5947_param_set(&tlc5947_chips, "tlc5947_chips");
    init_ret |= is_tlc5947_param_set(&tlc5947_data, "tlc5947_data");
    init_ret |= is_tlc5947_param_set(&tlc5947_clock, "tlc5947_clock");
    init_ret |= is_tlc5947_param_set(&tlc5947_latch, "tlc5947_latch");
    if(init_ret) {
        return -EINVAL;
    }
    tlc5947[0].gpio = tlc5947_data;
	tlc5947[1].gpio = tlc5947_clock;
	tlc5947[2].gpio = tlc5947_latch;
	init_ret = gpio_request_array(tlc5947, TLC5947_GPIOS);
    if(init_ret) {
        printk(KERN_ERR "Unable to request GPIOs!\nCalling gpio_request_array returned %d\n", init_ret);
        return init_ret;
    }
    init_ret = alloc_chrdev_region(&tlc5947_numbers, tlc5947_first_minor, tlc5947_minor_count, TLC5947_NAME);
    if(init_ret) {
       printk(KERN_ERR "Could not allocate device numbers\nCalling alloc_chrdev_region returned %d\n", init_ret);
        return init_ret;
    }
    tlc5947_major_number = MAJOR(tlc5947_numbers);
    printk(KERN_INFO "Device major number is %d. Use $ sudo make device\n", tlc5947_major_number);
    tlc5947_cdev = cdev_alloc();
    tlc5947_cdev->owner = THIS_MODULE;
    tlc5947_file_operations.owner = THIS_MODULE;
    tlc5947_file_operations.open = tlc5947_file_open;
    tlc5947_file_operations.release = tlc5947_file_close;
    tlc5947_file_operations.write = tlc5947_file_write;
    tlc5947_cdev->ops = &tlc5947_file_operations;
    init_ret = cdev_add(tlc5947_cdev, tlc5947_numbers, tlc5947_minor_count);
    if(init_ret) {
        printk(KERN_ERR "Failed to add device numbers to struct cdev\nCalling cdev_add returned %d\n", init_ret);
        return init_ret;
    }

    return 0;
}

static void __exit tlc5947_exit(void) {
    cdev_del(tlc5947_cdev);
    unregister_chrdev_region(tlc5947_numbers, tlc5947_minor_count);
	gpio_free_array(tlc5947, TLC5947_GPIOS);
}

module_init(tlc5947_init);
module_exit(tlc5947_exit);
