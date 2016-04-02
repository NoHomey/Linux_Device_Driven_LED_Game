#include "tlc5947.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int tlc5947_write(const uint16_t pwm, const uint8_t chips) {
    const int leds = chips * LIBTLC5947_LEDS;
    const int length = leds + (leds / 2);
    unsigned char buffer[length];
    int i, j, fd, written;
    if(pwm > LIBTLC5947_MAXPWM) {
        return -EINVAL;
    }
    for(i = 0; i < leds; i += 2) {
        buffer[j++] = pwm | 255;
        buffer[j++] = (pwm | 3840) + (pwm | 15);
        buffer[j++] = pwm | 4080;
    }
    errno = 0;
    fd = open(LIBTLC5947_DEVICE, O_WRONLY);
    if(fd == -1) {
        return -errno;
    }
    errno = 0;
    written = write(fd, (char*) buffer, length);
    if(written < 1) {
        close(fd);
        return -errno;
    }
    close(fd);

    return 0;
}
