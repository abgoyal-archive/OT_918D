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
 * linux/fs/ext3/xattr_security.c
 * Handler for storing security labels as extended attributes.
 */

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/ext3_jbd.h>
#include <linux/ext3_fs.h>
#include <linux/security.h>
#include "xattr.h"

static size_t
ext3_xattr_security_list(struct dentry *dentry, char *list, size_t list_size,
			 const char *name, size_t name_len, int type)
{
	const size_t prefix_len = XATTR_SECURITY_PREFIX_LEN;
	const size_t total_len = prefix_len + name_len + 1;


	if (list && total_len <= list_size) {
		memcpy(list, XATTR_SECURITY_PREFIX, prefix_len);
		memcpy(list+prefix_len, name, name_len);
		list[prefix_len + name_len] = '\0';
	}
	return total_len;
}

static int
ext3_xattr_security_get(struct dentry *dentry, const char *name,
		void *buffer, size_t size, int type)
{
	if (strcmp(name, "") == 0)
		return -EINVAL;
	return ext3_xattr_get(dentry->d_inode, EXT3_XATTR_INDEX_SECURITY,
			      name, buffer, size);
}

static int
ext3_xattr_security_set(struct dentry *dentry, const char *name,
		const void *value, size_t size, int flags, int type)
{
	if (strcmp(name, "") == 0)
		return -EINVAL;
	return ext3_xattr_set(dentry->d_inode, EXT3_XATTR_INDEX_SECURITY,
			      name, value, size, flags);
}

int
ext3_init_security(handle_t *handle, struct inode *inode, struct inode *dir)
{
	int err;
	size_t len;
	void *value;
	char *name;

	err = security_inode_init_security(inode, dir, &name, &value, &len);
	if (err) {
		if (err == -EOPNOTSUPP)
			return 0;
		return err;
	}
	err = ext3_xattr_set_handle(handle, inode, EXT3_XATTR_INDEX_SECURITY,
				    name, value, len, 0);
	kfree(name);
	kfree(value);
	return err;
}

const struct xattr_handler ext3_xattr_security_handler = {
	.prefix	= XATTR_SECURITY_PREFIX,
	.list	= ext3_xattr_security_list,
	.get	= ext3_xattr_security_get,
	.set	= ext3_xattr_security_set,
};
