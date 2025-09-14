#include <linux/module.h>
#include <linux/init.h>
#include <linux/hrtimer.h>


static struct hrtimer my_timer;
u64 start_t;

static enum hrtimer_restart test_hrtimer_handler(struct hrtimer *timer){
    u64 now_t = jiffies;
    printk("start_t - now_t = %u\n", jiffies_to_msecs(now_t - start_t));
    return HRTIMER_NORESTART;
}

static int __init my_init(void)
{
    printk("hello - Hello kernel!\n");
    hrtimer_init(&my_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    my_timer.function = &test_hrtimer_handler;
    start_t = jiffies;
    hrtimer_start(&my_timer, ms_to_ktime(100), HRTIMER_MODE_REL);
    return 0;
}

static void __exit my_exit(void)
{
    hrtimer_cancel(&my_timer);
    printk("hello - GoodBye Kernel!\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sevket Ozurfali <sevketozurfali@gmail.com>");
MODULE_DESCRIPTION("This is better of the helloworld module.");