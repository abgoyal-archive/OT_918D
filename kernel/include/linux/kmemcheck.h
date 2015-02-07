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

#ifndef LINUX_KMEMCHECK_H
#define LINUX_KMEMCHECK_H

#include <linux/mm_types.h>
#include <linux/types.h>

#ifdef CONFIG_KMEMCHECK
extern int kmemcheck_enabled;

/* The slab-related functions. */
void kmemcheck_alloc_shadow(struct page *page, int order, gfp_t flags, int node);
void kmemcheck_free_shadow(struct page *page, int order);
void kmemcheck_slab_alloc(struct kmem_cache *s, gfp_t gfpflags, void *object,
			  size_t size);
void kmemcheck_slab_free(struct kmem_cache *s, void *object, size_t size);

void kmemcheck_pagealloc_alloc(struct page *p, unsigned int order,
			       gfp_t gfpflags);

void kmemcheck_show_pages(struct page *p, unsigned int n);
void kmemcheck_hide_pages(struct page *p, unsigned int n);

bool kmemcheck_page_is_tracked(struct page *p);

void kmemcheck_mark_unallocated(void *address, unsigned int n);
void kmemcheck_mark_uninitialized(void *address, unsigned int n);
void kmemcheck_mark_initialized(void *address, unsigned int n);
void kmemcheck_mark_freed(void *address, unsigned int n);

void kmemcheck_mark_unallocated_pages(struct page *p, unsigned int n);
void kmemcheck_mark_uninitialized_pages(struct page *p, unsigned int n);
void kmemcheck_mark_initialized_pages(struct page *p, unsigned int n);

int kmemcheck_show_addr(unsigned long address);
int kmemcheck_hide_addr(unsigned long address);

bool kmemcheck_is_obj_initialized(unsigned long addr, size_t size);

/*
 * Bitfield annotations
 *
 * How to use: If you have a struct using bitfields, for example
 *
 *     struct a {
 *             int x:8, y:8;
 *     };
 *
 * then this should be rewritten as
 *
 *     struct a {
 *             kmemcheck_bitfield_begin(flags);
 *             int x:8, y:8;
 *             kmemcheck_bitfield_end(flags);
 *     };
 *
 * Now the "flags_begin" and "flags_end" members may be used to refer to the
 * beginning and end, respectively, of the bitfield (and things like
 * &x.flags_begin is allowed). As soon as the struct is allocated, the bit-
 * fields should be annotated:
 *
 *     struct a *a = kmalloc(sizeof(struct a), GFP_KERNEL);
 *     kmemcheck_annotate_bitfield(a, flags);
 */
#define kmemcheck_bitfield_begin(name)	\
	int name##_begin[0];

#define kmemcheck_bitfield_end(name)	\
	int name##_end[0];

#define kmemcheck_annotate_bitfield(ptr, name)				\
	do {								\
		int _n;							\
									\
		if (!ptr)						\
			break;						\
									\
		_n = (long) &((ptr)->name##_end)			\
			- (long) &((ptr)->name##_begin);		\
		MAYBE_BUILD_BUG_ON(_n < 0);				\
									\
		kmemcheck_mark_initialized(&((ptr)->name##_begin), _n);	\
	} while (0)

#define kmemcheck_annotate_variable(var)				\
	do {								\
		kmemcheck_mark_initialized(&(var), sizeof(var));	\
	} while (0)							\

#else
#define kmemcheck_enabled 0

static inline void
kmemcheck_alloc_shadow(struct page *page, int order, gfp_t flags, int node)
{
}

static inline void
kmemcheck_free_shadow(struct page *page, int order)
{
}

static inline void
kmemcheck_slab_alloc(struct kmem_cache *s, gfp_t gfpflags, void *object,
		     size_t size)
{
}

static inline void kmemcheck_slab_free(struct kmem_cache *s, void *object,
				       size_t size)
{
}

static inline void kmemcheck_pagealloc_alloc(struct page *p,
	unsigned int order, gfp_t gfpflags)
{
}

static inline bool kmemcheck_page_is_tracked(struct page *p)
{
	return false;
}

static inline void kmemcheck_mark_unallocated(void *address, unsigned int n)
{
}

static inline void kmemcheck_mark_uninitialized(void *address, unsigned int n)
{
}

static inline void kmemcheck_mark_initialized(void *address, unsigned int n)
{
}

static inline void kmemcheck_mark_freed(void *address, unsigned int n)
{
}

static inline void kmemcheck_mark_unallocated_pages(struct page *p,
						    unsigned int n)
{
}

static inline void kmemcheck_mark_uninitialized_pages(struct page *p,
						      unsigned int n)
{
}

static inline void kmemcheck_mark_initialized_pages(struct page *p,
						    unsigned int n)
{
}

static inline bool kmemcheck_is_obj_initialized(unsigned long addr, size_t size)
{
	return true;
}

#define kmemcheck_bitfield_begin(name)
#define kmemcheck_bitfield_end(name)
#define kmemcheck_annotate_bitfield(ptr, name)	\
	do {					\
	} while (0)

#define kmemcheck_annotate_variable(var)	\
	do {					\
	} while (0)

#endif /* CONFIG_KMEMCHECK */

#endif /* LINUX_KMEMCHECK_H */
