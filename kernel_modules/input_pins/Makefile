obj-m := input_pins.o

major = 243

all: clean
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load:
	insmod input_pins.ko input_pins=17,22,27

unload:
	rmmod -f input_pins

device:
	mknod /dev/input_pins c ${major} 0
	chmod 777 /dev/input_pins

remove:
	rm -f /dev/input_pins
