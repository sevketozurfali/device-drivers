#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio/consumer.h>
#include <linux/gpio.h>


static struct gpio_desc *led, *button;

#define IO_LED 21
#define IO_BUTTON 20

#define IO_OFFSET 512

static int __init my_init(void)
{
    int status;

    led = gpio_to_desc(IO_LED + IO_OFFSET);
    if(!led) {
        printk("gpioctrl - Error getting pin 21\n");
        return -ENODEV;
    }

    button = gpio_to_desc(IO_BUTTON + IO_OFFSET);
    if(!button) {
        printk("gpioctrl - Error getting pin 20\n");
        return -ENODEV;
    }

    status = gpiod_direction_output(led, 0);
    if(status) {
        printk("gpioctrl - Error setting pin 20 to output\n");
        return status;
    }

    status = gpiod_direction_input(button);
    if(status) {
        printk("gpiocntrl - Error setting pin 21 to input");
        return status;
    }

    gpiod_set_value(led, 1);

    printk("gpioctrl - button is %s pressed. \n", gpiod_get_value(button) ? "not" : "" );

    printk("gpiobutton : %d \n", gpiod_get_value(button));

    return 0;
}

static void __exit my_exit(void)
{
    gpiod_set_value(led, 0);
    printk("hello - GoodBye Kernel!\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sevket Ozurfali <sevketozurfali@gmail.com>");
MODULE_DESCRIPTION("This is the led driver module.");
