#include <linux/gpio.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/types.h>
#include <linux/stat.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#ifndef _INPUT_PINS_H
#define _INPUT_PINS_H

#define CONST_PARAM S_IRUSR | S_IRGRP | S_IROTH
#define INPUT_PINS_MAX 26
#define INPUT_PINS_DEBOUNCE 150
#define INPUT_PINS_INTERRUPT IRQF_TRIGGER_RISING | IRQF_SHARED

#define INPUT_PINS_MAP(i) ((i + 1) * 5) - 3
#define INPUT_PINS_UNMAP(id) ((id + 3) / 5) - 1

static int input_pins[INPUT_PINS_MAX];
static int input_pins_init_length = 0;
static char i;
static int return_value;
static dev_t input_pins_numbers;
static int input_pins_major_number;
static const int input_pins_first_minor = 0;
static const unsigned int input_pins_minor_count = 1;
static struct file_operations input_pins_file_operations;
static struct cdev* input_pins_cdev;
static int input_pins_file_opened = 0;
static int input_pins_length;
static u8* input_pins_ids;
static int* input_pins_irqs;
static char* input_pins_buffer;
static u8* input_pins_values;

static int input_pins_file_open(struct inode* inode, struct file* file);
static ssize_t input_pins_file_read(struct file* file, char __user* buffer, const size_t length, loff_t* offset);
static int input_pins_file_close(struct inode* inode, struct file* file);
static irqreturn_t input_pins_interrupt(int irq, void* dev_id);
static int __init input_pins_init(void);
static void __exit input_pins_exit(void);

#endif
