#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ocrossi");
MODULE_DESCRIPTION("A simple module to create a misd device called fortytwo");
MODULE_VERSION("1.0");




static int my_open(struct inode *inodeptr, struct file *file) {
	printk(KERN_INFO "Device opened\n");
	return 0;
}


static int my_read(struct file *file, char __user *buf, size_t len, loff_t *off) {
	printk(KERN_INFO "Device read\n");
	return 0;
}

static int my_release(struct file *file, char __user *buf, size_t len, loff_t *off) {
	printk(KERN_INFO "Device closed\n");
	return 0;
}

// static ssize_t my_write(struct file *file, char __user *buf, size_t len, loff_t *off) {
// 	printk(KERN_INFO "Device write\n");
// 	if (ft_strcmp(buf, "ocrossi")) {
//
// 	}
// 	return len;
// }

static int my_write(struct file *file, const char __user *buf, size_t len, loff_t *off) {
    char *kbuf;
    ssize_t ret;

    // Allocate kernel buffer
    kbuf = kzalloc(len, GFP_KERNEL);
    if (!kbuf) {
        printk(KERN_ERR "Failed to allocate kernel buffer\n");
        return -ENOMEM;
    }

    // Copy data from user space to kernel space
    ret = copy_from_user(kbuf, buf, len);
    if (ret) {
        printk(KERN_ERR "Failed to copy data from user space\n");
        kfree(kbuf);
        return -EFAULT;
    }

    // Process the data (for demonstration, we just print it)
    printk(KERN_INFO "Data written: %.*s\n", (int)len, kbuf);

    // Free the kernel buffer
    kfree(kbuf);

    // Return the number of bytes written
    return len;
}

static struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .read = my_read,
    .write = my_write,
};

static struct miscdevice my_misc_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "fortytwo",
    .fops = &my_fops,
};

static int __init my_init(void) {
	int ret;
	ret = misc_register(&my_misc_device);
	if (ret) {
		printk(KERN_INFO "failed to register misc device\n");
		return ret;
	}
	printk(KERN_INFO "Misc device registered\n");
	return 0;
}

static void __exit my_exit(void) {
	misc_deregister(&my_misc_device);
	printk(KERN_INFO "USB Keyboard Module unloaded\n");
}

module_init(my_init);
module_exit(my_exit);
