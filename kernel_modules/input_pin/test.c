#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <input_pin.h>

#define DEVICE "/dev/input_pin"

int main(void) {
	unsigned int read_length;
	unsigned int j, i = 0;
	const unsigned int length = 16;
	char read_data[length];
	int fd = open(DEVICE, O_RDONLY);
	if(fd == -1) {
		printf("File %s not exist or is in use.\n", DEVICE);
		return fd;
	}
	while(1) {
    		if(ioctl(fd, INPUT_PIN_AWAIT)) {
				read_length = read(fd, read_data, length);
				for(j = 0; j < read_length; ++j) {
					printf("i %d pin %d\n", i++, read_data[j]);
					if(i == 20) {
						return 0;
					}
				}
   		 }
	}

	return 0;
}
