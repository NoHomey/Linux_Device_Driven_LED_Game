#include <linux/gpio.h>
#include <linux/stat.h>

#ifndef _COMMON_HELPERS_H
#define _COMMON_HELPERS_H

#define CONST_Param S_IRUSR | S_IRGRP | S_IROTH

extern int request_gpios(struct gpio* gpios, const int length);

#endif
