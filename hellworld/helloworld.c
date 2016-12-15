#include <linux/init.h>
#include <linux/module.h>

static int hello_init(void){
    printk(KERN_ALERT"Hello, World!\n");
    return 0;
}

static int hello_exit(void){
    printk(KERN_ALERT"Goodby,cruel world!\n");
}

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("yep");
module_init(hello_init);
module_exit(hello_exit);
