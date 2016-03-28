#include <linux/gpio.h>

#ifndef _TLC5947_LED_MATRIX_H
#define _TLC5947_LED_MATRIX_H

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

int __init tlc5947_init(void);
void __exit tlc5947_exit(void);

#endif
