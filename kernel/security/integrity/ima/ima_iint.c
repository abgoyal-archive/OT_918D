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
 * Copyright (C) 2008 IBM Corporation
 *
 * Authors:
 * Mimi Zohar <zohar@us.ibm.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2 of the
 * License.
 *
 * File: ima_iint.c
 * 	- implements the IMA hooks: ima_inode_alloc, ima_inode_free
 *	- cache integrity information associated with an inode
 *	  using a radix tree.
 */
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/radix-tree.h>
#include "ima.h"

RADIX_TREE(ima_iint_store, GFP_ATOMIC);
DEFINE_SPINLOCK(ima_iint_lock);
static struct kmem_cache *iint_cache __read_mostly;

int iint_initialized = 0;

/* ima_iint_find_get - return the iint associated with an inode
 *
 * ima_iint_find_get gets a reference to the iint. Caller must
 * remember to put the iint reference.
 */
struct ima_iint_cache *ima_iint_find_get(struct inode *inode)
{
	struct ima_iint_cache *iint;

	rcu_read_lock();
	iint = radix_tree_lookup(&ima_iint_store, (unsigned long)inode);
	if (!iint)
		goto out;
	kref_get(&iint->refcount);
out:
	rcu_read_unlock();
	return iint;
}

/**
 * ima_inode_alloc - allocate an iint associated with an inode
 * @inode: pointer to the inode
 */
int ima_inode_alloc(struct inode *inode)
{
	struct ima_iint_cache *iint = NULL;
	int rc = 0;

	iint = kmem_cache_alloc(iint_cache, GFP_NOFS);
	if (!iint)
		return -ENOMEM;

	rc = radix_tree_preload(GFP_NOFS);
	if (rc < 0)
		goto out;

	spin_lock(&ima_iint_lock);
	rc = radix_tree_insert(&ima_iint_store, (unsigned long)inode, iint);
	spin_unlock(&ima_iint_lock);
	radix_tree_preload_end();
out:
	if (rc < 0)
		kmem_cache_free(iint_cache, iint);

	return rc;
}

/* iint_free - called when the iint refcount goes to zero */
void iint_free(struct kref *kref)
{
	struct ima_iint_cache *iint = container_of(kref, struct ima_iint_cache,
						   refcount);
	iint->version = 0;
	iint->flags = 0UL;
	if (iint->readcount != 0) {
		printk(KERN_INFO "%s: readcount: %ld\n", __func__,
		       iint->readcount);
		iint->readcount = 0;
	}
	if (iint->writecount != 0) {
		printk(KERN_INFO "%s: writecount: %ld\n", __func__,
		       iint->writecount);
		iint->writecount = 0;
	}
	if (iint->opencount != 0) {
		printk(KERN_INFO "%s: opencount: %ld\n", __func__,
		       iint->opencount);
		iint->opencount = 0;
	}
	kref_init(&iint->refcount);
	kmem_cache_free(iint_cache, iint);
}

void iint_rcu_free(struct rcu_head *rcu_head)
{
	struct ima_iint_cache *iint = container_of(rcu_head,
						   struct ima_iint_cache, rcu);
	kref_put(&iint->refcount, iint_free);
}

/**
 * ima_inode_free - called on security_inode_free
 * @inode: pointer to the inode
 *
 * Free the integrity information(iint) associated with an inode.
 */
void ima_inode_free(struct inode *inode)
{
	struct ima_iint_cache *iint;

	spin_lock(&ima_iint_lock);
	iint = radix_tree_delete(&ima_iint_store, (unsigned long)inode);
	spin_unlock(&ima_iint_lock);
	if (iint)
		call_rcu(&iint->rcu, iint_rcu_free);
}

static void init_once(void *foo)
{
	struct ima_iint_cache *iint = foo;

	memset(iint, 0, sizeof *iint);
	iint->version = 0;
	iint->flags = 0UL;
	mutex_init(&iint->mutex);
	iint->readcount = 0;
	iint->writecount = 0;
	iint->opencount = 0;
	kref_init(&iint->refcount);
}

static int __init ima_iintcache_init(void)
{
	iint_cache =
	    kmem_cache_create("iint_cache", sizeof(struct ima_iint_cache), 0,
			      SLAB_PANIC, init_once);
	iint_initialized = 1;
	return 0;
}
security_initcall(ima_iintcache_init);
