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

#ifndef BLK_IOPOLL_H
#define BLK_IOPOLL_H

struct blk_iopoll;
typedef int (blk_iopoll_fn)(struct blk_iopoll *, int);

struct blk_iopoll {
	struct list_head list;
	unsigned long state;
	unsigned long data;
	int weight;
	int max;
	blk_iopoll_fn *poll;
};

enum {
	IOPOLL_F_SCHED		= 0,
	IOPOLL_F_DISABLE	= 1,
};

/*
 * Returns 0 if we successfully set the IOPOLL_F_SCHED bit, indicating
 * that we were the first to acquire this iop for scheduling. If this iop
 * is currently disabled, return "failure".
 */
static inline int blk_iopoll_sched_prep(struct blk_iopoll *iop)
{
	if (!test_bit(IOPOLL_F_DISABLE, &iop->state))
		return test_and_set_bit(IOPOLL_F_SCHED, &iop->state);

	return 1;
}

static inline int blk_iopoll_disable_pending(struct blk_iopoll *iop)
{
	return test_bit(IOPOLL_F_DISABLE, &iop->state);
}

extern void blk_iopoll_sched(struct blk_iopoll *);
extern void blk_iopoll_init(struct blk_iopoll *, int, blk_iopoll_fn *);
extern void blk_iopoll_complete(struct blk_iopoll *);
extern void __blk_iopoll_complete(struct blk_iopoll *);
extern void blk_iopoll_enable(struct blk_iopoll *);
extern void blk_iopoll_disable(struct blk_iopoll *);

extern int blk_iopoll_enabled;

#endif
