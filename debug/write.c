#include <comio/como.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int main(void) {
	uint8_t byte = 75;
	int fd = como_open();
	int i;
	if(fd < 0) {
		perror("error while opening serial\n");
		return 1;
	}
	printf("serial opened\n");
	como_flush(fd);
    printf("serial flushed\n");
	for(i = 0; i < 3000; ++i) {
		if(como_do(fd, &byte) == -1) {
			perror("error while writing\n");
			return 1;
		}
	}
	close(fd);

	return 0;
}
