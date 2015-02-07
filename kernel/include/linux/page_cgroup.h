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

#ifndef __LINUX_PAGE_CGROUP_H
#define __LINUX_PAGE_CGROUP_H

#ifdef CONFIG_CGROUP_MEM_RES_CTLR
#include <linux/bit_spinlock.h>
/*
 * Page Cgroup can be considered as an extended mem_map.
 * A page_cgroup page is associated with every page descriptor. The
 * page_cgroup helps us identify information about the cgroup
 * All page cgroups are allocated at boot or memory hotplug event,
 * then the page cgroup for pfn always exists.
 */
struct page_cgroup {
	unsigned long flags;
	struct mem_cgroup *mem_cgroup;
	struct page *page;
	struct list_head lru;		/* per cgroup LRU list */
};

void __meminit pgdat_page_cgroup_init(struct pglist_data *pgdat);

#ifdef CONFIG_SPARSEMEM
static inline void __init page_cgroup_init_flatmem(void)
{
}
extern void __init page_cgroup_init(void);
#else
void __init page_cgroup_init_flatmem(void);
static inline void __init page_cgroup_init(void)
{
}
#endif

struct page_cgroup *lookup_page_cgroup(struct page *page);

enum {
	/* flags for mem_cgroup */
	PCG_LOCK,  /* page cgroup is locked */
	PCG_CACHE, /* charged as cache */
	PCG_USED, /* this object is in use. */
	PCG_ACCT_LRU, /* page has been accounted for */
	PCG_FILE_MAPPED, /* page is accounted as "mapped" */
	PCG_MIGRATION, /* under page migration */
};

#define TESTPCGFLAG(uname, lname)			\
static inline int PageCgroup##uname(struct page_cgroup *pc)	\
	{ return test_bit(PCG_##lname, &pc->flags); }

#define SETPCGFLAG(uname, lname)			\
static inline void SetPageCgroup##uname(struct page_cgroup *pc)\
	{ set_bit(PCG_##lname, &pc->flags);  }

#define CLEARPCGFLAG(uname, lname)			\
static inline void ClearPageCgroup##uname(struct page_cgroup *pc)	\
	{ clear_bit(PCG_##lname, &pc->flags);  }

#define TESTCLEARPCGFLAG(uname, lname)			\
static inline int TestClearPageCgroup##uname(struct page_cgroup *pc)	\
	{ return test_and_clear_bit(PCG_##lname, &pc->flags);  }

TESTPCGFLAG(Locked, LOCK)

/* Cache flag is set only once (at allocation) */
TESTPCGFLAG(Cache, CACHE)
CLEARPCGFLAG(Cache, CACHE)
SETPCGFLAG(Cache, CACHE)

TESTPCGFLAG(Used, USED)
CLEARPCGFLAG(Used, USED)
SETPCGFLAG(Used, USED)

SETPCGFLAG(AcctLRU, ACCT_LRU)
CLEARPCGFLAG(AcctLRU, ACCT_LRU)
TESTPCGFLAG(AcctLRU, ACCT_LRU)
TESTCLEARPCGFLAG(AcctLRU, ACCT_LRU)


SETPCGFLAG(FileMapped, FILE_MAPPED)
CLEARPCGFLAG(FileMapped, FILE_MAPPED)
TESTPCGFLAG(FileMapped, FILE_MAPPED)

SETPCGFLAG(Migration, MIGRATION)
CLEARPCGFLAG(Migration, MIGRATION)
TESTPCGFLAG(Migration, MIGRATION)

static inline int page_cgroup_nid(struct page_cgroup *pc)
{
	return page_to_nid(pc->page);
}

static inline enum zone_type page_cgroup_zid(struct page_cgroup *pc)
{
	return page_zonenum(pc->page);
}

static inline void lock_page_cgroup(struct page_cgroup *pc)
{
	bit_spin_lock(PCG_LOCK, &pc->flags);
}

static inline void unlock_page_cgroup(struct page_cgroup *pc)
{
	bit_spin_unlock(PCG_LOCK, &pc->flags);
}

#else /* CONFIG_CGROUP_MEM_RES_CTLR */
struct page_cgroup;

static inline void __meminit pgdat_page_cgroup_init(struct pglist_data *pgdat)
{
}

static inline struct page_cgroup *lookup_page_cgroup(struct page *page)
{
	return NULL;
}

static inline void page_cgroup_init(void)
{
}

static inline void __init page_cgroup_init_flatmem(void)
{
}

#endif

#include <linux/swap.h>

#ifdef CONFIG_CGROUP_MEM_RES_CTLR_SWAP
extern unsigned short swap_cgroup_cmpxchg(swp_entry_t ent,
					unsigned short old, unsigned short new);
extern unsigned short swap_cgroup_record(swp_entry_t ent, unsigned short id);
extern unsigned short lookup_swap_cgroup(swp_entry_t ent);
extern int swap_cgroup_swapon(int type, unsigned long max_pages);
extern void swap_cgroup_swapoff(int type);
#else

static inline
unsigned short swap_cgroup_record(swp_entry_t ent, unsigned short id)
{
	return 0;
}

static inline
unsigned short lookup_swap_cgroup(swp_entry_t ent)
{
	return 0;
}

static inline int
swap_cgroup_swapon(int type, unsigned long max_pages)
{
	return 0;
}

static inline void swap_cgroup_swapoff(int type)
{
	return;
}

#endif
#endif