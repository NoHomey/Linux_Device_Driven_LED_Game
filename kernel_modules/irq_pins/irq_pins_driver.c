#include "irq_pins_driver.h"
#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include "common_helpers.h"

module_param_array(irq_pins, u8, &irq_pins_init_length, CONST_Param);
MODULE_PARM_DESC(irq_pins, "Number of pins for wich interup will be logged (BCM Enum).");

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Borislav Stratev");
MODULE_DESCRIPTION("Basic Linux Kernel module that loggs interups on pins");
MODULE_SUPPORTED_DEVICE("IRQ GPIOs");

irqreturn_t irq_pins_interrupt(int irq, void* dev_id) {
    int id = *((int*) dev_id;
    u8 i;
    u8 value = gpio_get_value(id);
    u8 change = 0;
    for(i = 0; i < irq_pins_init_length; ++i) {
        if(irq_pins[i] == id) {
            if((irq_pins_value[i] == 2) || (irq_pins_value[i] != value)) {
                change = i;
            }
        }
    }
    if(change) {
        irq_pins_value[change] = value;
        printk(KERN_INFO "pin has changed %d\n", change);
    }
    return IRQ_HANDLED;
}

void fill_gpio_array(struct gpio* irq_pins_gpio) {
    u8 i;
    for(i = 0; i < irq_pins_init_length; ++i) {
        irq_pins_gpio[i].gpio = irq_pins[i];
        irq_pins_gpio[i].flags = GPIOF_IN;
        irq_pins_gpio[i].label = "IRQ Pin";
        irq_pins_value[i] = 2;
    }
}

int __init irq_pins_init(void) {
    u8 i;
    int request_gpios_error;
    int request_irq_error;
    struct gpio irq_pins_gpio[irq_pins_init_length];
    fill_gpio_array(irq_pins_gpio);
    request_gpios_error = request_gpios(irq_pins_gpio, irq_pins_init_length);
	if(!request_gpios_error) {
        for(i = 0; i < irq_pins_init_length; ++i) {
            gpio_set_debounce(irq_pins[i], 200);
            irqs[i] = gpio_to_irq(irq_pins[i]);
            if(irqs[i] < 0) {
                printk(KERN_ERR "Unable to map GPIO to IRQ\nCalling gpio_to_irq() returned %d\n", irqs[i]);
                return irqs[i];
            }
            request_irq_error = request_irq(irqs[i], &irq_pins_interrupt, GPIO_INTERUPT, "irq gpios", (void*) (irq_pins + i));
            if(request_irq_error) {
                printk(KERN_ERR "Unable to request INTERUP\nCalling request_irq() returned %d\n", request_irq_error);
                return request_irq_error;
            }
        }
    }
    return request_gpios_error;
}

void __exit irq_pins_exit(void) {
    u8 i;
    struct gpio irq_pins_gpio[irq_pins_init_length];
    for(i = 0; i < irq_pins_init_length; ++i) {
        free_irq(irqs[i], (void*) (irq_pins + i));
    }
    fill_gpio_array(irq_pins_gpio);
	gpio_free_array(irq_pins_gpio, irq_pins_init_length);
}

module_init(irq_pins_init);
module_exit(irq_pins_exit);
