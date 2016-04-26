#include <wiringPi.h>
#include <wiringSerial.h>
#include <stdio.h>
#include <errno.h>

int main(void) {
	if(wiringPiSetupGpio() == -1) {
                perror("error while seting up wiringPi\n");
                return 1;
        }
        printf("wringPi setup\n"); 
	int fd = serialOpen("/dev/ttyAMA0", 115200);
	int number, c;
	int i, count = 0;
	if(fd < 0) {
		perror("error while opening serial\n");
		return 1;
	}
	printf("serial opened\n");
	serialFlush(fd);
	printf("serial flushed\n");
	while(1) {
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
			if(count == 10) {
				goto end_it;
			}
		}
	}
	end_it:
	serialPutchar(fd, 's');
	serialClose(fd);

	return 0;
}
