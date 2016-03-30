target = tlc5947
driver = tlc5947_led_matrix

obj-m := $(target).o
$(target)-objs := $(driver).o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f /dev/$(target)

activate:
	insmod $(target).ko tlc5947_chips=8 tlc5947_data=17 tlc5947_clock=27 tlc5947_latch=22

remove:
	rmmod -rf $(target)
	modprobe -rf $(target)

device:
	mknod /dev/$(target) c 243 0
	chmod 777 /dev/$(target)
