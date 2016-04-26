#include <wiringPi.h>
#include <wiringSerial.h>
#include <stdio.h>
#include <errno.h>

int main(void) {
	int fd = serialOpen("/dev/ttyAMA0", 9600);
	int number, c;
	int count = 0;
	if(fd < 0) {
		perror("error while opening serial\n");
		return 1;
	}
	if(wiringPiSetupGpio() == -1) {
		perror("error while seting up wiringPi\n");
		return 1;
	}
	while(count < 10) {
		number = serialDataAvail(fd);
		if(number == -1) {
			perror("error while counting avalible chars\n");
			return 1;
		}
		if(number == 0) {
			continue;
		}
		for(i = 0; i < number; ++i) {
			c = serialGetchar(fd);
			if(c == -1) {
				perror("error while reading char from serial\n");
				return 1;
			}
			printf("%d %d\n", count++, c);
		}
	}
	serialClose(fd);

	return 0;
}
