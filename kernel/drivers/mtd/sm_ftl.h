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
 * Copyright © 2009 - Maxim Levitsky
 * SmartMedia/xD translation layer
 *
 * Based loosly on ssfdc.c which is
 *  © 2005 Eptar srl
 *  Author: Claudio Lanconelli <lanconelli.claudio@eptar.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/mtd/blktrans.h>
#include <linux/kfifo.h>
#include <linux/sched.h>
#include <linux/completion.h>
#include <linux/mtd/mtd.h>



struct ftl_zone {
	int initialized;
	int16_t *lba_to_phys_table;		/* LBA to physical table */
	struct kfifo free_sectors;	/* queue of free sectors */
};

struct sm_ftl {
	struct mtd_blktrans_dev *trans;

	struct mutex mutex;		/* protects the structure */
	struct ftl_zone *zones;		/* FTL tables for each zone */

	/* Media information */
	int block_size;			/* block size in bytes */
	int zone_size;			/* zone size in blocks */
	int zone_count;			/* number of zones */
	int max_lba;			/* maximum lba in a zone */
	int smallpagenand;		/* 256 bytes/page nand */
	int readonly;			/* is FS readonly */
	int unstable;
	int cis_block;			/* CIS block location */
	int cis_boffset;		/* CIS offset in the block */
	int cis_page_offset;		/* CIS offset in the page */
	void *cis_buffer;		/* tmp buffer for cis reads */

	/* Cache */
	int cache_block;		/* block number of cached block */
	int cache_zone;			/* zone of cached block */
	unsigned char *cache_data;	/* cached block data */
	long unsigned int cache_data_invalid_bitmap;
	int cache_clean;
	struct work_struct flush_work;
	struct timer_list timer;

	/* Async erase stuff */
	struct completion erase_completion;

	/* Geometry stuff */
	int heads;
	int sectors;
	int cylinders;

	struct attribute_group *disk_attributes;
};

struct chs_entry {
	unsigned long size;
	unsigned short cyl;
	unsigned char head;
	unsigned char sec;
};


#define SM_FTL_PARTN_BITS	3

#define sm_printk(format, ...) \
	printk(KERN_WARNING "sm_ftl" ": " format "\n", ## __VA_ARGS__)

#define dbg(format, ...) \
	if (debug) \
		printk(KERN_DEBUG "sm_ftl" ": " format "\n", ## __VA_ARGS__)

#define dbg_verbose(format, ...) \
	if (debug > 1) \
		printk(KERN_DEBUG "sm_ftl" ": " format "\n", ## __VA_ARGS__)


static void sm_erase_callback(struct erase_info *self);
static int sm_erase_block(struct sm_ftl *ftl, int zone_num, uint16_t block,
								int put_free);
static void sm_mark_block_bad(struct sm_ftl *ftl, int zone_num, int block);

static int sm_recheck_media(struct sm_ftl *ftl);
