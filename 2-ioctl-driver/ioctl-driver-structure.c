#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/slab.h>
#include<linux/uaccess.h>
#include<linux/ioctl.h>

#include "data-types.h"

#define mem_size 1024

// Define the ioctl code


 #define WR_DATA _IOW('a', 'a', data_t*)
 #define RD_DATA _IOR('a', 'b', data_t*)

 static data_t val;

dev_t dev = 0;
static struct class *dev_class;
static struct cdev my_cdev;
uint8_t *kernel_buffer;

static int __init chr_driver_init(void);
static void __exit chr_driver_exit(void);

static int chr_open(struct inode *inode, struct file *file);
static int chr_release(struct inode *inode, struct file *file);
static ssize_t chr_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t chr_write(struct file *filp, const char *buf, size_t len, loff_t *off);

static long chr_ioctl(struct file *file, unsigned int cmd, unsigned long arg);


static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = chr_read,
    .write = chr_write,
    .open = chr_open,
    .unlocked_ioctl = chr_ioctl,
    .release = chr_release,
};

static int chr_open(struct inode *inode, struct file *file){
    // Creating physical memmory
    if((kernel_buffer = kmalloc(mem_size, GFP_KERNEL)) == 0){
        printk(KERN_INFO "Cannot allocate the memory to the kernel \n");
        return -1;
    }
    printk(KERN_INFO "Device file opened... \n");
    return 0;
}

static int chr_release(struct inode *inode, struct file *file){
    kfree(kernel_buffer);
    printk(KERN_INFO "Device file closed.. \n");
    return 0;
}

static ssize_t chr_read(struct file *filp, char __user *buf, size_t len, loff_t *off){
    copy_to_user(buf,kernel_buffer,mem_size);
    printk(KERN_INFO "Data read: Done . \n");
    return mem_size;
}

static ssize_t chr_write(struct file *filp, const char __user *buf, size_t len, loff_t *off){
    copy_from_user(kernel_buffer, buf, len);
    printk(KERN_INFO "Data is written successfully....\n");
    return len;
}

static long chr_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
    switch (cmd)
    {
    case WR_DATA:
        copy_from_user(&val, (data_t*)arg, sizeof(val));
        printk(KERN_INFO "val = %d - %d\n", val.num1, val.num2);
        break;
    case RD_DATA:
        copy_to_user((data_t*)arg, &val, sizeof(val));
        break;
    default:
        break;
    }
    return 0;
}


static int __init chr_driver_init(void){
    /* Allocate major number*/
    if((alloc_chrdev_region(&dev,0,1,"my_dev")) < 0){
        printk(KERN_INFO "Can not allocate the major number...\n");
        return -1;
    }

    printk(KERN_INFO"Major number : %d minor number : %d",MAJOR(dev),MINOR(dev));

    /* creating cdev structure */
    cdev_init(&my_cdev,&fops);

    /* adding character device to the system */
    if((cdev_add(&my_cdev,dev,1)) < 0){
        printk(KERN_INFO"Cannot add the device to the system...\n");
        goto r_class;
    }

    //  creating struct class 
    if((dev_class = class_create("my_class")) == NULL){
        printk(KERN_INFO"Cannot create struct device \n");
        goto r_class;
    }

    // creating device 
    if((device_create(dev_class,NULL,dev,NULL,"ioctl_device")) == NULL){
        printk(KERN_INFO"cannot create the device...\n");
        goto r_device;
    }

    printk(KERN_INFO"Device driver insert....done ! properly\n");
    return 0;

r_device:
    class_destroy(dev_class);

r_class:
    unregister_chrdev_region(dev,1);
    return -1;
    
}


void __exit chr_driver_exit(void){
    device_destroy(dev_class,dev);
    class_destroy(dev_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev,1);
    printk(KERN_INFO "Device driver is removed successfully.\n");
}

module_init(chr_driver_init);
module_exit(chr_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sevket ozurfali.");
MODULE_DESCRIPTION("The character device driver.");