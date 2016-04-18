#ifndef _LIBTLC5947_H
#define _LIBTLC5947_H

#include <stdint.h>

#define LIBTLC5947_DEVICE "/dev/tlc5947"
#define LIBTLC5947_LEDS 24
#define LIBTLC5947_MAXPWM 4095

#define Wir_LED 0
#define Wir_RGB 36
#define Wir_GBR 18
#define Wir_BRG 9
#define Wir_RBG 25
#define Wir_GRB 33
#define Wir_BGR 6
#define Wir_MASK 3

#define Wir_RED(w) w & Wir_MASK
#define Wir_GREEN(w) (w >> 2) & Wir_MASK
#define Wir_BLUE(w) (w >> 4) & Wir_MASK

struct tlc5947 {
    int fd;
    uint8_t chips;
    uint16_t leds;
    uint16_t length;
    uint8_t wiring[3];
    uint16_t* pwm;
    unsigned char* buffer;
};

int tlc5947_init(struct tlc5947* tlc5947, const uint8_t chips, const uint8_t wiring);
void tlc5947_free(struct tlc5947* tlc5947);
int tlc5947_setLED(struct tlc5947* tlc5947, const uint16_t led, const uint16_t pwm);
int tlc5947_setRGBLED(struct tlc5947* tlc5947, const uint16_t rgb, const uint16_t* pwm);
int tlc5947_unsetLED(struct tlc5947* tlc5947, const uint16_t led);
int tlc5947_unsetRGBLED(struct tlc5947* tlc5947, const uint16_t rgb);
int tlc5947_write(struct tlc5947* tlc5947);

#endif
