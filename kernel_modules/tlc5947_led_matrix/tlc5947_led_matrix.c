#include "tlc5947_led_matrix.h"
#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include "common_helpers.h"

module_param(tlc5947_chips, int, CONST_Param);
MODULE_PARM_DESC(tlc5967_chips, "Number of tlc5967 chips that are connected.");
module_param(tlc5947_data, int, CONST_Param);
MODULE_PARM_DESC(tlc5967_data, "Number of gpio pin on wich DATA signal is connected (BCM Enum).");
module_param(tlc5947_clock, int, CONST_Param);
MODULE_PARM_DESC(tlc5967_clock, "Number of gpio pin on wich CLOCK signal is connected (BCM Enum).");
module_param(tlc5947_latch, int, CONST_Param);
MODULE_PARM_DESC(tlc5967_latch, "Number of gpio pin on wich LATCH signal is connected (BCM Enum).");
module_param(tlc5947_pwm, int, CONST_Param);
MODULE_PARM_DESC(tlc5967_pwm, "Number of pwm to be written.");

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ivo Stratev");
MODULE_DESCRIPTION("Basic Linux Kernel module using GPIOs to drive tlc5947");
MODULE_SUPPORTED_DEVICE("tlc5947");

int __init tlc5947_init(void) {
    tlc5947[0].gpio = tlc5947_data;
	tlc5947[1].gpio = tlc5947_clock;
	tlc5947[2].gpio = tlc5947_latch;
	return request_gpios(tlc5947, 3);
}

void __exit tlc5947_exit(void) {
    int i = tlc5947_chips * TLC5947_LEDS - 1;
    gpio_set_value(tlc5947_latch, GPIO_LOW);
    while(1) {
        int bit = 11;
        while(1) {
            gpio_set_value(tlc5947_clock, GPIO_LOW);
            gpio_set_value(tlc5947_data, (tlc5947_pwm & (1 << bit)) ? GPIO_HIGH : GPIO_LOW);
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
