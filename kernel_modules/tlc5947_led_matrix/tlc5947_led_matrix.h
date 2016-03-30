#include <linux/gpio.h>

#ifndef _TLC5947_LED_MATRIX_H
#define _TLC5947_LED_MATRIX_H

#define TLC5947_LEDS 24
#define TLC5947_MAXPWM 4096
#define GPIO_HIGH 1
#define GPIO_LOW 0

ushort tlc5947_chips = 255;
ushort tlc5947_data = 255;
ushort tlc5947_clock = 255;
ushort tlc5947_latch = 255;

struct gpio tlc5947[3] = {
	{ .gpio = -1, GPIOF_OUT_INIT_HIGH, "TLC5947 DATA" },
	{ .gpio = -1, GPIOF_OUT_INIT_HIGH, "TLC5947 CLOCK" },
	{ .gpio = -1, GPIOF_OUT_INIT_HIGH, "TLC5947 LATCH" },
};

int is_tlc5947_param_set(ushort* tlc5947_param, const char* tlc5947_param_name);
int __init tlc5947_init(void);
void __exit tlc5947_exit(void);

#endif
