#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/types.h>
#include <asm/errno.h>

#define IS_ERR(r) r > 0
#define PIN_DEBOUNCE 150
#define INPUT_PIN_IRQ IRQF_TRIGGER_FALLING | IRQF_ONESHOT

int return_value;
u_int8_t pin = 27;
int irq_number = 0;
bool pin_value = 0;

static irqreturn_t gpio_interrupt_handle(int irq, void* dev_id) {
	pin_value = 1;
	printk(KERN_ERR "irq %d handeled\n", irq);
	return IRQ_HANDLED;
}

static int __init input_pin_init(void) {
	return_value = gpio_request(pin, "INPUT_PIN");
	if(IS_ERR(return_value)) {
		printk(KERN_ERR "Unable to request GPIO!\nCalling gpio_request returned %d\n", return_value);
		return return_value;
	}
	return_value = gpio_direction_input(pin);
	if(IS_ERR(return_value)) {
		printk(KERN_ERR "Unable to initialize GPIO!\nCalling gpio_direction_input returned %d\n", return_value);
		return return_value;
	}
	gpio_set_debounce(pin, PIN_DEBOUNCE);
	irq_number = gpio_to_irq(pin);
	if(IS_ERR(irq_number)) {
		printk(KERN_ERR "Unable to map GPIO to IRQ\nCalling gpio_to_irq() returned %d\n", irq_number);
		gpio_free(pin);
		return irq_number;
	}
	return_value = request_irq(irq_number, gpio_interrupt_handle, INPUT_PIN_IRQ, "INPUT_PIN", NULL);
	if(IS_ERR(return_value)) {
		printk(KERN_ERR "Unable to request INTERRUP\nCalling request_irq returned %d\n", return_value);
		gpio_free(pin);
		return return_value;
	}
	return 0;
}

static void __exit input_pin_exit(void) {
	free_irq(irq_number, NULL);
	gpio_free(pin);
}

module_init(input_pin_init);
module_exit(input_pin_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Borislav Stratev");
MODULE_DESCRIPTION("Basic Linux Kernel module that loggs interrups on pin 27");
MODULE_SUPPORTED_DEVICE("PIN 27");
