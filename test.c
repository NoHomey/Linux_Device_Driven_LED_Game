#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>

#define DATA_ 17
#define CLOCK_ 27
#define LATCH_ 22
#define NUM_ 8
#define LEDS_ 24
#define MAXPWM_ 4096
#define PWM_ 2048

static struct gpio tlc5947[] = {
		{ DATA_, GPIOF_OUT_INIT_HIGH, "DATA" },
		{ CLOCK_, GPIOF_OUT_INIT_HIGH, "CLOCK" },
		{ LATCH_, GPIOF_OUT_INIT_HIGH, "LATCH" },
};

static int __init test_init(void) {
    int ret = 0;
	printk(KERN_INFO "%s\n", __func__);
	ret = gpio_request_array(tlc5947, ARRAY_SIZE(tlc5947));
	if (ret) {
		printk(KERN_ERR "Unable to request GPIOs: %d\n", ret);
	}
	return ret;
}

static void __exit test_exit(void) {
    int i = NUM_ * LEDS_ - 1;
    gpio_set_value(tlc5947[2].gpio, 0);
    while(1) {
        int bit = 11;
        while(1) {
            gpio_set_value(tlc5947[1].gpio, 0);
            gpio_set_value(tlc5947[0].gpio, (PWM_ & (1 << bit)) ? 1 : 0);
            gpio_set_value(tlc5947[1].gpio, 1);
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
    gpio_set_value(tlc5947[1].gpio, 0);
    gpio_set_value(tlc5947[2].gpio, 1);
    gpio_set_value(tlc5947[2].gpio, 0);
    gpio_free_array(tlc5947, ARRAY_SIZE(tlc5947));
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ivo Stratev");
MODULE_DESCRIPTION("Basic Linux Kernel module using GPIOs to drive tlc5947");

module_init(test_init);
module_exit(test_exit);
