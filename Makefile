obj-m := test.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

activate:
	insmod test.ko tlc5947_pwm=4000

remove:
	rmmod test
