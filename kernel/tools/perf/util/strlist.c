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
 * (c) 2009 Arnaldo Carvalho de Melo <acme@redhat.com>
 *
 * Licensed under the GPLv2.
 */

#include "strlist.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct str_node *str_node__new(const char *s, bool dupstr)
{
	struct str_node *self = malloc(sizeof(*self));

	if (self != NULL) {
		if (dupstr) {
			s = strdup(s);
			if (s == NULL)
				goto out_delete;
		}
		self->s = s;
	}

	return self;

out_delete:
	free(self);
	return NULL;
}

static void str_node__delete(struct str_node *self, bool dupstr)
{
	if (dupstr)
		free((void *)self->s);
	free(self);
}

int strlist__add(struct strlist *self, const char *new_entry)
{
	struct rb_node **p = &self->entries.rb_node;
	struct rb_node *parent = NULL;
	struct str_node *sn;

	while (*p != NULL) {
		int rc;

		parent = *p;
		sn = rb_entry(parent, struct str_node, rb_node);
		rc = strcmp(sn->s, new_entry);

		if (rc > 0)
			p = &(*p)->rb_left;
		else if (rc < 0)
			p = &(*p)->rb_right;
		else
			return -EEXIST;
	}

	sn = str_node__new(new_entry, self->dupstr);
	if (sn == NULL)
		return -ENOMEM;

	rb_link_node(&sn->rb_node, parent, p);
	rb_insert_color(&sn->rb_node, &self->entries);
	++self->nr_entries;

	return 0;
}

int strlist__load(struct strlist *self, const char *filename)
{
	char entry[1024];
	int err;
	FILE *fp = fopen(filename, "r");

	if (fp == NULL)
		return errno;

	while (fgets(entry, sizeof(entry), fp) != NULL) {
		const size_t len = strlen(entry);

		if (len == 0)
			continue;
		entry[len - 1] = '\0';

		err = strlist__add(self, entry);
		if (err != 0)
			goto out;
	}

	err = 0;
out:
	fclose(fp);
	return err;
}

void strlist__remove(struct strlist *self, struct str_node *sn)
{
	rb_erase(&sn->rb_node, &self->entries);
	str_node__delete(sn, self->dupstr);
}

struct str_node *strlist__find(struct strlist *self, const char *entry)
{
	struct rb_node **p = &self->entries.rb_node;
	struct rb_node *parent = NULL;

	while (*p != NULL) {
		struct str_node *sn;
		int rc;

		parent = *p;
		sn = rb_entry(parent, struct str_node, rb_node);
		rc = strcmp(sn->s, entry);

		if (rc > 0)
			p = &(*p)->rb_left;
		else if (rc < 0)
			p = &(*p)->rb_right;
		else
			return sn;
	}

	return NULL;
}

static int strlist__parse_list_entry(struct strlist *self, const char *s)
{
	if (strncmp(s, "file://", 7) == 0)
		return strlist__load(self, s + 7);

	return strlist__add(self, s);
}

int strlist__parse_list(struct strlist *self, const char *s)
{
	char *sep;
	int err;

	while ((sep = strchr(s, ',')) != NULL) {
		*sep = '\0';
		err = strlist__parse_list_entry(self, s);
		*sep = ',';
		if (err != 0)
			return err;
		s = sep + 1;
	}

	return *s ? strlist__parse_list_entry(self, s) : 0;
}

struct strlist *strlist__new(bool dupstr, const char *slist)
{
	struct strlist *self = malloc(sizeof(*self));

	if (self != NULL) {
		self->entries	 = RB_ROOT;
		self->dupstr	 = dupstr;
		self->nr_entries = 0;
		if (slist && strlist__parse_list(self, slist) != 0)
			goto out_error;
	}

	return self;
out_error:
	free(self);
	return NULL;
}

void strlist__delete(struct strlist *self)
{
	if (self != NULL) {
		struct str_node *pos;
		struct rb_node *next = rb_first(&self->entries);

		while (next) {
			pos = rb_entry(next, struct str_node, rb_node);
			next = rb_next(&pos->rb_node);
			strlist__remove(self, pos);
		}
		self->entries = RB_ROOT;
		free(self);
	}
}

struct str_node *strlist__entry(const struct strlist *self, unsigned int idx)
{
	struct rb_node *nd;

	for (nd = rb_first(&self->entries); nd; nd = rb_next(nd)) {
		struct str_node *pos = rb_entry(nd, struct str_node, rb_node);

		if (!idx--)
			return pos;
	}

	return NULL;
}
