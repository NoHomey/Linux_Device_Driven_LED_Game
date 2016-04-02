#include <stdint.h>

#ifndef _LIBTLC5947_H
#define _LIBTLC5947_H

#define LIBTLC5947_DEVICE "/dev/tlc5947"
#define LIBTLC5947_LEDS 24
#define LIBTLC5947_MAXPWM 4095

#define W_LED 0
#define W_RGB 36
#define W_GBR 18
#define W_BRG 9
#define W_RBG 25
#define W_GRB 33
#define W_BGR 6
#define W_MASK 3

#define W_RED(w) w & W_MASK
#define W_GREEN(w) (w >> 2) & W_MASK
#define W_BLUE(w) (w >> 4) & W_MASK

struct tlc5947 {
    int fd;
    uint8_t chips;
    uint16_t leds;
    uint16_t length;
    uint8_t wiring[3];
    uint16_t* pwm;
    unsigned char* buffer;
};

int tlc5947_init(struct tlc5947* tlc5947, const uint8_t chips);
void tlc5947_free(struct tlc5947* tlc5947);
int tlc5947_setLED(struct tlc5947* tlc5947, const uint16_t led, const uint16_t pwm);
int tlc5947_setRGBLED(struct tlc5947* tlc5947, const uint16_t rgb, const uint16_t* pwm);
int tlc5947_write(struct tlc5947* tlc5947);

#endif
