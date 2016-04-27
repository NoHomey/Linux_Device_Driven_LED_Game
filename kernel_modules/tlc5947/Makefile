obj-m := tlc5947.o

major = 243
data = 17
clock = 27
latch = 22

all: clean
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load:
	insmod tlc5947.ko tlc5947_data=$(data) tlc5947_clock=$(clock) tlc5947_latch=$(latch)

unload:
	rmmod -f tlc5947

device:
	mknod /dev/tlc5947 c $(major) 0
	chmod 777 /dev/tlc5947

remove:
	rm -f /dev/tlc5947

header:
	rm -f /usr/include/tlc5947_ioctl.h
	cp tlc5947.h /usr/include/tlc5947_ioctl.h
	chmod 555 /usr/include/tlc5947_ioctl.h
