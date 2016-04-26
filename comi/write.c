#include <wiringPi.h>
#include <wiringSerial.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int main(void) {
	int fd = serialOpen("/dev/ttyAMA0", 9600);
	char c;
	if(fd < 0) {
		perror("error while opening serial\n");
		return 1;
	}
	if(wiringPiSetupGpio() == -1) {
		perror("error while seting up wiringPi\n");
		return 1;
	}
	for(c = 66; c < 76; ++c) {
		serialPutchar(fd, c);
		sleep(2);
	}
	serialClose(fd);

	return 0;
}
