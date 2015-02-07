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
 * Copyright (c) 2005-2009 Brocade Communications Systems, Inc.
 * All rights reserved
 * www.brocade.com
 *
 * Linux driver for Brocade Fibre Channel Host Bus Adapter.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License (GPL) Version 2 as
 * published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */

#ifndef __BFA_CEE_H__
#define __BFA_CEE_H__

#include <defs/bfa_defs_cee.h>
#include <bfa_ioc.h>
#include <cs/bfa_trc.h>
#include <cs/bfa_log.h>

typedef void (*bfa_cee_get_attr_cbfn_t) (void *dev, bfa_status_t status);
typedef void (*bfa_cee_get_stats_cbfn_t) (void *dev, bfa_status_t status);
typedef void (*bfa_cee_reset_stats_cbfn_t) (void *dev, bfa_status_t status);
typedef void (*bfa_cee_hbfail_cbfn_t) (void *dev, bfa_status_t status);

struct bfa_cee_cbfn_s {
	bfa_cee_get_attr_cbfn_t    get_attr_cbfn;
	void *get_attr_cbarg;
	bfa_cee_get_stats_cbfn_t   get_stats_cbfn;
	void *get_stats_cbarg;
	bfa_cee_reset_stats_cbfn_t reset_stats_cbfn;
	void *reset_stats_cbarg;
};

struct bfa_cee_s {
	void *dev;
	bfa_boolean_t get_attr_pending;
	bfa_boolean_t get_stats_pending;
	bfa_boolean_t reset_stats_pending;
	bfa_status_t get_attr_status;
	bfa_status_t get_stats_status;
	bfa_status_t reset_stats_status;
	struct bfa_cee_cbfn_s cbfn;
	struct bfa_ioc_hbfail_notify_s hbfail;
	struct bfa_trc_mod_s *trcmod;
	struct bfa_log_mod_s *logmod;
	struct bfa_cee_attr_s *attr;
	struct bfa_cee_stats_s *stats;
	struct bfa_dma_s attr_dma;
	struct bfa_dma_s stats_dma;
	struct bfa_ioc_s *ioc;
	struct bfa_mbox_cmd_s get_cfg_mb;
	struct bfa_mbox_cmd_s get_stats_mb;
	struct bfa_mbox_cmd_s reset_stats_mb;
};

u32 bfa_cee_meminfo(void);
void bfa_cee_mem_claim(struct bfa_cee_s *cee, u8 *dma_kva,
			 u64 dma_pa);
void bfa_cee_attach(struct bfa_cee_s *cee, struct bfa_ioc_s *ioc, void *dev,
			struct bfa_trc_mod_s *trcmod,
			struct bfa_log_mod_s *logmod);
void bfa_cee_detach(struct bfa_cee_s *cee);
bfa_status_t bfa_cee_get_attr(struct bfa_cee_s *cee,
			 struct bfa_cee_attr_s *attr,
			bfa_cee_get_attr_cbfn_t cbfn, void *cbarg);
bfa_status_t bfa_cee_get_stats(struct bfa_cee_s *cee,
			struct bfa_cee_stats_s *stats,
			bfa_cee_get_stats_cbfn_t cbfn, void *cbarg);
bfa_status_t bfa_cee_reset_stats(struct bfa_cee_s *cee,
			bfa_cee_reset_stats_cbfn_t cbfn, void *cbarg);
#endif /* __BFA_CEE_H__ */
