obj-m := tlc5947.o

all: clean
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f /dev/tlc5947

load:
	insmod tlc5947.ko tlc5947_data=17 tlc5947_clock=27 tlc5947_latch=22

unload:
	rmmod -rf tlc5947

device:
	mknod /dev/tlc5947 c 243 0
	chmod 777 /dev/tlc5947
