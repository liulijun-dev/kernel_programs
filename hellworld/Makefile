#ifneq ($(KERNELRELEASE),)
obj-m := hello.o
hello-objs:=helloworld.o
#else
KERNELDIR := /lib/modules/$(shell uname -r)/build
PWD := `pwd`
#endif

default:
	make -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm -rf *.o *.cmd *.ko *.mod.c .tmp_versions *.order *.symvers 
