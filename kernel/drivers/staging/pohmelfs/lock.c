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
 * 2007+ Copyright (c) Evgeniy Polyakov <zbr@ioremap.net>
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/module.h>
#include <linux/backing-dev.h>
#include <linux/fs.h>
#include <linux/fsnotify.h>
#include <linux/mempool.h>

#include "netfs.h"

static int pohmelfs_send_lock_trans(struct pohmelfs_inode *pi,
		u64 id, u64 start, u32 size, int type)
{
	struct inode *inode = &pi->vfs_inode;
	struct pohmelfs_sb *psb = POHMELFS_SB(inode->i_sb);
	struct netfs_trans *t;
	struct netfs_cmd *cmd;
	int path_len, err;
	void *data;
	struct netfs_lock *l;
	int isize = (type & POHMELFS_LOCK_GRAB) ? 0 : sizeof(struct netfs_inode_info);

	err = pohmelfs_path_length(pi);
	if (err < 0)
		goto err_out_exit;

	path_len = err;

	err = -ENOMEM;
	t = netfs_trans_alloc(psb, path_len + sizeof(struct netfs_lock) + isize,
			NETFS_TRANS_SINGLE_DST, 0);
	if (!t)
		goto err_out_exit;

	cmd = netfs_trans_current(t);
	data = cmd + 1;

	err = pohmelfs_construct_path_string(pi, data, path_len);
	if (err < 0)
		goto err_out_free;
	path_len = err;

	l = data + path_len;

	l->start = start;
	l->size = size;
	l->type = type;
	l->ino = pi->ino;

	cmd->cmd = NETFS_LOCK;
	cmd->start = 0;
	cmd->id = id;
	cmd->size = sizeof(struct netfs_lock) + path_len + isize;
	cmd->ext = path_len;
	cmd->csize = 0;

	netfs_convert_cmd(cmd);
	netfs_convert_lock(l);

	if (isize) {
		struct netfs_inode_info *info = (struct netfs_inode_info *)(l + 1);

		info->mode = inode->i_mode;
		info->nlink = inode->i_nlink;
		info->uid = inode->i_uid;
		info->gid = inode->i_gid;
		info->blocks = inode->i_blocks;
		info->rdev = inode->i_rdev;
		info->size = inode->i_size;
		info->version = inode->i_version;

		netfs_convert_inode_info(info);
	}

	netfs_trans_update(cmd, t, path_len + sizeof(struct netfs_lock) + isize);

	return netfs_trans_finish(t, psb);

err_out_free:
	netfs_trans_free(t);
err_out_exit:
	printk("%s: err: %d.\n", __func__, err);
	return err;
}

int pohmelfs_data_lock(struct pohmelfs_inode *pi, u64 start, u32 size, int type)
{
	struct pohmelfs_sb *psb = POHMELFS_SB(pi->vfs_inode.i_sb);
	struct pohmelfs_mcache *m;
	int err = -ENOMEM;
	struct iattr iattr;
	struct inode *inode = &pi->vfs_inode;

	dprintk("%s: %p: ino: %llu, start: %llu, size: %u, "
			"type: %d, locked as: %d, owned: %d.\n",
			__func__, &pi->vfs_inode, pi->ino,
			start, size, type, pi->lock_type,
			!!test_bit(NETFS_INODE_OWNED, &pi->state));

	if (!pohmelfs_need_lock(pi, type))
		return 0;

	m = pohmelfs_mcache_alloc(psb, start, size, NULL);
	if (IS_ERR(m))
		return PTR_ERR(m);

	err = pohmelfs_send_lock_trans(pi, m->gen, start, size,
			type | POHMELFS_LOCK_GRAB);
	if (err)
		goto err_out_put;

	err = wait_for_completion_timeout(&m->complete, psb->mcache_timeout);
	if (err)
		err = m->err;
	else
		err = -ETIMEDOUT;

	if (err) {
		printk("%s: %p: ino: %llu, mgen: %llu, start: %llu, size: %u, err: %d.\n",
			__func__, &pi->vfs_inode, pi->ino, m->gen, start, size, err);
	}

	if (err && (err != -ENOENT))
		goto err_out_put;

	if (!err) {
		netfs_convert_inode_info(&m->info);

		iattr.ia_valid = ATTR_MODE | ATTR_UID | ATTR_GID | ATTR_SIZE | ATTR_ATIME;
		iattr.ia_mode = m->info.mode;
		iattr.ia_uid = m->info.uid;
		iattr.ia_gid = m->info.gid;
		iattr.ia_size = m->info.size;
		iattr.ia_atime = CURRENT_TIME;

		dprintk("%s: %p: ino: %llu, mgen: %llu, start: %llu, isize: %llu -> %llu.\n",
			__func__, &pi->vfs_inode, pi->ino, m->gen, start, inode->i_size, m->info.size);

		err = pohmelfs_setattr_raw(inode, &iattr);
		if (!err) {
			struct dentry *dentry = d_find_alias(inode);
			if (dentry) {
				fsnotify_change(dentry, iattr.ia_valid);
				dput(dentry);
			}
		}
	}

	pi->lock_type = type;
	set_bit(NETFS_INODE_OWNED, &pi->state);

	pohmelfs_mcache_put(psb, m);

	return 0;

err_out_put:
	pohmelfs_mcache_put(psb, m);
	return err;
}

int pohmelfs_data_unlock(struct pohmelfs_inode *pi, u64 start, u32 size, int type)
{
	dprintk("%s: %p: ino: %llu, start: %llu, size: %u, type: %d.\n",
			__func__, &pi->vfs_inode, pi->ino, start, size, type);
	pi->lock_type = 0;
	clear_bit(NETFS_INODE_REMOTE_DIR_SYNCED, &pi->state);
	clear_bit(NETFS_INODE_OWNED, &pi->state);
	return pohmelfs_send_lock_trans(pi, pi->ino, start, size, type);
}
