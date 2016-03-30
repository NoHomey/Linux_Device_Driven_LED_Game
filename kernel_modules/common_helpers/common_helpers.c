#include "common_helpers.h"
#include <linux/kernel.h>

int request_gpios(struct gpio* gpios, const u8 length) {
	int request_error = gpio_request_array(gpios, length);
	if(request_error) {
		printk(KERN_ERR "Unable to request GPIOs!\nCalling gpio_request_array() returned %d\n", request_error);
	}
	return request_error;
}
