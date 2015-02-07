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

#ifndef __BFA_PORT_PRIV_H__
#define __BFA_PORT_PRIV_H__

#include <defs/bfa_defs_pport.h>
#include <bfi/bfi_pport.h>
#include "bfa_intr_priv.h"

/**
 * Link notification data structure
 */
struct bfa_fcport_ln_s {
	struct bfa_fcport_s     *fcport;
	bfa_sm_t                sm;
	struct bfa_cb_qe_s      ln_qe;  /*  BFA callback queue elem for ln */
	enum bfa_pport_linkstate ln_event; /*  ln event for callback */
};

/**
 * BFA FC port data structure
 */
struct bfa_fcport_s {
	struct bfa_s 		*bfa;	/*  parent BFA instance */
	bfa_sm_t		sm;	/*  port state machine */
	wwn_t			nwwn;	/*  node wwn of physical port */
	wwn_t			pwwn;	/*  port wwn of physical oprt */
	enum bfa_pport_speed speed_sup;
					/*  supported speeds */
	enum bfa_pport_speed speed;	/*  current speed */
	enum bfa_pport_topology topology;	/*  current topology */
	u8			myalpa;	/*  my ALPA in LOOP topology */
	u8			rsvd[3];
	u32             mypid:24;
	u32             rsvd_b:8;
	struct bfa_pport_cfg_s	cfg;	/*  current port configuration */
	struct bfa_qos_attr_s  qos_attr;   /* QoS Attributes */
	struct bfa_qos_vc_attr_s qos_vc_attr;  /*  VC info from ELP */
	struct bfa_reqq_wait_s	reqq_wait;
					/*  to wait for room in reqq */
	struct bfa_reqq_wait_s	svcreq_wait;
					/*  to wait for room in reqq */
	struct bfa_reqq_wait_s	stats_reqq_wait;
					/*  to wait for room in reqq (stats) */
	void			*event_cbarg;
	void			(*event_cbfn) (void *cbarg,
						bfa_pport_event_t event);
	union {
		union bfi_fcport_i2h_msg_u i2hmsg;
	} event_arg;
	void			*bfad;	/*  BFA driver handle */
	struct bfa_fcport_ln_s   ln; /* Link Notification */
	struct bfa_cb_qe_s		hcb_qe;	/*  BFA callback queue elem */
	struct bfa_timer_s      timer;  /*  timer */
	u32		msgtag;	/*  fimrware msg tag for reply */
	u8			*stats_kva;
	u64		stats_pa;
	union bfa_fcport_stats_u *stats;
	union bfa_fcport_stats_u *stats_ret; /*  driver stats location */
	bfa_status_t            stats_status; /*  stats/statsclr status */
	bfa_boolean_t           stats_busy; /*  outstanding stats/statsclr */
	bfa_boolean_t           stats_qfull;
	bfa_cb_pport_t          stats_cbfn; /*  driver callback function */
	void                    *stats_cbarg; /* *!< user callback arg */
	bfa_boolean_t           diag_busy; /*  diag busy status */
	bfa_boolean_t           beacon; /*  port beacon status */
	bfa_boolean_t           link_e2e_beacon; /*  link beacon status */
};

#define BFA_FCPORT_MOD(__bfa)	(&(__bfa)->modules.fcport)

/*
 * public functions
 */
void	bfa_fcport_isr(struct bfa_s *bfa, struct bfi_msg_s *msg);
#endif /* __BFA_PORT_PRIV_H__ */
