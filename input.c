#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE "/dev/input_pins"

int main(void) {
	int fd = open(DEVICE, O_RDWR);
	const int length = 4;
	char read_data[length];
        int i, read_length;
	if(fd == -1) {
		printf("File %s not exist or is in use.\n", DEVICE);
		return fd;
	}
    while(1) {
        read_length = read(fd, read_data, length);
        if(read_length > 0) {
            for(i = 0; i < read_length; ++i) {
                printf("pin %d\n", read_data[i]);
            }
        }
    }
	close(fd);

	return 0;
}
