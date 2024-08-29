ifeq ($(KERNELRELEASE),)

KERNELDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	
modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install
	modprobe -r cp210x
	cp -f $(PWD)/cp210x.ko /lib/modules/$(shell uname -r)/kernel/drivers/usb/serial
	modprobe cp210x
	
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

.PHONY: modules modules_install clean

else

obj-m := cp210x.o

endif
