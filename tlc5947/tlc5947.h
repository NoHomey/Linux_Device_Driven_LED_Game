#include <stdint.h>

#ifndef _LIBTLC5947_H
#define _LIBTLC5947_H

#define LIBTLC5947_DEVICE "/dev/tlc5947"
#define LIBTLC5947_LEDS 24
#define LIBTLC5947_MAXPWM 4095

struct tlc5947 {
    int fd;
    uint8_t chips;
    uint16_t leds;
    uint16_t length;
    uint16_t* pwm;
    unsigned char* buffer;
};

int tlc5947_init(struct tlc5947* tlc5947, const uint8_t chips);
void tlc5947_free(struct tlc5947* tlc5947);
int tlc5947_setLED(struct tlc5947* tlc5947, const uint16_t led, const uint16_t pwm);
int tlc5947_setRGBLED(struct tlc5947* tlc5947, const uint16_t rgb, const uint16_t* pwm);
int tlc5947_write(struct tlc5947* tlc5947);

#endif
