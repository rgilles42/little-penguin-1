obj-m=fortytwo.o

PWD=$(CURDIR)
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	insmod fortytwo.ko
	chmod 755 /sys/kernel/debug

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
