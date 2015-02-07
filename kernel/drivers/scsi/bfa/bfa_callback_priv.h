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

#ifndef __BFA_CALLBACK_PRIV_H__
#define __BFA_CALLBACK_PRIV_H__

#include <cs/bfa_q.h>

typedef void    (*bfa_cb_cbfn_t) (void *cbarg, bfa_boolean_t complete);

/**
 * Generic BFA callback element.
 */
struct bfa_cb_qe_s {
	struct list_head         qe;
	bfa_cb_cbfn_t  cbfn;
	bfa_boolean_t   once;
	u32		rsvd;
	void           *cbarg;
};

#define bfa_cb_queue(__bfa, __hcb_qe, __cbfn, __cbarg) do {		\
	(__hcb_qe)->cbfn  = (__cbfn);      \
	(__hcb_qe)->cbarg = (__cbarg);      \
	list_add_tail(&(__hcb_qe)->qe, &(__bfa)->comp_q);      \
} while (0)

#define bfa_cb_dequeue(__hcb_qe)	list_del(&(__hcb_qe)->qe)

#define bfa_cb_queue_once(__bfa, __hcb_qe, __cbfn, __cbarg) do {	\
	(__hcb_qe)->cbfn  = (__cbfn);      \
	(__hcb_qe)->cbarg = (__cbarg);      \
	if (!(__hcb_qe)->once) {      \
		list_add_tail((__hcb_qe), &(__bfa)->comp_q);      \
		(__hcb_qe)->once = BFA_TRUE;				\
	}								\
} while (0)

#define bfa_cb_queue_done(__hcb_qe) do {				\
	(__hcb_qe)->once = BFA_FALSE;					\
} while (0)

#endif /* __BFA_CALLBACK_PRIV_H__ */