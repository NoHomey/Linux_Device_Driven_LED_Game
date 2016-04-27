#ifndef _INPUT_PIN_H
#define _INPUT_PIN_H

#ifdef __KERNEL__

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/types.h>
#include <linux/stat.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm/errno.h>
#include <linux/spinlock.h>
#include <linux/jiffies.h>
#include <linux/ioctl.h>

#define IS_ERR(r) r < 0
#define INPUT_PIN_MAX_COUNT 26
#define INPUT_PIN_IRQ IRQF_TRIGGER_FALLING | IRQF_ONESHOT
#define INPUT_PIN_IRQ_TIMEOUT HZ / 3 - 50
#define INPUT_PIN_NAME "input_pin"
#define CONST_PARAM S_IRUSR | S_IRGRP | S_IROTH

static int return_value;
static int device_major_number = -1;
static const unsigned device_base_number = 0;
static const unsigned device_minor_count = 1;
static struct file_operations file_operations;
static dev_t device_numbers;
static struct cdev* cdev = NULL;
static bool file_opened;
static size_t buffer_length;
static int input_pin_init_length = 0;

static int input_pin[INPUT_PIN_MAX_COUNT];
static int* irq_number = NULL;
static bool* pin_value = NULL;
static unsigned long* last_jiffies = NULL;
static char* input_pin_buffer = NULL;
static spinlock_t* value_lock = NULL;

static int8_t current_i = 0;
static u_int8_t free_state;

static int input_pin_file_open(struct inode* inode, struct file* file);
static int input_pin_file_close(struct inode* inode, struct file* file);
static ssize_t input_pin_file_read(struct file* file, char __user* buffer, const size_t length, loff_t* offset);
long input_pin_ioctl(struct file * file, unsigned int cmd, unsigned long arg);
static irqreturn_t gpio_interrupt_handle(int irq, void* dev_id);
static int __init input_pin_init(void);
static void input_pin_exit(void);

#else

#include <sys/ioctl.h>

#endif

#define INPUT_PIN_MAGIC_NUMBER '?'
#define INPUT_PIN_AWAIT _IO(INPUT_PIN_MAGIC_NUMBER, 0)
#define INPUT_PIN_MAX_NUMBER 0

#endif
