obj-m=fortytwo.o

PWD=$(CURDIR)
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	insmod fortytwo.ko
	chmod 666 /dev/fortytwo

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
