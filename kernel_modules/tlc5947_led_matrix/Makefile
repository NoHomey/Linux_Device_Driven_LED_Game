target = tlc5947
main = tlc5947_led_matrix
helpers = common_helpers

obj-m := $(target).o
$(target)-objs := $(main).o $(helpers).o

all: $(helpers)
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

$(helpers):
	cp -a ./../$(helpers)/. ./

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

activate:
	insmod $(target).ko tlc5947_pwm=4000

remove:
	rmmod $(target)
