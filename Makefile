#obj-m+=usb_driver.o
obj-m+=usb_driver_transfer.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	#make COPTS=-g
	$(CC) -g user_application_interface.c -o user_application_interface
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
