obj-m := gpioctrl.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
		$(MAKE) -C $(KDIR) M=$(PWD) modules
install:
		$(MAKE) -C $(KDIR) KBUILD_EXTMOD=$(PWD) module_install
clean:
		$(MAKE) -C $(KDIR) KBUILD_EXTMOD=$(PWD) clean
