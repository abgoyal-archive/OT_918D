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

/**
 *  hal_sgpg.h BFA SG page module
 */

#ifndef __BFA_SGPG_PRIV_H__
#define __BFA_SGPG_PRIV_H__

#include <cs/bfa_q.h>

#define BFA_SGPG_MIN	(16)

/**
 * Alignment macro for SG page allocation
 */
#define BFA_SGPG_ROUNDUP(_l) (((_l) + (sizeof(struct bfi_sgpg_s) - 1)) \
			& ~(sizeof(struct bfi_sgpg_s) - 1))

struct bfa_sgpg_wqe_s {
	struct list_head qe;	/*  queue sg page element	*/
	int	nsgpg;		/*  pages to be allocated	*/
	int	nsgpg_total;	/*  total pages required	*/
	void	(*cbfn) (void *cbarg);
				/*  callback function		*/
	void	*cbarg;		/*  callback arg		*/
	struct list_head sgpg_q;	/*  queue of alloced sgpgs	*/
};

struct bfa_sgpg_s {
	struct list_head 	qe;	/*  queue sg page element	*/
	struct bfi_sgpg_s *sgpg; /*  va of SG page		*/
	union bfi_addr_u sgpg_pa;/*  pa of SG page		*/
};

/**
 * Given number of SG elements, BFA_SGPG_NPAGE() returns the number of
 * SG pages required.
 */
#define BFA_SGPG_NPAGE(_nsges)  (((_nsges) / BFI_SGPG_DATA_SGES) + 1)

struct bfa_sgpg_mod_s {
	struct bfa_s *bfa;
	int		num_sgpgs;	/*  number of SG pages		*/
	int		free_sgpgs;	/*  number of free SG pages	*/
	struct bfa_sgpg_s *hsgpg_arr;	/*  BFA SG page array	*/
	struct bfi_sgpg_s *sgpg_arr;	/*  actual SG page array	*/
	u64	sgpg_arr_pa;	/*  SG page array DMA addr	*/
	struct list_head sgpg_q;	/*  queue of free SG pages	*/
	struct list_head sgpg_wait_q; /*  wait queue for SG pages	*/
};
#define BFA_SGPG_MOD(__bfa)	(&(__bfa)->modules.sgpg_mod)

bfa_status_t	bfa_sgpg_malloc(struct bfa_s *bfa, struct list_head *sgpg_q,
								int nsgpgs);
void		bfa_sgpg_mfree(struct bfa_s *bfa, struct list_head *sgpg_q,
								int nsgpgs);
void		bfa_sgpg_winit(struct bfa_sgpg_wqe_s *wqe,
				   void (*cbfn) (void *cbarg), void *cbarg);
void		bfa_sgpg_wait(struct bfa_s *bfa, struct bfa_sgpg_wqe_s *wqe,
								int nsgpgs);
void		bfa_sgpg_wcancel(struct bfa_s *bfa, struct bfa_sgpg_wqe_s *wqe);

#endif /* __BFA_SGPG_PRIV_H__ */
