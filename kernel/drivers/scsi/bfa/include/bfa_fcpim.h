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

#ifndef __BFA_FCPIM_H__
#define __BFA_FCPIM_H__

#include <bfa.h>
#include <bfa_svc.h>
#include <bfi/bfi_fcpim.h>
#include <defs/bfa_defs_fcpim.h>

/*
 * forward declarations
 */
struct bfa_itnim_s;
struct bfa_ioim_s;
struct bfa_tskim_s;
struct bfad_ioim_s;
struct bfad_tskim_s;

/*
 * bfa fcpim module API functions
 */
void		bfa_fcpim_path_tov_set(struct bfa_s *bfa, u16 path_tov);
u16	bfa_fcpim_path_tov_get(struct bfa_s *bfa);
void		bfa_fcpim_qdepth_set(struct bfa_s *bfa, u16 q_depth);
u16	bfa_fcpim_qdepth_get(struct bfa_s *bfa);
bfa_status_t bfa_fcpim_get_modstats(struct bfa_s *bfa,
			struct bfa_fcpim_stats_s *modstats);
bfa_status_t bfa_fcpim_clr_modstats(struct bfa_s *bfa);

/*
 * bfa itnim API functions
 */
struct bfa_itnim_s *bfa_itnim_create(struct bfa_s *bfa,
					struct bfa_rport_s *rport, void *itnim);
void		bfa_itnim_delete(struct bfa_itnim_s *itnim);
void		bfa_itnim_online(struct bfa_itnim_s *itnim,
				 bfa_boolean_t seq_rec);
void		bfa_itnim_offline(struct bfa_itnim_s *itnim);
void		bfa_itnim_get_stats(struct bfa_itnim_s *itnim,
			struct bfa_itnim_hal_stats_s *stats);
void		bfa_itnim_clear_stats(struct bfa_itnim_s *itnim);


/**
 * 		BFA completion callback for bfa_itnim_online().
 *
 * @param[in]		itnim		FCS or driver itnim instance
 *
 * return None
 */
void            bfa_cb_itnim_online(void *itnim);

/**
 * 		BFA completion callback for bfa_itnim_offline().
 *
 * @param[in]		itnim		FCS or driver itnim instance
 *
 * return None
 */
void            bfa_cb_itnim_offline(void *itnim);
void            bfa_cb_itnim_tov_begin(void *itnim);
void            bfa_cb_itnim_tov(void *itnim);

/**
 * 		BFA notification to FCS/driver for second level error recovery.
 *
 * Atleast one I/O request has timedout and target is unresponsive to
 * repeated abort requests. Second level error recovery should be initiated
 * by starting implicit logout and recovery procedures.
 *
 * @param[in]		itnim		FCS or driver itnim instance
 *
 * return None
 */
void            bfa_cb_itnim_sler(void *itnim);

/*
 * bfa ioim API functions
 */
struct bfa_ioim_s	*bfa_ioim_alloc(struct bfa_s *bfa,
					struct bfad_ioim_s *dio,
					struct bfa_itnim_s *itnim,
					u16 nsgles);

void		bfa_ioim_free(struct bfa_ioim_s *ioim);
void		bfa_ioim_start(struct bfa_ioim_s *ioim);
void		bfa_ioim_abort(struct bfa_ioim_s *ioim);
void 		bfa_ioim_delayed_comp(struct bfa_ioim_s *ioim,
				      bfa_boolean_t iotov);


/**
 * 		I/O completion notification.
 *
 * @param[in]		dio			driver IO structure
 * @param[in]		io_status		IO completion status
 * @param[in]		scsi_status		SCSI status returned by target
 * @param[in]		sns_len			SCSI sense length, 0 if none
 * @param[in]		sns_info		SCSI sense data, if any
 * @param[in]		residue			Residual length
 *
 * @return None
 */
void            bfa_cb_ioim_done(void *bfad, struct bfad_ioim_s *dio,
				  enum bfi_ioim_status io_status,
				  u8 scsi_status, int sns_len,
				  u8 *sns_info, s32 residue);

/**
 * 		I/O good completion notification.
 *
 * @param[in]		dio			driver IO structure
 *
 * @return None
 */
void            bfa_cb_ioim_good_comp(void *bfad, struct bfad_ioim_s *dio);

/**
 * 		I/O abort completion notification
 *
 * @param[in]		dio			driver IO that was aborted
 *
 * @return None
 */
void            bfa_cb_ioim_abort(void *bfad, struct bfad_ioim_s *dio);
void		bfa_cb_ioim_resfree(void *hcb_bfad);

void 			bfa_cb_ioim_resfree(void *hcb_bfad);

/*
 * bfa tskim API functions
 */
struct bfa_tskim_s	*bfa_tskim_alloc(struct bfa_s *bfa,
					struct bfad_tskim_s *dtsk);
void		bfa_tskim_free(struct bfa_tskim_s *tskim);
void		bfa_tskim_start(struct bfa_tskim_s *tskim,
				struct bfa_itnim_s *itnim, lun_t lun,
				enum fcp_tm_cmnd tm, u8 t_secs);
void		bfa_cb_tskim_done(void *bfad, struct bfad_tskim_s *dtsk,
				  enum bfi_tskim_status tsk_status);

#endif /* __BFA_FCPIM_H__ */

