/*
*	@file:		usb_driver_transfer.c
*	@author:	Sonu Verma
*	@date:		24 april 2021
*	brif:		USB Linux device driver for small data transfer
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sonu Verma");
MODULE_DESCRIPTION("USB Pen Device Driver for data transfer");

// USB device information - 
/*
T:  Bus=02 Lev=01 Prnt=01 Port=01 Cnt=02 Dev#=  5 Spd=12   MxCh= 0
D:  Ver= 1.10 Cls=ff(vend.) Sub=00 Prot=00 MxPS= 8 #Cfgs=  1
P:  Vendor=1a86 ProdID=7523 Rev= 2.64
S:  Product=USB Serial
C:* #Ifs= 1 Cfg#= 1 Atr=80 MxPwr= 98mA
I:* If#= 0 Alt= 0 #EPs= 3 Cls=ff(vend.) Sub=01 Prot=02 Driver=ch341
E:  Ad=82(I) Atr=02(Bulk) MxPS=  32 Ivl=0ms
E:  Ad=02(O) Atr=02(Bulk) MxPS=  32 Ivl=0ms
E:  Ad=81(I) Atr=03(Int.) MxPS=   8 Ivl=1ms
*/

	// Sandisk Pen Drive
/*
T:  Bus=01 Lev=01 Prnt=01 Port=00 Cnt=01 Dev#=  3 Spd=480  MxCh= 0
D:  Ver= 2.00 Cls=00(>ifc ) Sub=00 Prot=00 MxPS=64 #Cfgs=  1
P:  Vendor=0781 ProdID=5567 Rev= 1.27
S:  Manufacturer=SanDisk
S:  Product=Cruzer Blade
S:  SerialNumber=4C530001130710107363
C:* #Ifs= 1 Cfg#= 1 Atr=80 MxPwr=200mA
I:* If#= 0 Alt= 0 #EPs= 2 Cls=08(stor.) Sub=06 Prot=50 Driver=usb-storage
E:  Ad=81(I) Atr=02(Bulk) MxPS= 512 Ivl=0ms
E:  Ad=02(O) Atr=02(Bulk) MxPS= 512 Ivl=125us
*/

#define MIN(a,b) (((a) <= (b)) ? (a) : (b))
#define BULK_EP_OUT 		0x02
#define BULK_EP_IN 		0x81
#define MAX_PKT_SIZE 		512
#define VENDOR_ID		0x0781
#define PRODUCT_ID		0x5567

static struct usb_device *device;
static struct usb_class_driver class;
static unsigned char bulk_buf[MAX_PKT_SIZE];
static int usb_count;
static int wrote_cnt;

static int pen_open(struct inode *i, struct file *f)
{
	usb_count++;
	printk(KERN_INFO "KERNAL: USB Device open count %d\n", usb_count);
    return 0;
}
static int pen_close(struct inode *i, struct file *f)
{
	printk(KERN_INFO "KERNAL: USB Device closed");
    return 0;
}
static ssize_t pen_read(struct file *f, char __user *buf, size_t cnt, loff_t *off)
{
    int retval;
    int read_cnt;

    /* Read the data from the bulk endpoint */
    retval = usb_bulk_msg(device, usb_rcvbulkpipe(device, BULK_EP_IN),
            bulk_buf,/* MAX_PKT_SIZE*/wrote_cnt, &read_cnt, 5000);
    if (retval)
    {
        printk(KERN_ERR "KERNAL: failed to read data from the USB endpoint %d with retvalue %d\n", BULK_EP_IN, retval);
        return retval;
    }
    if (copy_to_user(buf, bulk_buf, /*MIN(cnt, read_cnt)*/wrote_cnt))
    {
		printk(KERN_INFO "KERNAL: failed to copy data from kernel to userspace");
        return -EFAULT;
    }

    return MIN(cnt, read_cnt);
}
static ssize_t pen_write(struct file *f, const char __user *buf, size_t cnt,
                                    loff_t *off)
{
    int retval;
    wrote_cnt = MIN(cnt, MAX_PKT_SIZE);

    if (copy_from_user(bulk_buf, buf, MIN(cnt, MAX_PKT_SIZE)))
    {
	printk(KERN_INFO "KERNAL: failed to copy data from userspace to kernel");
        return -EFAULT;
    }

    /* Write the data into the bulk endpoint */
    retval = usb_bulk_msg(device, usb_sndbulkpipe(device, BULK_EP_OUT),
            bulk_buf, MIN(cnt, MAX_PKT_SIZE), &wrote_cnt, 5000);
    if (retval)
    {
        printk(KERN_ERR "KERNAL: failed to write data to the USB endpoint %d with retvalue %d\n", BULK_EP_OUT, retval);
        return retval;
    }
    else
    {
	printk(KERN_INFO "write success %s", bulk_buf);
    }

    return wrote_cnt;
}

static struct file_operations fops =
{
    .owner = THIS_MODULE,
    .open = pen_open,
    .release = pen_close,
    .read = pen_read,
    .write = pen_write,
};

static int pen_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    int retval;

    device = interface_to_usbdev(interface);

    class.name = "usb/pen%d";
    class.fops = &fops;
    if ((retval = usb_register_dev(interface, &class)) < 0)
    {
        /* Something prevented us from registering this driver */
        printk(KERN_ERR "KERNAL: Not able to get a minor for this device.");
    }
    else
    {
        printk(KERN_INFO "KERNAL: Minor obtained: %d\n", interface->minor);
		printk(KERN_INFO "KERNAL: USB device (%04X:%04X) plugged\n", id->idVendor,
                                id->idProduct);
    }

    return retval;
}

static void pen_disconnect(struct usb_interface *interface)
{
    usb_deregister_dev(interface, &class);
	printk(KERN_INFO "KERNAL: USB device removed\n");
}

/* Table of devices that work with this driver */
static struct usb_device_id pen_table[] =
{
    { USB_DEVICE(VENDOR_ID, PRODUCT_ID) },
    {} /* Terminating entry */
};
MODULE_DEVICE_TABLE (usb, pen_table);

static struct usb_driver pen_driver =
{
    .name = "pen_driver",
    .probe = pen_probe,
    .disconnect = pen_disconnect,
    .id_table = pen_table,
};

static int __init pen_init(void)
{
    int result;

    /* Register this driver with the USB subsystem */
    if ((result = usb_register(&pen_driver)))
    {
        printk(KERN_ERR "KERNAL: usb_register failed. Error number %d", result);
    }
    return result;
}

static void __exit pen_exit(void)
{
    /* Deregister this driver with the USB subsystem */
    usb_deregister(&pen_driver);
}

module_init(pen_init);
module_exit(pen_exit);
