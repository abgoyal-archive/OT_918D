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

/* CacheFiles security management
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */

#include <linux/fs.h>
#include <linux/cred.h>
#include "internal.h"

/*
 * determine the security context within which we access the cache from within
 * the kernel
 */
int cachefiles_get_security_ID(struct cachefiles_cache *cache)
{
	struct cred *new;
	int ret;

	_enter("{%s}", cache->secctx);

	new = prepare_kernel_cred(current);
	if (!new) {
		ret = -ENOMEM;
		goto error;
	}

	if (cache->secctx) {
		ret = set_security_override_from_ctx(new, cache->secctx);
		if (ret < 0) {
			put_cred(new);
			printk(KERN_ERR "CacheFiles:"
			       " Security denies permission to nominate"
			       " security context: error %d\n",
			       ret);
			goto error;
		}
	}

	cache->cache_cred = new;
	ret = 0;
error:
	_leave(" = %d", ret);
	return ret;
}

/*
 * see if mkdir and create can be performed in the root directory
 */
static int cachefiles_check_cache_dir(struct cachefiles_cache *cache,
				      struct dentry *root)
{
	int ret;

	ret = security_inode_mkdir(root->d_inode, root, 0);
	if (ret < 0) {
		printk(KERN_ERR "CacheFiles:"
		       " Security denies permission to make dirs: error %d",
		       ret);
		return ret;
	}

	ret = security_inode_create(root->d_inode, root, 0);
	if (ret < 0)
		printk(KERN_ERR "CacheFiles:"
		       " Security denies permission to create files: error %d",
		       ret);

	return ret;
}

/*
 * check the security details of the on-disk cache
 * - must be called with security override in force
 * - must return with a security override in force - even in the case of an
 *   error
 */
int cachefiles_determine_cache_security(struct cachefiles_cache *cache,
					struct dentry *root,
					const struct cred **_saved_cred)
{
	struct cred *new;
	int ret;

	_enter("");

	/* duplicate the cache creds for COW (the override is currently in
	 * force, so we can use prepare_creds() to do this) */
	new = prepare_creds();
	if (!new)
		return -ENOMEM;

	cachefiles_end_secure(cache, *_saved_cred);

	/* use the cache root dir's security context as the basis with
	 * which create files */
	ret = set_create_files_as(new, root->d_inode);
	if (ret < 0) {
		abort_creds(new);
		cachefiles_begin_secure(cache, _saved_cred);
		_leave(" = %d [cfa]", ret);
		return ret;
	}

	put_cred(cache->cache_cred);
	cache->cache_cred = new;

	cachefiles_begin_secure(cache, _saved_cred);
	ret = cachefiles_check_cache_dir(cache, root);

	if (ret == -EOPNOTSUPP)
		ret = 0;
	_leave(" = %d", ret);
	return ret;
}
