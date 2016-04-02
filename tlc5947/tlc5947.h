#include <stdint.h>

#ifndef _LIBTLC5947_H
#define _LIBTLC5947_H

#define LIBTLC5947_DEVICE "/dev/tlc5947"
#define LIBTLC5947_LEDS 24
#define LIBTLC5947_MAXPWM 4095

int tlc5947_write(const uint16_t pwm, const uint8_t chips);

#endif
