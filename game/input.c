#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <input_pin.h>
#include <comio/como.h>

#define DEVICE "/dev/input_pin"

int main(void) {
	unsigned int read_length;
	unsigned int j;
	const unsigned int length = 16;
	char read_data[length];
	int input = open(DEVICE, O_RDONLY);
	int fd = como_open();
	if(fd < 0) {
		perror("error while opening serial\n");
		return 1;
	}
	if(input == -1) {
		printf("File %s not exist or is in use.\n", DEVICE);
		return input;
	}
	como_flush(fd);
	while(1) {
    		if(ioctl(fd, INPUT_PIN_AWAIT)) {
				read_length = read(fd, read_data, length);
				for(j = 0; j < read_length; ++j) {
					como_do(fd, read_data + j);
				}
   		 }
	}
	close(fd);
	close(input);

	return 0;
}
