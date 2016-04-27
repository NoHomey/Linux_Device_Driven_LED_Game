CONFIG_MODULE_SIG=n

obj-m:=input_pin.o

major=243

input_pin=6,13,19,26,17,27,22,5,18,23,24,25,12,16,20,21

all: clean
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load:
	insmod input_pin.ko input_pin=$(input_pin)

unload:
	rmmod -f input_pin

device:
	mknod /dev/input_pin c $(major) 0
	chmod 555 /dev/input_pin

remove:
	rm -f /dev/input_pin

test:
	gcc test.c -Wall -o test
	./test
	rm ./test
