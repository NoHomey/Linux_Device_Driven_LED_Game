#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/sched.h>
#include <linux/tty.h>
#include <linux/sysfs.h>
#include <linux/stat.h>

#define CONST_Param S_IRUSR | S_IRGRP | S_IROTH

#define TLC5947_LEDS 24
#define TLC5947_MAXPWM 4096
#define GPIO_HIGH 1
#define GPIO_LOW 0

int tlc5947_chips = 8;
int tlc5947_data = 17;
int tlc5947_clock = 27;
int tlc5947_latch = 22;
int tlc5947_pwm = 450;

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

int request_gpios(struct gpio* gpios, const int length) {
	int request_error = gpio_request_array(gpios, length);
	if(request_error) {
		printk(KERN_ERR "Unable to request GPIOs!\n Calling gpio_request_array() returned %d\n", request_error);
	}
	return request_error;
}

void free_gpios(struct gpio* gpios, const int length) {
	gpio_free_array(gpios, length);
}

static struct gpio tlc5947[3] = {
		{ .gpio = -1, GPIOF_OUT_INIT_HIGH, "TLC5947 DATA" },
		{ .gpio = -1, GPIOF_OUT_INIT_HIGH, "TLC5947 CLOCK" },
		{ .gpio = -1, GPIOF_OUT_INIT_HIGH, "TLC5947 LATCH" },
};

static int __init test_init(void) {
    tlc5947[0].gpio = tlc5947_data;
	tlc5947[1].gpio = tlc5947_clock;
	tlc5947[2].gpio = tlc5947_latch;
	return request_gpios(tlc5947, 3);
}

static void __exit test_exit(void) {
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
	free_gpios(tlc5947, 3);
}

module_init(test_init);
module_exit(test_exit);
