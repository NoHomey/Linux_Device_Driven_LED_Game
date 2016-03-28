#include "tlc5947_led_matrix.h"

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

int __init test_init(void) {
    tlc5947[0].gpio = tlc5947_data;
	tlc5947[1].gpio = tlc5947_clock;
	tlc5947[2].gpio = tlc5947_latch;
	return request_gpios(tlc5947, 3);
}

void __exit test_exit(void) {
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
