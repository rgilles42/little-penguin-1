// SPDX-License-Identifier: GPL-2.0
/*
 * fortytwo - A sample debugfs filesystem.
 *	- rgilles@student.42.fr
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/fs.h>
#include <linux/debugfs.h>
#include <linux/jiffies.h>
#include <linux/mutex.h>

struct dentry *directory;
struct dentry *id;
struct dentry *jiffies_whisperer;
struct dentry *foo;

static ssize_t ft_id_write(struct file *file, const char __user *buf, size_t len, loff_t *ppos)
{
	char str[8];
	ssize_t l;

	l = simple_write_to_buffer(str, 8, ppos, buf, len);
	if ((l == 7 && memcmp(str, "rgilles", l) == 0) || (l == 8 && memcmp(str, "rgilles\n", l) == 0))
		return len;
	else
		return -EINVAL;
}

static ssize_t ft_id_read(struct file *file, char __user *buf, size_t count, loff_t *f_pos)
{
	return simple_read_from_buffer(buf, count, f_pos, "rgilles", 7);
}

static const struct file_operations id_fops = {
	.owner          = THIS_MODULE,
	.write          = ft_id_write,
	.read           = ft_id_read,
};

static ssize_t ft_jiffies_read(struct file *file, char __user *buf, size_t count, loff_t *f_pos)
{
	char str[64];
	ssize_t len;

	len = snprintf(str, 64, "%lu", jiffies);
	return simple_read_from_buffer(buf, count, f_pos, str, len);
}

static const struct file_operations jiffies_fops = {
	.owner			= THIS_MODULE,
	.read			= ft_jiffies_read,
};

DEFINE_MUTEX(foo_mutex);
char foo_buf[PAGE_SIZE];
size_t foo_len;

static ssize_t ft_foo_write(struct file *file, const char __user *buf, size_t len, loff_t *f_pos)
{
	size_t l;

	mutex_lock(&foo_mutex);
	l = simple_write_to_buffer(foo_buf, PAGE_SIZE, f_pos, buf, len);
	foo_len = l;
	mutex_unlock(&foo_mutex);
	return l;
}

static ssize_t ft_foo_read(struct file *file, char __user *buf, size_t count, loff_t *f_pos)
{
	ssize_t ret;

	mutex_lock(&foo_mutex);
	ret = simple_read_from_buffer(buf, count, f_pos, foo_buf, foo_len);
	mutex_unlock(&foo_mutex);
	return ret;
}

static const struct file_operations foo_fops = {
	.owner			= THIS_MODULE,
	.write			= ft_foo_write,
	.read			= ft_foo_read,
};

static int __init fortytwo_init(void)
{
	directory = debugfs_create_dir("fortytwo", NULL);
	if (IS_ERR(directory)) {
		pr_err("fortytwo debugfs failed to load!\n");
		return -1;
	}
	id = debugfs_create_file("id", 0666, directory, NULL, &id_fops);
	if (IS_ERR(id)) {
		debugfs_remove(directory);
		pr_err("fortytwo debugfs failed to load!\n");
		return -1;
	}
	jiffies_whisperer = debugfs_create_file("jiffies", 0444, directory, NULL, &jiffies_fops);
	if (IS_ERR(jiffies_whisperer)) {
		debugfs_remove(id);
		debugfs_remove(directory);
		pr_err("fortytwo debugfs failed to load!\n");
		return -1;
	}
	foo = debugfs_create_file("foo", 0644, directory, NULL, &foo_fops);
	if (IS_ERR(foo)) {
		debugfs_remove(jiffies_whisperer);
		debugfs_remove(id);
		debugfs_remove(directory);
		pr_err("fortytwo debugfs failed to load!\n");
		return -1;
	}
	pr_info("fortytwo debugfs successfully loaded!\n");
	return 0;
}

static void __exit fortytwo_exit(void)
{
	debugfs_remove(foo);
	debugfs_remove(jiffies_whisperer);
	debugfs_remove(id);
	debugfs_remove(directory);
	pr_info("fortytwo dewbugfs successfully removed.\n");
}

module_init(fortytwo_init);
module_exit(fortytwo_exit);

MODULE_LICENSE("GPL");
