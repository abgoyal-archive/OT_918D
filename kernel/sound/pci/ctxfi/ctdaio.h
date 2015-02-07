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

/**
 * Copyright (C) 2008, Creative Technology Ltd. All Rights Reserved.
 *
 * This source file is released under GPL v2 license (no other versions).
 * See the COPYING file included in the main directory of this source
 * distribution for the license terms and conditions.
 *
 * @File	ctdaio.h
 *
 * @Brief
 * This file contains the definition of Digital Audio Input Output
 * resource management object.
 *
 * @Author	Liu Chun
 * @Date 	May 23 2008
 *
 */

#ifndef CTDAIO_H
#define CTDAIO_H

#include "ctresource.h"
#include "ctimap.h"
#include <linux/spinlock.h>
#include <linux/list.h>

/* Define the descriptor of a daio resource */
enum DAIOTYP {
	LINEO1,
	LINEO2,
	LINEO3,
	LINEO4,
	SPDIFOO,	/* S/PDIF Out (Flexijack/Optical) */
	LINEIM,
	SPDIFIO,	/* S/PDIF In (Flexijack/Optical) on the card */
	SPDIFI1,	/* S/PDIF In on internal Drive Bay */
	NUM_DAIOTYP
};

struct dao_rsc_ops;
struct dai_rsc_ops;
struct daio_mgr;

struct daio {
	struct rsc rscl;	/* Basic resource info for left TX/RX */
	struct rsc rscr;	/* Basic resource info for right TX/RX */
	enum DAIOTYP type;
};

struct dao {
	struct daio daio;
	struct dao_rsc_ops *ops;	/* DAO specific operations */
	struct imapper **imappers;
	struct daio_mgr *mgr;
	void *hw;
	void *ctrl_blk;
};

struct dai {
	struct daio daio;
	struct dai_rsc_ops *ops;	/* DAI specific operations */
	void *hw;
	void *ctrl_blk;
};

struct dao_desc {
	unsigned int msr:4;
	unsigned int passthru:1;
};

struct dao_rsc_ops {
	int (*set_spos)(struct dao *dao, unsigned int spos);
	int (*commit_write)(struct dao *dao);
	int (*get_spos)(struct dao *dao, unsigned int *spos);
	int (*reinit)(struct dao *dao, const struct dao_desc *desc);
	int (*set_left_input)(struct dao *dao, struct rsc *input);
	int (*set_right_input)(struct dao *dao, struct rsc *input);
	int (*clear_left_input)(struct dao *dao);
	int (*clear_right_input)(struct dao *dao);
};

struct dai_rsc_ops {
	int (*set_srt_srcl)(struct dai *dai, struct rsc *src);
	int (*set_srt_srcr)(struct dai *dai, struct rsc *src);
	int (*set_srt_msr)(struct dai *dai, unsigned int msr);
	int (*set_enb_src)(struct dai *dai, unsigned int enb);
	int (*set_enb_srt)(struct dai *dai, unsigned int enb);
	int (*commit_write)(struct dai *dai);
};

/* Define daio resource request description info */
struct daio_desc {
	unsigned int type:4;
	unsigned int msr:4;
	unsigned int passthru:1;
};

struct daio_mgr {
	struct rsc_mgr mgr;	/* Basic resource manager info */
	spinlock_t mgr_lock;
	spinlock_t imap_lock;
	struct list_head imappers;
	struct imapper *init_imap;
	unsigned int init_imap_added;

	 /* request one daio resource */
	int (*get_daio)(struct daio_mgr *mgr,
			const struct daio_desc *desc, struct daio **rdaio);
	/* return one daio resource */
	int (*put_daio)(struct daio_mgr *mgr, struct daio *daio);
	int (*daio_enable)(struct daio_mgr *mgr, struct daio *daio);
	int (*daio_disable)(struct daio_mgr *mgr, struct daio *daio);
	int (*imap_add)(struct daio_mgr *mgr, struct imapper *entry);
	int (*imap_delete)(struct daio_mgr *mgr, struct imapper *entry);
	int (*commit_write)(struct daio_mgr *mgr);
};

/* Constructor and destructor of daio resource manager */
int daio_mgr_create(void *hw, struct daio_mgr **rdaio_mgr);
int daio_mgr_destroy(struct daio_mgr *daio_mgr);

#endif /* CTDAIO_H */
