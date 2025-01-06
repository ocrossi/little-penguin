// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Octave Rossi");
MODULE_DESCRIPTION("Reverse module");

static ssize_t myfd_read(struct file *fp, char __user *user, size_t size,
		loff_t *offs);
static ssize_t myfd_write(struct file *fp, const char __user *user, size_t size,
		loff_t *offs);

static struct file_operations myfd_fops = {
	.owner = THIS_MODULE,
	.read = &myfd_read,
	.write = &myfd_write,
};

static struct miscdevice myfd_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "reverse",
	.fops = &myfd_fops
};

char str[PAGE_SIZE];
size_t str_length;

static ssize_t myfd_read(struct file *fp, char __user *user, size_t size, loff_t *offs)
{
	size_t ret = simple_read_from_buffer(user, size, offs, str, str_length);

	if (ret < 0)
		pr_err("simple read from buffer crashed\n");

	return ret;
}

static ssize_t myfd_write(struct file *fp, const char __user *user, size_t size, loff_t *offs)
{
	ssize_t res;
	size_t i, rev_i;
	char tmp;

	res = simple_write_to_buffer(str, sizeof(str) - 1, offs, user, size);

	if (res <= 0) {
		str_length = 0;
		str[0] = '\0';
	}

	str_length = res;

	for (i = 0, rev_i = str_length - 1; i < str_length / 2; i++, rev_i--) {
		tmp = str[i];
		str[i] = str[rev_i];
		str[rev_i] = tmp;
	}

	return res;
}

static int __init myfd_init(void)
{
	int retval;

	retval = misc_register(&myfd_device);

	if (retval) {
		pr_err("Couldn register misc device\n");
		return -retval;
	}

	return 0;
}

static void __exit myfd_cleanup(void)
{
	misc_deregister(&myfd_device);
	pr_info("Cleaning up module\n");
}

module_init(myfd_init);
module_exit(myfd_cleanup);
