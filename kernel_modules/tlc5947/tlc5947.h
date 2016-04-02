#include <linux/gpio.h>
#include <linux/types.h>
#include <linux/stat.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#ifndef _TLC5947_H
#define _TLC5947_H

#define TLC5947_GPIOS 3
#define GPIO_HIGH 1
#define GPIO_LOW 0
#define CONST_PARAM S_IRUSR | S_IRGRP | S_IROTH

ushort tlc5947_data = 255;
ushort tlc5947_clock = 255;
ushort tlc5947_latch = 255;

static struct gpio tlc5947[TLC5947_GPIOS] = {
	{.gpio = -1, .flags = GPIOF_OUT_INIT_HIGH, .label = "TLC5947 DATA"},
	{.gpio = -1, .flags = GPIOF_OUT_INIT_HIGH, .label = "TLC5947 CLOCK"},
	{.gpio = -1, .flags = GPIOF_OUT_INIT_HIGH, .label = "TLC5947 LATCH"},
};

static int return_value;
static dev_t tlc5947_numbers;
static int tlc5947_major_number;
static int tlc5947_first_minor = 0;
static unsigned int tlc5947_minor_count = 1;
static struct file_operations tlc5947_file_operations;
static struct cdev* tlc5947_cdev;
static int tlc5947_file_opened = 0;
char* tlc5947_buffer;

static int tlc5947_file_open(struct inode* inode, struct file* file);
static ssize_t tlc5947_file_write(struct file* file, const char __user* buffer, const size_t length, loff_t* offset);
static int tlc5947_file_close(struct inode* inode, struct file* file);
static int __init tlc5947_init(void);
static void __exit tlc5947_exit(void);

#endif
