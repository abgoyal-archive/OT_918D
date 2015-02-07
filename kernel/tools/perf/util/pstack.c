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
 * Simple pointer stack
 *
 * (c) 2010 Arnaldo Carvalho de Melo <acme@redhat.com>
 */

#include "util.h"
#include "pstack.h"
#include <linux/kernel.h>
#include <stdlib.h>

struct pstack {
	unsigned short	top;
	unsigned short	max_nr_entries;
	void		*entries[0];
};

struct pstack *pstack__new(unsigned short max_nr_entries)
{
	struct pstack *self = zalloc((sizeof(*self) +
				     max_nr_entries * sizeof(void *)));
	if (self != NULL)
		self->max_nr_entries = max_nr_entries;
	return self;
}

void pstack__delete(struct pstack *self)
{
	free(self);
}

bool pstack__empty(const struct pstack *self)
{
	return self->top == 0;
}

void pstack__remove(struct pstack *self, void *key)
{
	unsigned short i = self->top, last_index = self->top - 1;

	while (i-- != 0) {
		if (self->entries[i] == key) {
			if (i < last_index)
				memmove(self->entries + i,
					self->entries + i + 1,
					(last_index - i) * sizeof(void *));
			--self->top;
			return;
		}
	}
	pr_err("%s: %p not on the pstack!\n", __func__, key);
}

void pstack__push(struct pstack *self, void *key)
{
	if (self->top == self->max_nr_entries) {
		pr_err("%s: top=%d, overflow!\n", __func__, self->top);
		return;
	}
	self->entries[self->top++] = key;
}

void *pstack__pop(struct pstack *self)
{
	void *ret;

	if (self->top == 0) {
		pr_err("%s: underflow!\n", __func__);
		return NULL;
	}

	ret = self->entries[--self->top];
	self->entries[self->top] = NULL;
	return ret;
}
