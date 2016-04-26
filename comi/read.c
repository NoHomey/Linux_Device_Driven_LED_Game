#include <comio/comi.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main(void) {
	uint8_t byte;
	int number;
	int count = 0;
	int fd = comi_open();
	if(fd < 0) {
		perror("error while opening serial\n");
		return 1;
	}
	printf("serial opened\n");
	comi_flush(fd);
	printf("serial flushed\n");
	while(1) {
		number = comi_do(fd, &byte);
		if(number == -1) {
			perror("error while reading\n");
			return 1;
		} else if(number) {
			printf("%d %d", count++, byte);
		}
		if(count == 10) {
			break;
		}
	}
	close(fd);

	return 0;
}
