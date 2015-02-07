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

#ifndef __BFA_PRIV_H__
#define __BFA_PRIV_H__

#include "bfa_iocfc.h"
#include "bfa_intr_priv.h"
#include "bfa_trcmod_priv.h"
#include "bfa_modules_priv.h"
#include "bfa_fwimg_priv.h"
#include <cs/bfa_log.h>
#include <bfa_timer.h>

/**
 * Macro to define a new BFA module
 */
#define BFA_MODULE(__mod) 						\
	static void bfa_ ## __mod ## _meminfo(				\
			struct bfa_iocfc_cfg_s *cfg, u32 *ndm_len,	\
			u32 *dm_len);      \
	static void bfa_ ## __mod ## _attach(struct bfa_s *bfa,		\
			void *bfad, struct bfa_iocfc_cfg_s *cfg, 	\
			struct bfa_meminfo_s *meminfo,			\
			struct bfa_pcidev_s *pcidev);      \
	static void bfa_ ## __mod ## _initdone(struct bfa_s *bfa);      \
	static void bfa_ ## __mod ## _detach(struct bfa_s *bfa);      \
	static void bfa_ ## __mod ## _start(struct bfa_s *bfa);      \
	static void bfa_ ## __mod ## _stop(struct bfa_s *bfa);      \
	static void bfa_ ## __mod ## _iocdisable(struct bfa_s *bfa);      \
									\
	extern struct bfa_module_s hal_mod_ ## __mod;			\
	struct bfa_module_s hal_mod_ ## __mod = {			\
		bfa_ ## __mod ## _meminfo,				\
		bfa_ ## __mod ## _attach,				\
		bfa_ ## __mod ## _initdone,				\
		bfa_ ## __mod ## _detach,				\
		bfa_ ## __mod ## _start,				\
		bfa_ ## __mod ## _stop,					\
		bfa_ ## __mod ## _iocdisable,				\
	}

#define BFA_CACHELINE_SZ	(256)

/**
 * Structure used to interact between different BFA sub modules
 *
 * Each sub module needs to implement only the entry points relevant to it (and
 * can leave entry points as NULL)
 */
struct bfa_module_s {
	void (*meminfo) (struct bfa_iocfc_cfg_s *cfg, u32 *km_len,
			u32 *dm_len);
	void (*attach) (struct bfa_s *bfa, void *bfad,
			struct bfa_iocfc_cfg_s *cfg,
			struct bfa_meminfo_s *meminfo,
			struct bfa_pcidev_s *pcidev);
	void (*initdone) (struct bfa_s *bfa);
	void (*detach) (struct bfa_s *bfa);
	void (*start) (struct bfa_s *bfa);
	void (*stop) (struct bfa_s *bfa);
	void (*iocdisable) (struct bfa_s *bfa);
};

extern struct bfa_module_s *hal_mods[];

struct bfa_s {
	void			*bfad;		/*  BFA driver instance    */
	struct bfa_aen_s	*aen;		/*  AEN module		    */
	struct bfa_plog_s	*plog;		/*  portlog buffer	    */
	struct bfa_log_mod_s	*logm;		/*  driver logging modulen */
	struct bfa_trc_mod_s	*trcmod;	/*  driver tracing	    */
	struct bfa_ioc_s	ioc;		/*  IOC module		    */
	struct bfa_iocfc_s	iocfc;		/*  IOCFC module	    */
	struct bfa_timer_mod_s	timer_mod;	/*  timer module	    */
	struct bfa_modules_s	modules;	/*  BFA modules	    */
	struct list_head	comp_q;		/*  pending completions    */
	bfa_boolean_t		rme_process;	/*  RME processing enabled */
	struct list_head		reqq_waitq[BFI_IOC_MAX_CQS];
	bfa_boolean_t		fcs;		/*  FCS is attached to BFA */
	struct bfa_msix_s	msix;
};

extern bfa_isr_func_t bfa_isrs[BFI_MC_MAX];
extern bfa_ioc_mbox_mcfunc_t  bfa_mbox_isrs[];
extern bfa_boolean_t bfa_auto_recover;
extern struct bfa_module_s hal_mod_flash;
extern struct bfa_module_s hal_mod_fcdiag;
extern struct bfa_module_s hal_mod_sgpg;
extern struct bfa_module_s hal_mod_fcport;
extern struct bfa_module_s hal_mod_fcxp;
extern struct bfa_module_s hal_mod_lps;
extern struct bfa_module_s hal_mod_uf;
extern struct bfa_module_s hal_mod_rport;
extern struct bfa_module_s hal_mod_fcpim;
extern struct bfa_module_s hal_mod_pbind;

#endif /* __BFA_PRIV_H__ */

