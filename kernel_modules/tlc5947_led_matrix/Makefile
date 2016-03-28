target = tlc5947_led_matrix

obj-m := $(target).o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

activate:
	insmod $(target).ko tlc5947_pwm=4000

remove:
	rmmod $(target)
