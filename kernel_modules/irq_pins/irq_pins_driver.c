#include "irq_pins_driver.h"
#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include "common_helpers.h"

module_param_array(irq_pins, int, &irq_pins_init_length, CONST_Param);
MODULE_PARM_DESC(irq_pins, "Number of pins for wich interup will be logged (BCM Enum).");

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Borislav Stratev");
MODULE_DESCRIPTION("Basic Linux Kernel module that loggs interups on pins");
MODULE_SUPPORTED_DEVICE("IRQ GPIOs");

irqreturn_t irq_pins_interrupt(int irq, void* dev_id, struct pt_regs*, regs) {
    printk(KERN_INFO "irq is %d\n", irq);
    return (irq_handler_t) IRQ_HANDLED;
}

int __init irq_pins_init(void) {
    int i;
    int request_gpios_error;
    int request_irq_error;
    for(i = 0; i < irq_pins_init_length; ++i) {
        irq_pins_gpio[i].gpio = irq_pins_[i];
        irq_pins_gpio[i].flags = GPIOF_IN;
        sprintf(irq_pins_gpio[i].label, "IRQ Pin %d", irq_pins_[i]);
    }
    request_error = request_gpios(irq_pins_gpio, irq_pins_init_length);
	if(!request_error) {
        for(i = 0; i < irq_pins_init_length; ++i) {
            gpio_set_debounce(irq_pins_gpio[i], 200);
            irqs[i] = gpio_to_irq(irq_pins_gpio[i]);
            if(irqs[i] < 0) {
                printk(KERN_ERR "Unable to map GPIO to IRQ\nCalling gpio_to_irq() returned %d\n", irqs[i]);
                return irqs[i];
            }
            request_irq_error = request_irq(irqs[i], irq_pins_interrupt, GPIO_INTERUPT, "irq gpios", NULL);
            if(request_irq_error) {
                printk(KERN_ERR "Unable to request INTERUP\nCalling request_irq() returned %d\n", request_irq_error);
                return request_irq_error;
            }
        }
    }
    return request_gpios_error;
}

void __exit irq_pins_exit(void) {
    for(i = 0; i < irq_pins_init_length; ++i) {
        free_irq(irqs[i], NULL);
    }
	gpio_free_array(irq_pins_gpio, irq_pins_init_length);
}

module_init(irq_pins_init);
module_exit(irq_pins_exit);
