// SPDX-License-Identifier: GPL-2.0
/*
 * hello_world - A sample kernel module.
 *	- rgilles@student.42.fr
 */

#include <linux/module.h>
#include <linux/printk.h>

int init_module(void)
{
		printk(KERN_INFO "Hello world !\n");
		return 0;
}

void cleanup_module(void)
{
		printk(KERN_INFO "Cleaning up module.\n");
}
MODULE_LICENSE("GPL");
