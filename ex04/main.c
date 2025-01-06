#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init hello_init(void) {
    printk(KERN_INFO "Hello, USB World!\n");
    return 0;
}

static void __exit hello_exit(void) {
    printk(KERN_INFO "Cleaning up USB World module\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple Hello USB World module!");
MODULE_AUTHOR("Your Name");
