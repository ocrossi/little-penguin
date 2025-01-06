#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/printk.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Octave ROSSI");
MODULE_DESCRIPTION("FortyTwo misc char device driver");
MODULE_VERSION("1.0");

#define DEVICE_NAME "fortytwo"
#define LOGIN "ocrossi\n"
#define LOGIN_LEN 9

static ssize_t my_read(struct file *filep, char *user_buffer, size_t user_len, loff_t *ppos)
{
	return simple_read_from_buffer(user_buffer, user_len, ppos, LOGIN, LOGIN_LEN);
}

static ssize_t my_write(struct file *filep, const char *user_buffer, size_t user_len, loff_t *ppos)
{
	ssize_t ret;
	char kernel_buffer[256];

	ret = simple_write_to_buffer(kernel_buffer, 256, ppos, user_buffer, user_len);
	pr_info("return from simple_write %lu\n", ret);
	
	if (ret > 0) {
		if (memcmp(kernel_buffer, LOGIN, LOGIN_LEN - 1) == 0) {
			pr_info("returns properly\n");
			return LOGIN_LEN;
		}
		pr_info("write did not match with login\n");
	}
	pr_err("We got a write but not the login expected\n");
	return -EINVAL;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = my_read,
	.write = my_write,
};

static struct miscdevice my_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &fops,
};

static int __init ft_init(void)
{
	int ret;
	printk(KERN_INFO "FortyTwo: Initializing the FortyTwo Misc char device driver\n");

	ret = misc_register(&my_misc_device);
	if (ret) {
		pr_err("FortyTwo: failed to register misc device\n");
		return ret;
	}
	pr_info("FortyTwo: registered correctly with minor number %d\n", my_misc_device.minor);
	return 0;
}

static void __exit ft_exit(void)
{
	misc_deregister(&my_misc_device);
	printk(KERN_INFO "FortyTwo: Goodbye from the FortyTwo\n");
}

module_init(ft_init);
module_exit(ft_exit);
