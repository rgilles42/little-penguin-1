A USB keyboard is an USB Device of class 0x03 (HID Device), subclass 0x01(Boot Interface) and Interface Protocol 0x01 (Keyboard).  
I.e.
```
rgilles [ ~/little-penguin-1 ]$ sudo lsusb -v
[sudo] password for rgilles: 

Bus 001 Device 006: ID 05ac:024f Apple, Inc. Aluminium Keyboard (ANSI)
Device Descriptor:
  (...)
  Configuration Descriptor:
    (...)
    Interface Descriptor:
      (...)
      bInterfaceClass         3 Human Interface Device
      bInterfaceSubClass      1 Boot Interface Subclass
      bInterfaceProtocol      1 Keyboard
  (...)
Device Status:     0x0000
  (Bus Powered)
```

Thus, we need to create a `udev` rule to filter plugged-in devices down to those corresponding to these values.  
The `udev` rule will run something upon insertion of the device, such as the insertion of a kernel module...


And when a usb keyboard is plugged, for instance with the qemu command `device_add usb-kbd,id=mykeyboard`
```sh
dmesg | tail -9
[ 6373.394008] usb 2-2: new high-speed USB device number 9 using ehci-pci
[ 6373.540644] usb 2-2: New USB device found, idVendor=0627, idProduct=0001, bcdDevice= 0.00
[ 6373.542655] usb 2-2: New USB device strings: Mfr=1, Product=4, SerialNumber=11
[ 6373.544393] usb 2-2: Product: QEMU USB Keyboard
[ 6373.546159] usb 2-2: Manufacturer: QEMU
[ 6373.547907] usb 2-2: SerialNumber: 68284-0000:00:1d.7-2
[ 6373.554941] input: QEMU QEMU USB Keyboard as /devices/pci0000:00/0000:00:1d.7/usb2/2-2/2-2:1.0/0003:0627:0001.0008/input/input12
[ 6373.609417] hid-generic 0003:0627:0001.0008: input,hidraw0: USB HID v1.11 Keyboard [QEMU QEMU USB Keyboard] on usb-0000:00:1d.7-2/input0
[ 6373.620581] Hello world !
```
