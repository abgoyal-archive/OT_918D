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

#ifndef _LINUX_RCULIST_NULLS_H
#define _LINUX_RCULIST_NULLS_H

#ifdef __KERNEL__

/*
 * RCU-protected list version
 */
#include <linux/list_nulls.h>
#include <linux/rcupdate.h>

/**
 * hlist_nulls_del_init_rcu - deletes entry from hash list with re-initialization
 * @n: the element to delete from the hash list.
 *
 * Note: hlist_nulls_unhashed() on the node return true after this. It is
 * useful for RCU based read lockfree traversal if the writer side
 * must know if the list entry is still hashed or already unhashed.
 *
 * In particular, it means that we can not poison the forward pointers
 * that may still be used for walking the hash list and we can only
 * zero the pprev pointer so list_unhashed() will return true after
 * this.
 *
 * The caller must take whatever precautions are necessary (such as
 * holding appropriate locks) to avoid racing with another
 * list-mutation primitive, such as hlist_nulls_add_head_rcu() or
 * hlist_nulls_del_rcu(), running on this same list.  However, it is
 * perfectly legal to run concurrently with the _rcu list-traversal
 * primitives, such as hlist_nulls_for_each_entry_rcu().
 */
static inline void hlist_nulls_del_init_rcu(struct hlist_nulls_node *n)
{
	if (!hlist_nulls_unhashed(n)) {
		__hlist_nulls_del(n);
		n->pprev = NULL;
	}
}

/**
 * hlist_nulls_del_rcu - deletes entry from hash list without re-initialization
 * @n: the element to delete from the hash list.
 *
 * Note: hlist_nulls_unhashed() on entry does not return true after this,
 * the entry is in an undefined state. It is useful for RCU based
 * lockfree traversal.
 *
 * In particular, it means that we can not poison the forward
 * pointers that may still be used for walking the hash list.
 *
 * The caller must take whatever precautions are necessary
 * (such as holding appropriate locks) to avoid racing
 * with another list-mutation primitive, such as hlist_nulls_add_head_rcu()
 * or hlist_nulls_del_rcu(), running on this same list.
 * However, it is perfectly legal to run concurrently with
 * the _rcu list-traversal primitives, such as
 * hlist_nulls_for_each_entry().
 */
static inline void hlist_nulls_del_rcu(struct hlist_nulls_node *n)
{
	__hlist_nulls_del(n);
	n->pprev = LIST_POISON2;
}

/**
 * hlist_nulls_add_head_rcu
 * @n: the element to add to the hash list.
 * @h: the list to add to.
 *
 * Description:
 * Adds the specified element to the specified hlist_nulls,
 * while permitting racing traversals.
 *
 * The caller must take whatever precautions are necessary
 * (such as holding appropriate locks) to avoid racing
 * with another list-mutation primitive, such as hlist_nulls_add_head_rcu()
 * or hlist_nulls_del_rcu(), running on this same list.
 * However, it is perfectly legal to run concurrently with
 * the _rcu list-traversal primitives, such as
 * hlist_nulls_for_each_entry_rcu(), used to prevent memory-consistency
 * problems on Alpha CPUs.  Regardless of the type of CPU, the
 * list-traversal primitive must be guarded by rcu_read_lock().
 */
static inline void hlist_nulls_add_head_rcu(struct hlist_nulls_node *n,
					struct hlist_nulls_head *h)
{
	struct hlist_nulls_node *first = h->first;

	n->next = first;
	n->pprev = &h->first;
	rcu_assign_pointer(h->first, n);
	if (!is_a_nulls(first))
		first->pprev = &n->next;
}
/**
 * hlist_nulls_for_each_entry_rcu - iterate over rcu list of given type
 * @tpos:	the type * to use as a loop cursor.
 * @pos:	the &struct hlist_nulls_node to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the hlist_nulls_node within the struct.
 *
 */
#define hlist_nulls_for_each_entry_rcu(tpos, pos, head, member) \
	for (pos = rcu_dereference_raw((head)->first);			 \
		(!is_a_nulls(pos)) &&			\
		({ tpos = hlist_nulls_entry(pos, typeof(*tpos), member); 1; }); \
		pos = rcu_dereference_raw(pos->next))

#endif
#endif
