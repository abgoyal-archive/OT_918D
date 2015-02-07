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

#ifndef __PERF_STRLIST_H
#define __PERF_STRLIST_H

#include <linux/rbtree.h>
#include <stdbool.h>

struct str_node {
	struct rb_node rb_node;
	const char     *s;
};

struct strlist {
	struct rb_root entries;
	unsigned int   nr_entries;
	bool	       dupstr;
};

struct strlist *strlist__new(bool dupstr, const char *slist);
void strlist__delete(struct strlist *self);

void strlist__remove(struct strlist *self, struct str_node *sn);
int strlist__load(struct strlist *self, const char *filename);
int strlist__add(struct strlist *self, const char *str);

struct str_node *strlist__entry(const struct strlist *self, unsigned int idx);
struct str_node *strlist__find(struct strlist *self, const char *entry);

static inline bool strlist__has_entry(struct strlist *self, const char *entry)
{
	return strlist__find(self, entry) != NULL;
}

static inline bool strlist__empty(const struct strlist *self)
{
	return self->nr_entries == 0;
}

static inline unsigned int strlist__nr_entries(const struct strlist *self)
{
	return self->nr_entries;
}

/* For strlist iteration */
static inline struct str_node *strlist__first(struct strlist *self)
{
	struct rb_node *rn = rb_first(&self->entries);
	return rn ? rb_entry(rn, struct str_node, rb_node) : NULL;
}
static inline struct str_node *strlist__next(struct str_node *sn)
{
	struct rb_node *rn;
	if (!sn)
		return NULL;
	rn = rb_next(&sn->rb_node);
	return rn ? rb_entry(rn, struct str_node, rb_node) : NULL;
}

/**
 * strlist_for_each      - iterate over a strlist
 * @pos:	the &struct str_node to use as a loop cursor.
 * @self:	the &struct strlist for loop.
 */
#define strlist__for_each(pos, self)	\
	for (pos = strlist__first(self); pos; pos = strlist__next(pos))

/**
 * strlist_for_each_safe - iterate over a strlist safe against removal of
 *                         str_node
 * @pos:	the &struct str_node to use as a loop cursor.
 * @n:		another &struct str_node to use as temporary storage.
 * @self:	the &struct strlist for loop.
 */
#define strlist__for_each_safe(pos, n, self)	\
	for (pos = strlist__first(self), n = strlist__next(pos); pos;\
	     pos = n, n = strlist__next(n))

int strlist__parse_list(struct strlist *self, const char *s);
#endif /* __PERF_STRLIST_H */
