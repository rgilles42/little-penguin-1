// SPDX-License-Identifier: GPL-2.0
/*
 * mymounts - A module that can list current mountpoints.
 *	- rgilles@student.42.fr
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>

struct proc_dir_entry *mymounts;

static ssize_t mymounts_read(struct file *file, char __user *buf, size_t count, loff_t *f_pos)
{
	ssize_t ret;
	char *data;

	str = "PROUT";
	if (!str)
		return -ENOMEM;
	ret = simple_read_from_buffer(buf, count, off, str, 5);
	return ret; 
}

const struct proc_ops mymounts_fops = {
	.owner			= THIS_MODULE,
	.read			= mymounts_read,
};

static int __init fortytwo_init(void)
{
	mymounts = proc_create("mymounts", 0444, NULL, &mymounts_fops);
	if (!mymounts) {
		pr_err("mymounts loading failed!\n");
		return -1;
	}
	pr_info("mymounts successfully loaded in procfs!\n");
	return 0;
}

static void __exit fortytwo_exit(void)
{
	proc_remove(mymounts);
	pr_info("mymounts successfully removed from procfs.\n");
}

module_init(fortytwo_init);
module_exit(fortytwo_exit);

MODULE_LICENSE("GPL");
