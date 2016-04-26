#include "comi.h"
#include "comio.h"
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>


int comi_open(void) {
	struct termios options;
	int status;
	int fd = open(COMIO_DEVICE, O_RDONLY | COMIO_MODE);
	if(fd != -1) {
		tcgetattr(fd, &options);
		cfmakeraw(&options);
		cfsetispeed(&options, COMIO_SPEED);
		COMIO_EXTENDED_OPTS(options);
		tcsetattr(fd, TCSANOW, &options);
		ioctl(fd, TIOCMGET, &status);
		status |= TIOCM_DTR;
		ioctl(fd, TIOCMSET, &status);
		usleep(COMIO_OPEN_DELAY);
	}

	return fd;
}

void comi_flush(const int fd) {
	tcflush(fd, TCIFLUSH);
}

int comi_do(const int fd, uint8_t* byte) {
	int bytes;
	if(ioctl(fd, FIONREAD, &bytes) == -1) {
		return -1;
	}
	if(bytes > 0) {
		if(read(fd, byte, 1) == -1) {
			return -1;
		}
		return 1;
	}
	return 0;
}
