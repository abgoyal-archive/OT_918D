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
 *  fs/partitions/ultrix.c
 *
 *  Code extracted from drivers/block/genhd.c
 *
 *  Re-organised Jul 1999 Russell King
 */

#include "check.h"
#include "ultrix.h"

int ultrix_partition(struct parsed_partitions *state)
{
	int i;
	Sector sect;
	unsigned char *data;
	struct ultrix_disklabel {
		s32	pt_magic;	/* magic no. indicating part. info exits */
		s32	pt_valid;	/* set by driver if pt is current */
		struct  pt_info {
			s32		pi_nblocks; /* no. of sectors */
			u32		pi_blkoff;  /* block offset for start */
		} pt_part[8];
	} *label;

#define PT_MAGIC	0x032957	/* Partition magic number */
#define PT_VALID	1		/* Indicates if struct is valid */

	data = read_part_sector(state, (16384 - sizeof(*label))/512, &sect);
	if (!data)
		return -1;
	
	label = (struct ultrix_disklabel *)(data + 512 - sizeof(*label));

	if (label->pt_magic == PT_MAGIC && label->pt_valid == PT_VALID) {
		for (i=0; i<8; i++)
			if (label->pt_part[i].pi_nblocks)
				put_partition(state, i+1, 
					      label->pt_part[i].pi_blkoff,
					      label->pt_part[i].pi_nblocks);
		put_dev_sector(sect);
		printk ("\n");
		return 1;
	} else {
		put_dev_sector(sect);
		return 0;
	}
}
