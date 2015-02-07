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
 *  linux/fs/affs/dir.c
 *
 *  (c) 1996  Hans-Joachim Widmaier - Rewritten
 *
 *  (C) 1993  Ray Burr - Modified for Amiga FFS filesystem.
 *
 *  (C) 1992  Eric Youngdale Modified for ISO 9660 filesystem.
 *
 *  (C) 1991  Linus Torvalds - minix filesystem
 *
 *  affs directory handling functions
 *
 */

#include "affs.h"

static int affs_readdir(struct file *, void *, filldir_t);

const struct file_operations affs_dir_operations = {
	.read		= generic_read_dir,
	.llseek		= generic_file_llseek,
	.readdir	= affs_readdir,
	.fsync		= affs_file_fsync,
};

/*
 * directories can handle most operations...
 */
const struct inode_operations affs_dir_inode_operations = {
	.create		= affs_create,
	.lookup		= affs_lookup,
	.link		= affs_link,
	.unlink		= affs_unlink,
	.symlink	= affs_symlink,
	.mkdir		= affs_mkdir,
	.rmdir		= affs_rmdir,
	.rename		= affs_rename,
	.setattr	= affs_notify_change,
};

static int
affs_readdir(struct file *filp, void *dirent, filldir_t filldir)
{
	struct inode		*inode = filp->f_path.dentry->d_inode;
	struct super_block	*sb = inode->i_sb;
	struct buffer_head	*dir_bh;
	struct buffer_head	*fh_bh;
	unsigned char		*name;
	int			 namelen;
	u32			 i;
	int			 hash_pos;
	int			 chain_pos;
	u32			 f_pos;
	u32			 ino;
	int			 stored;
	int			 res;

	pr_debug("AFFS: readdir(ino=%lu,f_pos=%lx)\n",inode->i_ino,(unsigned long)filp->f_pos);

	stored = 0;
	res    = -EIO;
	dir_bh = NULL;
	fh_bh  = NULL;
	f_pos  = filp->f_pos;

	if (f_pos == 0) {
		filp->private_data = (void *)0;
		if (filldir(dirent, ".", 1, f_pos, inode->i_ino, DT_DIR) < 0)
			return 0;
		filp->f_pos = f_pos = 1;
		stored++;
	}
	if (f_pos == 1) {
		if (filldir(dirent, "..", 2, f_pos, parent_ino(filp->f_path.dentry), DT_DIR) < 0)
			return stored;
		filp->f_pos = f_pos = 2;
		stored++;
	}

	affs_lock_dir(inode);
	chain_pos = (f_pos - 2) & 0xffff;
	hash_pos  = (f_pos - 2) >> 16;
	if (chain_pos == 0xffff) {
		affs_warning(sb, "readdir", "More than 65535 entries in chain");
		chain_pos = 0;
		hash_pos++;
		filp->f_pos = ((hash_pos << 16) | chain_pos) + 2;
	}
	dir_bh = affs_bread(sb, inode->i_ino);
	if (!dir_bh)
		goto readdir_out;

	/* If the directory hasn't changed since the last call to readdir(),
	 * we can jump directly to where we left off.
	 */
	ino = (u32)(long)filp->private_data;
	if (ino && filp->f_version == inode->i_version) {
		pr_debug("AFFS: readdir() left off=%d\n", ino);
		goto inside;
	}

	ino = be32_to_cpu(AFFS_HEAD(dir_bh)->table[hash_pos]);
	for (i = 0; ino && i < chain_pos; i++) {
		fh_bh = affs_bread(sb, ino);
		if (!fh_bh) {
			affs_error(sb, "readdir","Cannot read block %d", i);
			goto readdir_out;
		}
		ino = be32_to_cpu(AFFS_TAIL(sb, fh_bh)->hash_chain);
		affs_brelse(fh_bh);
		fh_bh = NULL;
	}
	if (ino)
		goto inside;
	hash_pos++;

	for (; hash_pos < AFFS_SB(sb)->s_hashsize; hash_pos++) {
		ino = be32_to_cpu(AFFS_HEAD(dir_bh)->table[hash_pos]);
		if (!ino)
			continue;
		f_pos = (hash_pos << 16) + 2;
inside:
		do {
			fh_bh = affs_bread(sb, ino);
			if (!fh_bh) {
				affs_error(sb, "readdir","Cannot read block %d", ino);
				goto readdir_done;
			}

			namelen = min(AFFS_TAIL(sb, fh_bh)->name[0], (u8)30);
			name = AFFS_TAIL(sb, fh_bh)->name + 1;
			pr_debug("AFFS: readdir(): filldir(\"%.*s\", ino=%u), hash=%d, f_pos=%x\n",
				 namelen, name, ino, hash_pos, f_pos);
			if (filldir(dirent, name, namelen, f_pos, ino, DT_UNKNOWN) < 0)
				goto readdir_done;
			stored++;
			f_pos++;
			ino = be32_to_cpu(AFFS_TAIL(sb, fh_bh)->hash_chain);
			affs_brelse(fh_bh);
			fh_bh = NULL;
		} while (ino);
	}
readdir_done:
	filp->f_pos = f_pos;
	filp->f_version = inode->i_version;
	filp->private_data = (void *)(long)ino;
	res = stored;

readdir_out:
	affs_brelse(dir_bh);
	affs_brelse(fh_bh);
	affs_unlock_dir(inode);
	pr_debug("AFFS: readdir()=%d\n", stored);
	return res;
}
