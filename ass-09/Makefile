obj-m=mymounts.o

PWD=$(CURDIR)
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	insmod mymounts.ko

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
