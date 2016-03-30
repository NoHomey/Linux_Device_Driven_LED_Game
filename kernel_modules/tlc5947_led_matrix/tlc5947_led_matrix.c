#include "tlc5947_led_matrix.h"
#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include "common_helpers.h"

module_param(tlc5947_chips, ushort, CONST_Param);
MODULE_PARM_DESC(tlc5967_chips, "Number of tlc5967 chips that are chain connected.");
module_param(tlc5947_data, ushort, CONST_Param);
MODULE_PARM_DESC(tlc5967_data, "Number of gpio pin on wich DATA signal is connected (BCM Enum).");
module_param(tlc5947_clock, ushort, CONST_Param);
MODULE_PARM_DESC(tlc5967_clock, "Number of gpio pin on wich CLOCK signal is connected (BCM Enum).");
module_param(tlc5947_latch, ushort, CONST_Param);
MODULE_PARM_DESC(tlc5967_latch, "Number of gpio pin on wich LATCH signal is connected (BCM Enum).");

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ivo Stratev");
MODULE_DESCRIPTION("Basic Linux Kernel module using GPIOs to drive tlc5947");
MODULE_SUPPORTED_DEVICE("tlc5947");

int is_tlc5947_param_set(ushort* tlc5947_param, const char* tlc5947_param_name) {
    int is_255 = ((*tlc5947_param) == 255);
    if(is_255) {
        printk(KERN_ERR "Parameter %s value not setted when loading the module\n", tlc5947_param_name);
    }
    return is_255;
}

int __init tlc5947_init(void) {
    struct gpio tlc5947[3];
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
	init_ret = request_gpios(tlc5947, 3);
    if(init_ret) {
        return init_ret;
    }
}

void __exit tlc5947_exit(void) {
    unsigned int i;
    i = tlc5947_chips * TLC5947_LEDS - 1;
    gpio_set_value(tlc5947_latch, GPIO_LOW);
    while(1) {
        unsigned char bit = 11;
        while(1) {
            gpio_set_value(tlc5947_clock, GPIO_LOW);
            gpio_set_value(tlc5947_data, (400 & (1 << bit)) ? GPIO_HIGH : GPIO_LOW);
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
	gpio_free_array(tlc5947, 3);
}

module_init(tlc5947_init);
module_exit(tlc5947_exit);
