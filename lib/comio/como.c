#include "como.h"
#include "comio.h"
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

int como_open(void) {
	struct termios options;
	int status;
	int fd = open(COMIO_DEVICE, O_WRONLY | COMIO_MODE);
	if(fd != -1) {
		tcgetattr(fd, &options);
		cfmakeraw(&options);
		cfsetospeed(&options, COMIO_SPEED);
		COMIO_EXTENDED_OPTS(options);
		tcsetattr(fd, TCSANOW, &options);
		ioctl(fd, TIOCMGET, &status);
		status |= TIOCM_DTR;
		status |= TIOCM_RTS;
		ioctl(fd, TIOCMSET, &status);
		usleep(COMIO_OPEN_DELAY);
	}

	return fd;
}

void como_flush(const int fd) {
	tcflush(fd, TCOFLUSH);
}

int como_do(const int fd, uint8_t* byte) {
	return write(fd, byte, 1);
}
