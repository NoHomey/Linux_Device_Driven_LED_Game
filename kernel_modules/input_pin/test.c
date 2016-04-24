#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE "/dev/input_pin"

int main(void) {
	unsigned int read_length;
	unsigned int i = 0;
	const unsigned int length = 1;
	char read_data[length];
	int fd = open(DEVICE, O_RDONLY);
	if(fd == -1) {
		printf("File %s not exist or is in use.\n", DEVICE);
		return fd;
	}
	while(i < 100) {
    		read_length = read(fd, read_data, length);
    		if(read_length > 0) {
        		printf("i %d pin %d\n", i++, read_data[0]);
   		 }
	}

	return 0;
}
