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

struct gpio tlc5947[3] = {
	{ .gpio = -1, GPIOF_OUT_INIT_HIGH, "TLC5947 DATA" },
	{ .gpio = -1, GPIOF_OUT_INIT_HIGH, "TLC5947 CLOCK" },
	{ .gpio = -1, GPIOF_OUT_INIT_HIGH, "TLC5947 LATCH" },
};

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

int request_gpios(struct gpio* gpios, const int length);
void free_gpios(struct gpio* gpios, const int length);

int __init test_init(void);
void __exit test_exit(void);
