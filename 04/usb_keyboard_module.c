#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("ocrossi");
MODULE_DESCRIPTION("A simple module to detect USB keyboard & mouse");
MODULE_VERSION("1.0");

static int __init usb_keyboard_module_init(void) {
	printk(KERN_INFO "USB Keyboard Module loaded\n");
	return 0;
}

static void __exit usb_keyboard_module_exit(void) {
	printk(KERN_INFO "USB Keyboard Module unloaded\n");
}

module_init(usb_keyboard_module_init);
module_exit(usb_keyboard_module_exit);
