#include <linux/module.h>
#include <linux/init.h>
#include <linux/spi/spi.h>


/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sevket Ozurfali <sevketozurfali@gmail.com>");
MODULE_DESCRIPTION("A simple LKM to read and write some registeres of the spi device.");

#define MY_BUS_NUM 0


static int __init my_init(void)
{
    printk("hello - Hello kernel!\n");
    return 0;
}

static void __exit my_exit(void)
{
    printk("hello - GoodBye Kernel!\n");
}

module_init(my_init);
module_exit(my_exit);

