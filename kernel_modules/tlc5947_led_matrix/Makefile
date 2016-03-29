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
	rm ./$(target).*

activate:
	insmod $(target).ko tlc5947_pwm=4000

remove:
	rmmod $(target)
