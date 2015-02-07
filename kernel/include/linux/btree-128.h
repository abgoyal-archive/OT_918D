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

extern struct btree_geo btree_geo128;

struct btree_head128 { struct btree_head h; };

static inline void btree_init_mempool128(struct btree_head128 *head,
					 mempool_t *mempool)
{
	btree_init_mempool(&head->h, mempool);
}

static inline int btree_init128(struct btree_head128 *head)
{
	return btree_init(&head->h);
}

static inline void btree_destroy128(struct btree_head128 *head)
{
	btree_destroy(&head->h);
}

static inline void *btree_lookup128(struct btree_head128 *head, u64 k1, u64 k2)
{
	u64 key[2] = {k1, k2};
	return btree_lookup(&head->h, &btree_geo128, (unsigned long *)&key);
}

static inline void *btree_get_prev128(struct btree_head128 *head,
				      u64 *k1, u64 *k2)
{
	u64 key[2] = {*k1, *k2};
	void *val;

	val = btree_get_prev(&head->h, &btree_geo128,
			     (unsigned long *)&key);
	*k1 = key[0];
	*k2 = key[1];
	return val;
}

static inline int btree_insert128(struct btree_head128 *head, u64 k1, u64 k2,
				  void *val, gfp_t gfp)
{
	u64 key[2] = {k1, k2};
	return btree_insert(&head->h, &btree_geo128,
			    (unsigned long *)&key, val, gfp);
}

static inline int btree_update128(struct btree_head128 *head, u64 k1, u64 k2,
				  void *val)
{
	u64 key[2] = {k1, k2};
	return btree_update(&head->h, &btree_geo128,
			    (unsigned long *)&key, val);
}

static inline void *btree_remove128(struct btree_head128 *head, u64 k1, u64 k2)
{
	u64 key[2] = {k1, k2};
	return btree_remove(&head->h, &btree_geo128, (unsigned long *)&key);
}

static inline void *btree_last128(struct btree_head128 *head, u64 *k1, u64 *k2)
{
	u64 key[2];
	void *val;

	val = btree_last(&head->h, &btree_geo128, (unsigned long *)&key[0]);
	if (val) {
		*k1 = key[0];
		*k2 = key[1];
	}

	return val;
}

static inline int btree_merge128(struct btree_head128 *target,
				 struct btree_head128 *victim,
				 gfp_t gfp)
{
	return btree_merge(&target->h, &victim->h, &btree_geo128, gfp);
}

void visitor128(void *elem, unsigned long opaque, unsigned long *__key,
		size_t index, void *__func);

typedef void (*visitor128_t)(void *elem, unsigned long opaque,
			     u64 key1, u64 key2, size_t index);

static inline size_t btree_visitor128(struct btree_head128 *head,
				      unsigned long opaque,
				      visitor128_t func2)
{
	return btree_visitor(&head->h, &btree_geo128, opaque,
			     visitor128, func2);
}

static inline size_t btree_grim_visitor128(struct btree_head128 *head,
					   unsigned long opaque,
					   visitor128_t func2)
{
	return btree_grim_visitor(&head->h, &btree_geo128, opaque,
				  visitor128, func2);
}

#define btree_for_each_safe128(head, k1, k2, val)	\
	for (val = btree_last128(head, &k1, &k2);	\
	     val;					\
	     val = btree_get_prev128(head, &k1, &k2))

