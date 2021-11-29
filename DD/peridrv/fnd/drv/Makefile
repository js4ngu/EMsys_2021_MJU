obj-m := fnddrv.o
KDIR := /home/cndi/ecube/linux_kernel/
PWD := $(shell pwd)

export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-
all:
	        $(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	        -rm *.o *.ko *.mod.c .*.cmd modules.order Module.symvers
		
