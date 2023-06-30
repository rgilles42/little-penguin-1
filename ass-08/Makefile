obj-m=coding_style.o

PWD=$(CURDIR)
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	insmod coding_style.ko
	chmod 766 /dev/reverse

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
