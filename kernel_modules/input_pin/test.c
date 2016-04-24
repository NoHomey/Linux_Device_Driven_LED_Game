#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE "/dev/input_pin"

int main(void) {
	unsigned int read_length;
	const unsigned int length = 1;
	char read_data[length];
	int fd = open(DEVICE, O_RDONLY);
	if(fd == -1) {
		printf("File %s not exist or is in use.\n", DEVICE);
		return fd;
	}
    read_length = read(fd, read_data, length);
    if(read_length > 0) {
        printf("%s\n", read_data);
    }

	return 0;
}
