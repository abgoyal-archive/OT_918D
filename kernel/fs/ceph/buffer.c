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


#include "ceph_debug.h"

#include <linux/slab.h>

#include "buffer.h"
#include "decode.h"

struct ceph_buffer *ceph_buffer_new(size_t len, gfp_t gfp)
{
	struct ceph_buffer *b;

	b = kmalloc(sizeof(*b), gfp);
	if (!b)
		return NULL;

	b->vec.iov_base = kmalloc(len, gfp | __GFP_NOWARN);
	if (b->vec.iov_base) {
		b->is_vmalloc = false;
	} else {
		b->vec.iov_base = __vmalloc(len, gfp, PAGE_KERNEL);
		if (!b->vec.iov_base) {
			kfree(b);
			return NULL;
		}
		b->is_vmalloc = true;
	}

	kref_init(&b->kref);
	b->alloc_len = len;
	b->vec.iov_len = len;
	dout("buffer_new %p\n", b);
	return b;
}

void ceph_buffer_release(struct kref *kref)
{
	struct ceph_buffer *b = container_of(kref, struct ceph_buffer, kref);

	dout("buffer_release %p\n", b);
	if (b->vec.iov_base) {
		if (b->is_vmalloc)
			vfree(b->vec.iov_base);
		else
			kfree(b->vec.iov_base);
	}
	kfree(b);
}

int ceph_buffer_alloc(struct ceph_buffer *b, int len, gfp_t gfp)
{
	b->vec.iov_base = kmalloc(len, gfp | __GFP_NOWARN);
	if (b->vec.iov_base) {
		b->is_vmalloc = false;
	} else {
		b->vec.iov_base = __vmalloc(len, gfp, PAGE_KERNEL);
		b->is_vmalloc = true;
	}
	if (!b->vec.iov_base)
		return -ENOMEM;
	b->alloc_len = len;
	b->vec.iov_len = len;
	return 0;
}

int ceph_decode_buffer(struct ceph_buffer **b, void **p, void *end)
{
	size_t len;

	ceph_decode_need(p, end, sizeof(u32), bad);
	len = ceph_decode_32(p);
	dout("decode_buffer len %d\n", (int)len);
	ceph_decode_need(p, end, len, bad);
	*b = ceph_buffer_new(len, GFP_NOFS);
	if (!*b)
		return -ENOMEM;
	ceph_decode_copy(p, (*b)->vec.iov_base, len);
	return 0;
bad:
	return -EINVAL;
}
