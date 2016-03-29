target = irq_pins
driver = irq_pins_driver
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
	insmod $(target).ko pins=x, y

remove:
	rmmod $(target)
