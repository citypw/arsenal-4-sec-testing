KERNEL_DIR=/usr/src/linux-2.6.39
KERNEL_HEADERS = -l$(KERNEL_DIR)/include

obj-m:=url_redirect.o
PWD = $(shell pwd)

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	rm -rf *.ko
	rm -rf *.o
	rm -rf *.mod.*
	rm -rf Module.*
	rm -rf modules.*
	rm -rf .*
