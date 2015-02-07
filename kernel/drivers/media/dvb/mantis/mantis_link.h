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
	Mantis PCI bridge driver

	Copyright (C) Manu Abraham (abraham.manu@gmail.com)

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef __MANTIS_LINK_H
#define __MANTIS_LINK_H

#include <linux/mutex.h>
#include <linux/workqueue.h>
#include "dvb_ca_en50221.h"

enum mantis_sbuf_status {
	MANTIS_SBUF_DATA_AVAIL		= 1,
	MANTIS_SBUF_DATA_EMPTY		= 2,
	MANTIS_SBUF_DATA_OVFLW		= 3
};

struct mantis_slot {
	u32				timeout;
	u32				slave_cfg;
	u32				bar;
};

/* Physical layer */
enum mantis_slot_state {
	MODULE_INSERTED			= 3,
	MODULE_XTRACTED			= 4
};

struct mantis_ca {
	struct mantis_slot		slot[4];

	struct work_struct		hif_evm_work;

	u32				hif_event;
	wait_queue_head_t		hif_opdone_wq;
	wait_queue_head_t		hif_brrdyw_wq;
	wait_queue_head_t		hif_data_wq;
	wait_queue_head_t		hif_write_wq; /* HIF Write op */

	enum mantis_sbuf_status		sbuf_status;

	enum mantis_slot_state		slot_state;

	void				*ca_priv;

	struct dvb_ca_en50221		en50221;
	struct mutex			ca_lock;
};

/* CA */
extern void mantis_event_cam_plugin(struct mantis_ca *ca);
extern void mantis_event_cam_unplug(struct mantis_ca *ca);
extern int mantis_pcmcia_init(struct mantis_ca *ca);
extern void mantis_pcmcia_exit(struct mantis_ca *ca);
extern int mantis_evmgr_init(struct mantis_ca *ca);
extern void mantis_evmgr_exit(struct mantis_ca *ca);

/* HIF */
extern int mantis_hif_init(struct mantis_ca *ca);
extern void mantis_hif_exit(struct mantis_ca *ca);
extern int mantis_hif_read_mem(struct mantis_ca *ca, u32 addr);
extern int mantis_hif_write_mem(struct mantis_ca *ca, u32 addr, u8 data);
extern int mantis_hif_read_iom(struct mantis_ca *ca, u32 addr);
extern int mantis_hif_write_iom(struct mantis_ca *ca, u32 addr, u8 data);

#endif /* __MANTIS_LINK_H */
