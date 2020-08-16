#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>         // Header to support the kernel Driver Model
#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include <linux/fs.h>             // Header for the Linux file system support
#include <linux/uaccess.h>

#include "chardev.h"    


#define DEBUG_ENA 1
#if (DEBUG_ENA == 1)
    #define print_db(...) printk(__VA_ARGS__)
#else
    #define print_db(...)
#endif

#define DEVICE_NAME "vsp_dev"
#define CLASS_NAME "vsp_class"

static int dev_major;
static struct class*  dev_class = NULL;
static struct device* dev_char = NULL;

static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

/* Is the device open right now? Used to prevent 
 * concurent access into the same device */
static int Device_Open = 0;

/* This function will be called when user open filesystem device */
static int dev_open(struct inode *inodep, struct file *filep) {
	if(Device_Open != 0){
    	print_db("#ERR: dev busy\n");
		return -EBUSY;
	}
    print_db("#INF: dev open\n");
	Device_Open ++;
    return 0;
}

/* This function will be called when user read filesystem device */
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, \
                                                        loff_t *offset) {
    print_db("#INF: device_read(%p,%p,%d)\n", filep, buffer, len);
    return 0;
}


/* This function will be called when user write filesystem device */
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, \
                                                            loff_t *offset) {
    print_db("#INF: dev write\n");
    return 0;
}

/* This function will be called when user close filesystem device */
static int dev_release(struct inode *inodep, struct file *filep) {
    print_db("#INF: dev release\n");
	Device_Open --;
    return 0;
}


static long my_ioctl (struct file *file, unsigned int cmd, unsigned long arg)
{
	print_db("#ioctl: cmd %d ; arg = %ld \n", cmd, arg);

	switch(cmd) {
		case IOCTL_GET_MSG:
			print_db("#ioctl: IOCTL_GET_MSG \n");
		break;
		case IOCTL_GET_NTH_BYTE:
			print_db("#ioctl: IOCTL_GET_NTH_BYTE \n");
		break;
		case IOCTL_SET_MSG:
			print_db("#ioctl: IOCTL_GET_MSG \n");
		break;
		default:
		return -ENOTTY;
	}

	return 0;
}


static struct file_operations fops =
{
	.owner      	= THIS_MODULE,
	.open 			= dev_open,
	.read			= dev_read,
	.write 			= dev_write,
	.release 		= dev_release,
	.unlocked_ioctl = my_ioctl,
};

static int __init comp_init(void) {
    print_db("Kernel module start\n");
    dev_major = register_chrdev(0, DEVICE_NAME, &fops);
    if (dev_major < 0) {
        print_db("#ERR: Failed to register a major number\n");
        return dev_major;
    }
    print_db("#INF: registered a major number %d\n", dev_major);

    dev_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(dev_class)) {
        unregister_chrdev(dev_major, DEVICE_NAME);
        print_db("Failed to register device class\n");
        return PTR_ERR(dev_class);
    }
    print_db("#INF: device class registered correctly\n");

    dev_char = device_create(dev_class, NULL, MKDEV(dev_major, 0), NULL, DEVICE_NAME);
    if (IS_ERR(dev_char)) {
        class_destroy(dev_class);
        unregister_chrdev(dev_major, DEVICE_NAME);
        print_db("#ERR: Failed to create the device\n");
        return PTR_ERR(dev_char);
    }
    print_db("#INF: device created correctly\n");
    return 0;
}

static void __exit comp_exit(void) {
    print_db("Kernel module exit\n");
    device_destroy(dev_class, MKDEV(dev_major, 0));     // remove the device
    class_unregister(dev_class);                          // unregister the device class
    class_destroy(dev_class);                             // remove the device class
    unregister_chrdev(dev_major, DEVICE_NAME);             // unregister the major number
    print_db("#INF: Remove device driver is completed!\n");
}

module_init(comp_init);
module_exit(comp_exit);
MODULE_AUTHOR("Phuong Nguyen");
MODULE_LICENSE("GPL");