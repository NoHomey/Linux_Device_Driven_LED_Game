#ifndef _COMIO_H
#define _COMIO_H

#define COMIO_DEVICE "/dev/ttyAMA0"
#define COMIO_SPEED B115200
#define COMIO_MODE O_NOCTTY | O_NONBLOCK | O_NDELAY
#define COMIO_OPEN_DELAY 10000

#define COMIO_EXTENDED_OPTS(options) \
options.c_cflag |= (CLOCAL | CREAD); \
options.c_cflag &= ~CSTOPB; \
options.c_cc[VMIN] = 0; \
options.c_cc[VTIME] = 1

#endif
