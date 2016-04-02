#include <tlc5947.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

int main(void) {
    uint16_t i;
    uint16_t pwm[] = {1000, 2000, 3000};
    struct tlc5947 tlc5947;
    tlc5947_init(&tlc5947, 8);
    for(i = 0; i < 64; ++i) {
        tlc5947_setRGBLED(&tlc5947, i, pwm);
    }
    printf("%d\n", tlc5947_write(&tlc5947));
    tlc5947_free(tlc5947);

    return 0;
}
