// SPDX-License-Identifier: GPL-2.0
/*
 * mymounts - A module that can list current mountpoints.
 *	- rgilles@student.42.fr
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/fs.h>
#include <linux/mount.h>
#include <linux/nsproxy.h>
#include <linux/ns_common.h>
#include <linux/proc_fs.h>

struct mnt_namespace {
	struct ns_common	ns;
	struct mount *root;
	/*
	 * Traversal and modification of .list is protected by either
	 * - taking namespace_sem for write, OR
	 * - taking namespace_sem for read AND taking .ns_lock.
	 */
	struct list_head	list;
	spinlock_t		ns_lock;
	struct user_namespace	*user_ns;
	struct ucounts		*ucounts;
	u64			seq;	/* Sequence number to prevent loops */
	wait_queue_head_t poll;
	u64 event;
	unsigned int		mounts; /* # of mounts in the namespace */
	unsigned int		pending_mounts;
} __randomize_layout;

struct mount {
	struct hlist_node mnt_hash;
	struct mount *mnt_parent;
	struct dentry *mnt_mountpoint;
	struct vfsmount mnt;
	union {
		struct rcu_head mnt_rcu;
		struct llist_node mnt_llist;
	};
#ifdef CONFIG_SMP
	struct mnt_pcp __percpu *mnt_pcp;
#else
	int mnt_count;
	int mnt_writers;
#endif
	struct list_head mnt_mounts;	/* list of children, anchored here */
	struct list_head mnt_child;	/* and going through their mnt_child */
	struct list_head mnt_instance;	/* mount instance on sb->s_mounts */
	const char *mnt_devname;	/* Name of device e.g. /dev/dsk/hda1 */
	struct list_head mnt_list;
	struct list_head mnt_expire;	/* link in fs-specific expiry list */
	struct list_head mnt_share;	/* circular list of shared mounts */
	struct list_head mnt_slave_list;/* list of slave mounts */
	struct list_head mnt_slave;	/* slave list entry */
	struct mount *mnt_master;	/* slave is on master->mnt_slave_list */
	struct mnt_namespace *mnt_ns;	/* containing namespace */
	struct mountpoint *mnt_mp;	/* where is it mounted */
	union {
		struct hlist_node mnt_mp_list;	/* list mounts with the same mountpoint */
		struct hlist_node mnt_umount;
	};
	struct list_head mnt_umounting; /* list entry for umount propagation */
#ifdef CONFIG_FSNOTIFY
	struct fsnotify_mark_connector __rcu *mnt_fsnotify_marks;
	__u32 mnt_fsnotify_mask;
#endif
	int mnt_id;			/* mount identifier */
	int mnt_group_id;		/* peer group identifier */
	int mnt_expiry_mark;		/* true if marked for expiry */
	struct hlist_head mnt_pins;
	struct hlist_head mnt_stuck_children;
} __randomize_layout;

struct proc_dir_entry *mymounts;

static char *compose_mountpoints_data(ssize_t *data_len)
{
	struct mount			*mnt;
	struct mnt_namespace	*ns = current->nsproxy->mnt_ns;
	size_t					field_len;
	char					mount_path_buf[PATH_MAX + 1];
	char					*mount_path;
	char					*tmp;
	char					*concatenation;

	concatenation = NULL;
	list_for_each_entry(mnt, &ns->list, mnt_list) {
		field_len = strlen(mnt->mnt_devname);
		tmp = krealloc(concatenation, *data_len + field_len + 1, GFP_KERNEL);
		if (!tmp) {
			kfree(concatenation);
			return tmp;
		}
		concatenation = tmp;
		memcpy(concatenation + *data_len, mnt->mnt_devname, field_len);
		concatenation[*data_len + field_len] = '\t';
		*data_len += ++field_len;

		mount_path = dentry_path_raw(mnt->mnt_mountpoint, mount_path_buf, PATH_MAX + 1);
		field_len = strlen(mount_path);
		tmp = krealloc(concatenation, *data_len + field_len + 1, GFP_KERNEL);
		if (!tmp) {
			kfree(concatenation);
			return tmp;
		}
		concatenation = tmp;
		memcpy(concatenation + *data_len, mount_path, field_len);
		concatenation[*data_len + field_len] = '\n';
		*data_len += ++field_len;
	}
	return concatenation;
}

static ssize_t mymounts_read(struct file *file, char __user *buf, size_t count, loff_t *f_pos)
{
	ssize_t ret;
	char *data;
	ssize_t data_len;

	data_len = 0;
	data = compose_mountpoints_data(&data_len);
	if (!data)
		return -ENOMEM;
	ret = simple_read_from_buffer(buf, count, f_pos, data, data_len);
	kfree(data);
	return ret;
}

const struct proc_ops mymounts_fops = {
	.proc_read			= mymounts_read,
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
