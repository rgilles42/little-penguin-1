/*
 * fortytwo.c - A sample misc device driver.	
 * 	- rgilles@student.42.fr
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

static ssize_t ft_misc_write(struct file* file, const char __user * buf, size_t len, loff_t* ppos) {
	char tmp[8];
	ssize_t l;

	l = simple_write_to_buffer(tmp, sizeof(tmp), ppos, buf, len);
	if (l == 7 && memcmp(tmp, "rgilles", l) == 0)
		return(len);
	else
		return(-EINVAL);
}

static ssize_t ft_misc_read(struct file* file, char __user * buf, size_t count, loff_t* f_pos) {
    return (simple_read_from_buffer(buf, count, f_pos, "rgilles", 7));
}

static const struct file_operations fops = {
    .owner          = THIS_MODULE,
    .write          = ft_misc_write,
    .read           = ft_misc_read,
};
struct miscdevice ft_misc_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "fortytwo",
    .fops = &fops,
};

static int __init fortytwo_init(void)
{
	int error;

	error = misc_register(&ft_misc_device);
	if (error) {
		pr_err("fortytwo misc device driver failed to load!\n");
		return(-1);
	}
	pr_info("fortytwo misc device driver succesfully loaded!\n");
	return(0);
}

static void __exit fortytwo_exit(void)
{
	misc_deregister(&ft_misc_device);
	pr_info("fortytwo misc device driver succesfully removed.\n");
}

module_init(fortytwo_init);
module_exit(fortytwo_exit);

MODULE_LICENSE("GPL");
