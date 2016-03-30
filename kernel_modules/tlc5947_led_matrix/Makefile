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
	rm ./$(helpers).*

activate:
	insmod $(target).ko tlc5947_chips=8 tlc5947_data=17 tlc5947_clock=27

remove:
	rmmod $(target)
