obj-m=hello_world.o

PWD=$(CURDIR)
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	cp hello_world.ko /lib/modules/$(shell uname -r)/
	echo 'ACTION=="add", SUBSYSTEMS=="usb", ATTRS{bInterfaceClass}=="03", ATTRS{bInterfaceSubClass}=="01", ATTRS{bInterfaceProtocol}=="01",' \
			'RUN+="/usr/sbin/insmod /lib/modules/$(shell uname -r)/hello_world.ko"' \
		> /etc/udev/rules.d/99-usbkeyboard_helloworld.rules
	udevadm control --reload

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f /lib/modules/$(shell uname -r)/hello_world.ko
	rm -f /etc/udev/rules.d/99-usbkeyboard_helloworld.rules
	udevadm control --reload
	rmmod hello_world
