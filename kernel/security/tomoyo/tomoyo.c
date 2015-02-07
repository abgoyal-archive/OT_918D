/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

/*
 * security/tomoyo/tomoyo.c
 *
 * LSM hooks for TOMOYO Linux.
 *
 * Copyright (C) 2005-2009  NTT DATA CORPORATION
 *
 * Version: 2.2.0   2009/04/01
 *
 */

#include <linux/security.h>
#include "common.h"

static int tomoyo_cred_alloc_blank(struct cred *new, gfp_t gfp)
{
	new->security = NULL;
	return 0;
}

static int tomoyo_cred_prepare(struct cred *new, const struct cred *old,
			       gfp_t gfp)
{
	struct tomoyo_domain_info *domain = old->security;
	new->security = domain;
	if (domain)
		atomic_inc(&domain->users);
	return 0;
}

static void tomoyo_cred_transfer(struct cred *new, const struct cred *old)
{
	tomoyo_cred_prepare(new, old, 0);
}

static void tomoyo_cred_free(struct cred *cred)
{
	struct tomoyo_domain_info *domain = cred->security;
	if (domain)
		atomic_dec(&domain->users);
}

static int tomoyo_bprm_set_creds(struct linux_binprm *bprm)
{
	int rc;

	rc = cap_bprm_set_creds(bprm);
	if (rc)
		return rc;

	/*
	 * Do only if this function is called for the first time of an execve
	 * operation.
	 */
	if (bprm->cred_prepared)
		return 0;
	/*
	 * Load policy if /sbin/tomoyo-init exists and /sbin/init is requested
	 * for the first time.
	 */
	if (!tomoyo_policy_loaded)
		tomoyo_load_policy(bprm->filename);
	/*
	 * Release reference to "struct tomoyo_domain_info" stored inside
	 * "bprm->cred->security". New reference to "struct tomoyo_domain_info"
	 * stored inside "bprm->cred->security" will be acquired later inside
	 * tomoyo_find_next_domain().
	 */
	atomic_dec(&((struct tomoyo_domain_info *)
		     bprm->cred->security)->users);
	/*
	 * Tell tomoyo_bprm_check_security() is called for the first time of an
	 * execve operation.
	 */
	bprm->cred->security = NULL;
	return 0;
}

static int tomoyo_bprm_check_security(struct linux_binprm *bprm)
{
	struct tomoyo_domain_info *domain = bprm->cred->security;

	/*
	 * Execute permission is checked against pathname passed to do_execve()
	 * using current domain.
	 */
	if (!domain) {
		const int idx = tomoyo_read_lock();
		const int err = tomoyo_find_next_domain(bprm);
		tomoyo_read_unlock(idx);
		return err;
	}
	/*
	 * Read permission is checked against interpreters using next domain.
	 */
	return tomoyo_check_open_permission(domain, &bprm->file->f_path, O_RDONLY);
}

static int tomoyo_path_truncate(struct path *path, loff_t length,
				unsigned int time_attrs)
{
	return tomoyo_path_perm(TOMOYO_TYPE_TRUNCATE, path);
}

static int tomoyo_path_unlink(struct path *parent, struct dentry *dentry)
{
	struct path path = { parent->mnt, dentry };
	return tomoyo_path_perm(TOMOYO_TYPE_UNLINK, &path);
}

static int tomoyo_path_mkdir(struct path *parent, struct dentry *dentry,
			     int mode)
{
	struct path path = { parent->mnt, dentry };
	return tomoyo_path_perm(TOMOYO_TYPE_MKDIR, &path);
}

static int tomoyo_path_rmdir(struct path *parent, struct dentry *dentry)
{
	struct path path = { parent->mnt, dentry };
	return tomoyo_path_perm(TOMOYO_TYPE_RMDIR, &path);
}

static int tomoyo_path_symlink(struct path *parent, struct dentry *dentry,
			       const char *old_name)
{
	struct path path = { parent->mnt, dentry };
	return tomoyo_path_perm(TOMOYO_TYPE_SYMLINK, &path);
}

static int tomoyo_path_mknod(struct path *parent, struct dentry *dentry,
			     int mode, unsigned int dev)
{
	struct path path = { parent->mnt, dentry };
	int type = TOMOYO_TYPE_CREATE;

	switch (mode & S_IFMT) {
	case S_IFCHR:
		type = TOMOYO_TYPE_MKCHAR;
		break;
	case S_IFBLK:
		type = TOMOYO_TYPE_MKBLOCK;
		break;
	case S_IFIFO:
		type = TOMOYO_TYPE_MKFIFO;
		break;
	case S_IFSOCK:
		type = TOMOYO_TYPE_MKSOCK;
		break;
	}
	return tomoyo_path_perm(type, &path);
}

static int tomoyo_path_link(struct dentry *old_dentry, struct path *new_dir,
			    struct dentry *new_dentry)
{
	struct path path1 = { new_dir->mnt, old_dentry };
	struct path path2 = { new_dir->mnt, new_dentry };
	return tomoyo_path2_perm(TOMOYO_TYPE_LINK, &path1, &path2);
}

static int tomoyo_path_rename(struct path *old_parent,
			      struct dentry *old_dentry,
			      struct path *new_parent,
			      struct dentry *new_dentry)
{
	struct path path1 = { old_parent->mnt, old_dentry };
	struct path path2 = { new_parent->mnt, new_dentry };
	return tomoyo_path2_perm(TOMOYO_TYPE_RENAME, &path1, &path2);
}

static int tomoyo_file_fcntl(struct file *file, unsigned int cmd,
			     unsigned long arg)
{
	if (cmd == F_SETFL && ((arg ^ file->f_flags) & O_APPEND))
		return tomoyo_check_rewrite_permission(file);
	return 0;
}

static int tomoyo_dentry_open(struct file *f, const struct cred *cred)
{
	int flags = f->f_flags;
	/* Don't check read permission here if called from do_execve(). */
	if (current->in_execve)
		return 0;
	return tomoyo_check_open_permission(tomoyo_domain(), &f->f_path, flags);
}

static int tomoyo_file_ioctl(struct file *file, unsigned int cmd,
			     unsigned long arg)
{
	return tomoyo_path_perm(TOMOYO_TYPE_IOCTL, &file->f_path);
}

static int tomoyo_path_chmod(struct dentry *dentry, struct vfsmount *mnt,
			     mode_t mode)
{
	struct path path = { mnt, dentry };
	return tomoyo_path_perm(TOMOYO_TYPE_CHMOD, &path);
}

static int tomoyo_path_chown(struct path *path, uid_t uid, gid_t gid)
{
	int error = 0;
	if (uid != (uid_t) -1)
		error = tomoyo_path_perm(TOMOYO_TYPE_CHOWN, path);
	if (!error && gid != (gid_t) -1)
		error = tomoyo_path_perm(TOMOYO_TYPE_CHGRP, path);
	return error;
}

static int tomoyo_path_chroot(struct path *path)
{
	return tomoyo_path_perm(TOMOYO_TYPE_CHROOT, path);
}

static int tomoyo_sb_mount(char *dev_name, struct path *path,
			   char *type, unsigned long flags, void *data)
{
	return tomoyo_path_perm(TOMOYO_TYPE_MOUNT, path);
}

static int tomoyo_sb_umount(struct vfsmount *mnt, int flags)
{
	struct path path = { mnt, mnt->mnt_root };
	return tomoyo_path_perm(TOMOYO_TYPE_UMOUNT, &path);
}

static int tomoyo_sb_pivotroot(struct path *old_path, struct path *new_path)
{
	return tomoyo_path2_perm(TOMOYO_TYPE_PIVOT_ROOT, new_path, old_path);
}

/*
 * tomoyo_security_ops is a "struct security_operations" which is used for
 * registering TOMOYO.
 */
static struct security_operations tomoyo_security_ops = {
	.name                = "tomoyo",
	.cred_alloc_blank    = tomoyo_cred_alloc_blank,
	.cred_prepare        = tomoyo_cred_prepare,
	.cred_transfer	     = tomoyo_cred_transfer,
	.cred_free           = tomoyo_cred_free,
	.bprm_set_creds      = tomoyo_bprm_set_creds,
	.bprm_check_security = tomoyo_bprm_check_security,
	.file_fcntl          = tomoyo_file_fcntl,
	.dentry_open         = tomoyo_dentry_open,
	.path_truncate       = tomoyo_path_truncate,
	.path_unlink         = tomoyo_path_unlink,
	.path_mkdir          = tomoyo_path_mkdir,
	.path_rmdir          = tomoyo_path_rmdir,
	.path_symlink        = tomoyo_path_symlink,
	.path_mknod          = tomoyo_path_mknod,
	.path_link           = tomoyo_path_link,
	.path_rename         = tomoyo_path_rename,
	.file_ioctl          = tomoyo_file_ioctl,
	.path_chmod          = tomoyo_path_chmod,
	.path_chown          = tomoyo_path_chown,
	.path_chroot         = tomoyo_path_chroot,
	.sb_mount            = tomoyo_sb_mount,
	.sb_umount           = tomoyo_sb_umount,
	.sb_pivotroot        = tomoyo_sb_pivotroot,
};

/* Lock for GC. */
struct srcu_struct tomoyo_ss;

static int __init tomoyo_init(void)
{
	struct cred *cred = (struct cred *) current_cred();

	if (!security_module_enable(&tomoyo_security_ops))
		return 0;
	/* register ourselves with the security framework */
	if (register_security(&tomoyo_security_ops) ||
	    init_srcu_struct(&tomoyo_ss))
		panic("Failure registering TOMOYO Linux");
	printk(KERN_INFO "TOMOYO Linux initialized\n");
	cred->security = &tomoyo_kernel_domain;
	tomoyo_realpath_init();
	return 0;
}

security_initcall(tomoyo_init);
