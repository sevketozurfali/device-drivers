#include <linux/module.h>
#include <linux/init.h>
#include <linux/hrtimer.h>
#include <linux/jiffies.h>
#include <linux/gpio.h>
#include <linux/gpio/consumer.h>

#define IO_LED 21
#define IO_OFFSET 512

static struct gpio_desc *led;

static struct hrtimer my_hrtimer;

u64 start_t;

static enum hrtimer_restart test_hrtimer_handler(struct hrtimer *timer){
    unsigned long now_t = jiffies;

    pr_info("start_t - now_t = %lu\n", jiffies_to_msecs(now_t - start_t));

    if(gpiod_get_value(led)){
        gpiod_set_value(led, 0);
    }
    else {
        gpiod_set_value(led, 1);
    }
    
    hrtimer_forward(&my_hrtimer, hrtimer_cb_get_time(&my_hrtimer), ms_to_ktime(1000));

    return HRTIMER_RESTART;
}


static int __init my_init(void)
{
    int status;
    pr_info("hello_cdev - hello kernel\n");

    hrtimer_init(&my_hrtimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);

    my_hrtimer.function = &test_hrtimer_handler;

    start_t = jiffies;

    hrtimer_start(&my_hrtimer, ms_to_ktime(1000), HRTIMER_MODE_REL);

    led = gpio_to_desc(IO_LED + IO_OFFSET);
    if(!led) {
        printk("gpioctrl - Error getting pin 21\n");
        return -ENODEV;
    }

    status = gpiod_direction_output(led, 0);
    if(status) {
        printk("gpioctrl - Error setting pin 20 to output\n");
        return status;
    }

    return 0;
}

static void __exit my_exit(void)
{
    pr_info("hello - GoodBye Kernel!\n");
    hrtimer_cancel(&my_hrtimer);
    gpiod_set_value(led, 0);

}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sevket Ozurfali <sevketozurfali@gmail.com>");
MODULE_DESCRIPTION("This is the led driver module.");
