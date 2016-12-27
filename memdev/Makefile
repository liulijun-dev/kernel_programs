obj-m := mymemdev.o
mymemdev-objs := memdev.o
KDIR :=/lib/modules/`uname -r`/build
PWD := $(shell pwd)

default: 
	make -C $(KDIR) M=$(PWD) modules

clean: 
	rm -rf *.o *.order *.mod.c *.mod.o *.ko *.symvers
