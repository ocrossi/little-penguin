#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/jiffies.h>
#include <linux/mutex.h>
#include <linux/slab.h>

#define LOGIN "ocrossi\n"
#define LOGIN_LEN 9

static struct dentry *debugfs_dir;
static struct dentry *id_file;
static struct dentry *jiffies_file;
static struct dentry *foo_file;

static char data[PAGE_SIZE];
static size_t data_len;

static DEFINE_MUTEX(foo_mutex);

// ID FILE OPERATIONS
static ssize_t id_read(struct file *filep, char *user_buff, size_t user_len, loff_t *ppos)
{
	return simple_read_from_buffer(user_buff, user_len, ppos, LOGIN, LOGIN_LEN);
}

static ssize_t id_write(struct file *filep, const char *user_buff, size_t user_len, loff_t *ppos)
{
	ssize_t ret;
	char kernel_buffer[256] = {0};

	ret = simple_write_to_buffer(kernel_buffer, LOGIN_LEN, ppos, user_buff, user_len);
	if (ret > 0) {
		if (memcmp(kernel_buffer, LOGIN, LOGIN_LEN - 1) == 0)
			return LOGIN_LEN;
	}
	pr_err("id write operation failed\n");
	return -EINVAL;
}

static struct file_operations id_file_ops = {
	.read = id_read,
	.write = id_write,
};

// JIFFIES FILE OPERATIONS
static ssize_t jiffies_read(struct file *filep, char *user_buff, size_t user_len, loff_t *ppos)
{
	int len;
	unsigned char jiffies_buffer[256] = {0};

	len = snprintf(jiffies_buffer, sizeof(jiffies_buffer), "%lu\n", jiffies);

	return simple_read_from_buffer(user_buff, user_len, ppos, jiffies_buffer, len);
}

static struct file_operations jiffies_file_ops = {
	.read = jiffies_read,
};

// FOO FILE OPERATIONS
static ssize_t foo_read(struct file *filep, char *user_buff, size_t user_len, loff_t *ppos)
{
        if (*ppos > 0)
                return 0;

        mutex_lock(&foo_mutex);
        
        int err = copy_to_user(user_buff, data, data_len);
        mutex_unlock(&foo_mutex);
        
	if (err) {
                pr_err("foo_read failed\n");
                return err;
        }
        *ppos += data_len;
	return (ssize_t)data_len;
}

static ssize_t foo_write(struct file *filep, const char *user_buff, size_t user_len, loff_t *ppos)
{
        if (user_len > PAGE_SIZE)
                return -EINVAL;

        mutex_lock(&foo_mutex);
        int ret = copy_from_user(data, user_buff, user_len);
        if (ret) {
                mutex_unlock(&foo_mutex);
                pr_err("foo write failed\n");
                return ret;
        }

        data_len = user_len;
        mutex_unlock(&foo_mutex);
	return user_len;
}

static struct file_operations foo_file_ops = {
	.read = foo_read,
	.write = foo_write,
};


static int __init hello_init(void)
{
	debugfs_dir = debugfs_create_dir("fortytwo", NULL);
	if (!debugfs_dir || IS_ERR(debugfs_dir)) {
		pr_err("Error creating debugFs dir fortytwo\n");
		return PTR_ERR(debugfs_dir);
	}
	pr_info("DebugFs dir fortytwo correctly created\n");

	id_file = debugfs_create_file("id", 0666, debugfs_dir, NULL, &id_file_ops);
	if (!id_file || IS_ERR(id_file)) {
		pr_err("Error creating debugFs file id in directory fortytwo\n");
		return PTR_ERR(debugfs_dir);
	}
	pr_info("DebugFs file id in fortytwo directory correctly created\n");

	jiffies_file = debugfs_create_file("jiffies", 0444, debugfs_dir, NULL, &jiffies_file_ops);
	if (!jiffies_file || IS_ERR(jiffies_file)) {
		pr_err("Error creating debugFs file jiffies in directory fortytwo\n");
		return PTR_ERR(debugfs_dir);
	}
	pr_info("DebugFs file jiffies in fortytwo directory correctly created\n");

	foo_file = debugfs_create_file("foo", 0644, debugfs_dir, NULL, &foo_file_ops);
	if (!foo_file || IS_ERR(foo_file)) {
		pr_err("Error creating debugFs file foo in directory fortytwo\n");
		return PTR_ERR(debugfs_dir);
	}
	pr_info("DebugFs file foo in fortytwo directory correctly created\n");

	return 0;
}

static void __exit hello_exit(void) {
	debugfs_remove_recursive(debugfs_dir);
	pr_info("Cleaning up module, removing debugfs dir fortytwo\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple debugFs module");
MODULE_AUTHOR("Octave ROSSI");
