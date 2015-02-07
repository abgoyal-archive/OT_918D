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

#ifndef __BFA_DEFS_VF_H__
#define __BFA_DEFS_VF_H__

#include <bfa_os_inc.h>
#include <defs/bfa_defs_port.h>
#include <protocol/types.h>

/**
 * VF states
 */
enum bfa_vf_state {
	BFA_VF_UNINIT    = 0,	/*  fabric is not yet initialized */
	BFA_VF_LINK_DOWN = 1,	/*  link is down */
	BFA_VF_FLOGI     = 2,	/*  flogi is in progress */
	BFA_VF_AUTH      = 3,	/*  authentication in progress */
	BFA_VF_NOFABRIC  = 4,	/*  fabric is not present */
	BFA_VF_ONLINE    = 5,	/*  login to fabric is complete */
	BFA_VF_EVFP      = 6,	/*  EVFP is in progress */
	BFA_VF_ISOLATED  = 7,	/*  port isolated due to vf_id mismatch */
};

/**
 * VF statistics
 */
struct bfa_vf_stats_s {
	u32        flogi_sent;	/*  Num FLOGIs sent 		*/
	u32        flogi_rsp_err;	/*  FLOGI response errors 	*/
	u32        flogi_acc_err;	/*  FLOGI accept errors 	*/
	u32        flogi_accepts;	/*  FLOGI accepts received 	*/
	u32        flogi_rejects;	/*  FLOGI rejects received 	*/
	u32        flogi_unknown_rsp; /*  Unknown responses for FLOGI */
	u32        flogi_alloc_wait; /*  Allocation waits prior to
					   * sending FLOGI
					   */
	u32        flogi_rcvd;	/*  FLOGIs received */
	u32        flogi_rejected;	/*  Incoming FLOGIs rejected */
	u32        fabric_onlines;	/*  Internal fabric online
					 * notification sent to other
					 * modules
					 */
	u32        fabric_offlines; /*  Internal fabric offline
					  * notification sent to other
					  * modules
					  */
	u32        resvd;
};

/**
 * VF attributes returned in queries
 */
struct bfa_vf_attr_s {
	enum bfa_vf_state  state;		/*  VF state */
	u32        rsvd;
	wwn_t           fabric_name;	/*  fabric name */
};

#endif /* __BFA_DEFS_VF_H__ */
