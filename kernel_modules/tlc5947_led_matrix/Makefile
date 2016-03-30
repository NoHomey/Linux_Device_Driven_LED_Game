target = tlc5947
driver = tlc5947_led_matrix
helpers = common_helpers

obj-m := $(target).o
$(target)-objs := $(driver).o $(helpers).o

all: $(helpers)
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

$(helpers):
	cp -a ./../$(helpers)/. ./

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f ./$(helpers).*
	rm -f /dev/$(target)

activate:
	insmod $(target).ko tlc5947_chips=8 tlc5947_data=17 tlc5947_clock=27 tlc5947_latch=22

remove:
	rmmod $(target)
	modprobe -rf $(target)

device:
	mknod /dev/$(target) c 243 0
	chmod 777 /dev/$(target)
