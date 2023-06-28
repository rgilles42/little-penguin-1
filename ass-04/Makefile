obj-m=hello_world.o

PWD=$(CURDIR)
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	sudo cp hello_world.ko /lib/modules/$(shell uname -r)/misc/
	echo 'ACTION=="add", SUBSYSTEMS=="usb", ATTRS{bInterfaceClass}=="03", ATTRS{bInterfaceSubClass}=="01", ATTRS{bInterfaceProtocol}=="01",' \
			'RUN+="/usr/sbin/insmod /lib/modules/$(shell uname -r)/misc/hello_world.ko"' \
		| sudo tee /etc/udev/rules.d/99-usbkeyboard_helloworld.rules > /dev/null
	sudo udevadm control --reload

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	sudo rm -f /lib/modules/$(shell uname -r)/misc/hello_world.ko
	sudo rm -f /etc/udev/rules.d/99-usbkeyboard_helloworld.rules
	sudo udevadm control --reload
	sudo /usr/sbin/rmmod hello_world
