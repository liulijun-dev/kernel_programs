#ifneq ($(KERNELRELEASE),)
obj-m := proc_example.o
proc_example-objs:=proc_lkm.o
#else
KERNELDIR := /lib/modules/$(shell uname -r)/build
PWD := `pwd`
#endif
EXTRA_CFLAGS += -w
default:
	make -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm -rf *.o *.cmd .*.cmd *.ko *.mod.c .tmp_versions *.order *.symvers 
