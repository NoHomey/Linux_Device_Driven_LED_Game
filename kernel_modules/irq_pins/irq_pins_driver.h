#include <linux/gpio.h>
#include <linux/irq.h>
#include <linux/interrupt.h>

#ifndef _IRQ_PINS_H
#define _IRQ_PINS_H

#define PINS 32
#define GPIO_INTERUPT IRQF_TRIGGER_RISING | IRQF_SHARED

int irqs[PINS];
u8 irq_pins[PINS];
u8 irq_pins_value[PINS];
u8 irq_pins_init_length;

irqreturn_t irq_pins_interrupt(int irq, void* dev_id);
void fill_gpio_array(struct gpio* irq_pins_gpio);
int __init irq_pins_init(void);
void __exit irq_pins_exit(void);

#endif
