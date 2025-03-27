#include <linux/module.h>
#include <linux/init.h>


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

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sevket Ozurfali <sevketozurfali@gmail.com>");
MODULE_DESCRIPTION("This is better of the helloworld module.");
