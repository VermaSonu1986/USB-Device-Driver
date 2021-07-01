/*
*	@file:		usb_driver.c
*	@author:	Sonu Verma
*	@date:		23 april 2021
*	@brif:		geneal LKM USB Driver
*/


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sonu Verma");
MODULE_DESCRIPTION("USB Pen Registration Driver");

static int pen_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    printk(KERN_INFO "Pen drive (%04X:%04X) plugged\n", id->idVendor,
                                id->idProduct);
    return 0;
}

static void pen_disconnect(struct usb_interface *interface)
{
    printk(KERN_INFO "Pen drive removed\n");
}

static struct usb_device_id pen_table[] =
{
    { USB_DEVICE(0x1a86, 0x7523) },
    {} /* Terminating entry */
};
MODULE_DEVICE_TABLE (usb, pen_table);

static struct usb_driver pen_driver =
{
    .name = "pen_driver",
    .id_table = pen_table,
    .probe = pen_probe,
    .disconnect = pen_disconnect,
};

static int __init pen_init(void)
{
    int ret = usb_register(&pen_driver);
	printk(KERN_INFO "usb_register return value %d", ret);
	if (ret)
	{
		printk(KERN_INFO "Pen drive Init failed %d \n", ret);
	}

    return ret;
}

static void __exit pen_exit(void)
{
    usb_deregister(&pen_driver);
}

module_init(pen_init);
module_exit(pen_exit);

