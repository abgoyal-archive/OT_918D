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

/* -*- mode: c; c-basic-offset: 8; -*-
 * vim: noexpandtab sw=8 ts=8 sts=0:
 *
 *  linux/cluster/ssi/cfs/symlink.c
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License as
 *	published by the Free Software Foundation; either version 2 of
 *	the License, or (at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE
 *	or NON INFRINGEMENT.  See the GNU General Public License for more
 *	details.
 *
 * 	You should have received a copy of the GNU General Public License
 * 	along with this program; if not, write to the Free Software
 * 	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *	Questions/Comments/Bugfixes to ssic-linux-devel@lists.sourceforge.net
 *
 *  Copyright (C) 1992  Rick Sladkey
 *
 *  Optimization changes Copyright (C) 1994 Florian La Roche
 *
 *  Jun 7 1999, cache symlink lookups in the page cache.  -DaveM
 *
 *  Portions Copyright (C) 2001 Compaq Computer Corporation
 *
 *  ocfs2 symlink handling code.
 *
 *  Copyright (C) 2004, 2005 Oracle.
 *
 */

#include <linux/fs.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/pagemap.h>
#include <linux/namei.h>

#define MLOG_MASK_PREFIX ML_NAMEI
#include <cluster/masklog.h>

#include "ocfs2.h"

#include "alloc.h"
#include "file.h"
#include "inode.h"
#include "journal.h"
#include "symlink.h"
#include "xattr.h"

#include "buffer_head_io.h"


static char *ocfs2_fast_symlink_getlink(struct inode *inode,
					struct buffer_head **bh)
{
	int status;
	char *link = NULL;
	struct ocfs2_dinode *fe;

	mlog_entry_void();

	status = ocfs2_read_inode_block(inode, bh);
	if (status < 0) {
		mlog_errno(status);
		link = ERR_PTR(status);
		goto bail;
	}

	fe = (struct ocfs2_dinode *) (*bh)->b_data;
	link = (char *) fe->id2.i_symlink;
bail:
	mlog_exit(status);

	return link;
}

static int ocfs2_readlink(struct dentry *dentry,
			  char __user *buffer,
			  int buflen)
{
	int ret;
	char *link;
	struct buffer_head *bh = NULL;
	struct inode *inode = dentry->d_inode;

	mlog_entry_void();

	link = ocfs2_fast_symlink_getlink(inode, &bh);
	if (IS_ERR(link)) {
		ret = PTR_ERR(link);
		goto out;
	}

	/*
	 * Without vfsmount we can't update atime now,
	 * but we will update atime here ultimately.
	 */
	ret = vfs_readlink(dentry, buffer, buflen, link);

	brelse(bh);
out:
	mlog_exit(ret);
	return ret;
}

static void *ocfs2_fast_follow_link(struct dentry *dentry,
				    struct nameidata *nd)
{
	int status = 0;
	int len;
	char *target, *link = ERR_PTR(-ENOMEM);
	struct inode *inode = dentry->d_inode;
	struct buffer_head *bh = NULL;

	mlog_entry_void();

	BUG_ON(!ocfs2_inode_is_fast_symlink(inode));
	target = ocfs2_fast_symlink_getlink(inode, &bh);
	if (IS_ERR(target)) {
		status = PTR_ERR(target);
		mlog_errno(status);
		goto bail;
	}

	/* Fast symlinks can't be large */
	len = strlen(target);
	link = kzalloc(len + 1, GFP_NOFS);
	if (!link) {
		status = -ENOMEM;
		mlog_errno(status);
		goto bail;
	}

	memcpy(link, target, len);

bail:
	nd_set_link(nd, status ? ERR_PTR(status) : link);
	brelse(bh);

	mlog_exit(status);
	return NULL;
}

static void ocfs2_fast_put_link(struct dentry *dentry, struct nameidata *nd, void *cookie)
{
	char *link = nd_get_link(nd);
	if (!IS_ERR(link))
		kfree(link);
}

const struct inode_operations ocfs2_symlink_inode_operations = {
	.readlink	= page_readlink,
	.follow_link	= page_follow_link_light,
	.put_link	= page_put_link,
	.getattr	= ocfs2_getattr,
	.setattr	= ocfs2_setattr,
	.setxattr	= generic_setxattr,
	.getxattr	= generic_getxattr,
	.listxattr	= ocfs2_listxattr,
	.removexattr	= generic_removexattr,
	.fiemap		= ocfs2_fiemap,
};
const struct inode_operations ocfs2_fast_symlink_inode_operations = {
	.readlink	= ocfs2_readlink,
	.follow_link	= ocfs2_fast_follow_link,
	.put_link	= ocfs2_fast_put_link,
	.getattr	= ocfs2_getattr,
	.setattr	= ocfs2_setattr,
	.setxattr	= generic_setxattr,
	.getxattr	= generic_getxattr,
	.listxattr	= ocfs2_listxattr,
	.removexattr	= generic_removexattr,
	.fiemap		= ocfs2_fiemap,
};
