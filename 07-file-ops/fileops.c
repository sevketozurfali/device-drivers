#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

static int major;

// ssize_t my_read (struct file *f, char __user *u, size_t l, loff_t *o){
//     printk("hello_cdev - Read is called\n");
//     return 0;
// }

static int my_open (struct inode *inode, struct file *filp){
    pr_info("hello_cdev - Major : %d, Minor %d \n", imajor(inode), iminor(inode));
    return 0;
}

static int my_release (struct inode *inode, struct file *filp){
    pr_info("hello_cdev - file closed.");
    return 0;
}

static struct file_operations fops = {
    // .read = my_read
    .open = my_open,
    .release = my_release,
};

static int __init my_init(void)
{
    major = register_chrdev(0, "crechar", &fops);
    if(major < 0){
        pr_err("hello_cdev - Error registering chrdev\n");
        return major;
    }

    pr_info("hello_cdev - Major device number : %d\n", major);

    return 0;
}

static void __exit my_exit(void)
{

    pr_info("hello - GoodBye Kernel!\n");
    unregister_chrdev(major, "crechar");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sevket Ozurfali <sevketozurfali@gmail.com>");
MODULE_DESCRIPTION("This is the led driver module.");
