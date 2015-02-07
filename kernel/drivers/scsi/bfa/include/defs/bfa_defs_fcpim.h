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
#ifndef __BFA_DEFS_FCPIM_H__
#define __BFA_DEFS_FCPIM_H__

struct bfa_fcpim_stats_s {
	u32        total_ios;	/*  Total IO count */
	u32        qresumes;	/*  IO waiting for CQ space */
	u32        no_iotags;	/*  NO IO contexts */
	u32        io_aborts;	/*  IO abort requests */
	u32        no_tskims;	/*  NO task management contexts */
	u32        iocomp_ok;	/*  IO completions with OK status */
	u32        iocomp_underrun;	/*  IO underrun (good) */
	u32        iocomp_overrun;	/*  IO overrun (good) */
	u32        iocomp_aborted;	/*  Aborted IO requests */
	u32        iocomp_timedout;	/*  IO timeouts */
	u32        iocom_nexus_abort;	/*  IO selection timeouts */
	u32        iocom_proto_err;	/*  IO protocol errors */
	u32        iocom_dif_err;	/*  IO SBC-3 protection errors */
	u32        iocom_tm_abort;	/*  IO aborted by TM requests */
	u32        iocom_sqer_needed;	/*  IO retry for SQ error
						 *recovery */
	u32        iocom_res_free;	/*  Delayed freeing of IO resources */
	u32        iocomp_scsierr;	/*  IO with non-good SCSI status */
	u32        iocom_hostabrts;	/*  Host IO abort requests */
	u32        iocom_utags;	/*  IO comp with unknown tags */
	u32        io_cleanups;	/*  IO implicitly aborted */
	u32        io_tmaborts;	/*  IO aborted due to TM commands */
	u32        rsvd;
};
#endif /*__BFA_DEFS_FCPIM_H__*/
