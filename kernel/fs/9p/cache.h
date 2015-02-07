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
 * V9FS cache definitions.
 *
 *  Copyright (C) 2009 by Abhishek Kulkarni <adkulkar@umail.iu.edu>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to:
 *  Free Software Foundation
 *  51 Franklin Street, Fifth Floor
 *  Boston, MA  02111-1301  USA
 *
 */

#ifndef _9P_CACHE_H
#ifdef CONFIG_9P_FSCACHE
#include <linux/fscache.h>
#include <linux/spinlock.h>

extern struct kmem_cache *vcookie_cache;

struct v9fs_cookie {
	spinlock_t lock;
	struct inode inode;
	struct fscache_cookie *fscache;
	struct p9_qid *qid;
};

static inline struct v9fs_cookie *v9fs_inode2cookie(const struct inode *inode)
{
	return container_of(inode, struct v9fs_cookie, inode);
}

extern struct fscache_netfs v9fs_cache_netfs;
extern const struct fscache_cookie_def v9fs_cache_session_index_def;
extern const struct fscache_cookie_def v9fs_cache_inode_index_def;

extern void v9fs_cache_session_get_cookie(struct v9fs_session_info *v9ses);
extern void v9fs_cache_session_put_cookie(struct v9fs_session_info *v9ses);

extern void v9fs_cache_inode_get_cookie(struct inode *inode);
extern void v9fs_cache_inode_put_cookie(struct inode *inode);
extern void v9fs_cache_inode_flush_cookie(struct inode *inode);
extern void v9fs_cache_inode_set_cookie(struct inode *inode, struct file *filp);
extern void v9fs_cache_inode_reset_cookie(struct inode *inode);

extern int __v9fs_cache_register(void);
extern void __v9fs_cache_unregister(void);

extern int __v9fs_fscache_release_page(struct page *page, gfp_t gfp);
extern void __v9fs_fscache_invalidate_page(struct page *page);
extern int __v9fs_readpage_from_fscache(struct inode *inode,
					struct page *page);
extern int __v9fs_readpages_from_fscache(struct inode *inode,
					 struct address_space *mapping,
					 struct list_head *pages,
					 unsigned *nr_pages);
extern void __v9fs_readpage_to_fscache(struct inode *inode, struct page *page);


/**
 * v9fs_cache_register - Register v9fs file system with the cache
 */
static inline int v9fs_cache_register(void)
{
	return __v9fs_cache_register();
}

/**
 * v9fs_cache_unregister - Unregister v9fs from the cache
 */
static inline void v9fs_cache_unregister(void)
{
	__v9fs_cache_unregister();
}

static inline int v9fs_fscache_release_page(struct page *page,
					    gfp_t gfp)
{
	return __v9fs_fscache_release_page(page, gfp);
}

static inline void v9fs_fscache_invalidate_page(struct page *page)
{
	__v9fs_fscache_invalidate_page(page);
}

static inline int v9fs_readpage_from_fscache(struct inode *inode,
					     struct page *page)
{
	return __v9fs_readpage_from_fscache(inode, page);
}

static inline int v9fs_readpages_from_fscache(struct inode *inode,
					      struct address_space *mapping,
					      struct list_head *pages,
					      unsigned *nr_pages)
{
	return __v9fs_readpages_from_fscache(inode, mapping, pages,
					     nr_pages);
}

static inline void v9fs_readpage_to_fscache(struct inode *inode,
					    struct page *page)
{
	if (PageFsCache(page))
		__v9fs_readpage_to_fscache(inode, page);
}

static inline void v9fs_uncache_page(struct inode *inode, struct page *page)
{
	struct v9fs_cookie *vcookie = v9fs_inode2cookie(inode);
	fscache_uncache_page(vcookie->fscache, page);
	BUG_ON(PageFsCache(page));
}

static inline void v9fs_vcookie_set_qid(struct inode *inode,
					struct p9_qid *qid)
{
	struct v9fs_cookie *vcookie = v9fs_inode2cookie(inode);
	spin_lock(&vcookie->lock);
	vcookie->qid = qid;
	spin_unlock(&vcookie->lock);
}

#else /* CONFIG_9P_FSCACHE */

static inline int v9fs_cache_register(void)
{
	return 1;
}

static inline void v9fs_cache_unregister(void) {}

static inline int v9fs_fscache_release_page(struct page *page,
					    gfp_t gfp) {
	return 1;
}

static inline void v9fs_fscache_invalidate_page(struct page *page) {}

static inline int v9fs_readpage_from_fscache(struct inode *inode,
					     struct page *page)
{
	return -ENOBUFS;
}

static inline int v9fs_readpages_from_fscache(struct inode *inode,
					      struct address_space *mapping,
					      struct list_head *pages,
					      unsigned *nr_pages)
{
	return -ENOBUFS;
}

static inline void v9fs_readpage_to_fscache(struct inode *inode,
					    struct page *page)
{}

static inline void v9fs_uncache_page(struct inode *inode, struct page *page)
{}

static inline void v9fs_vcookie_set_qid(struct inode *inode,
					struct p9_qid *qid)
{}

#endif /* CONFIG_9P_FSCACHE */
#endif /* _9P_CACHE_H */
