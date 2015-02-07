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
 * broadsheetfb.h - definitions for the broadsheet framebuffer driver
 *
 * Copyright (C) 2008 by Jaya Kumar
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file COPYING in the main directory of this archive for
 * more details.
 *
 */

#ifndef _LINUX_BROADSHEETFB_H_
#define _LINUX_BROADSHEETFB_H_

/* Broadsheet command defines */
#define BS_CMD_INIT_SYS_RUN	0x06
#define BS_CMD_INIT_DSPE_CFG	0x09
#define BS_CMD_INIT_DSPE_TMG	0x0A
#define BS_CMD_INIT_ROTMODE	0x0B
#define BS_CMD_RD_REG		0x10
#define BS_CMD_WR_REG		0x11
#define BS_CMD_LD_IMG		0x20
#define BS_CMD_LD_IMG_AREA	0x22
#define BS_CMD_LD_IMG_END	0x23
#define BS_CMD_WAIT_DSPE_TRG	0x28
#define BS_CMD_WAIT_DSPE_FREND	0x29
#define BS_CMD_RD_WFM_INFO	0x30
#define BS_CMD_UPD_INIT		0x32
#define BS_CMD_UPD_FULL		0x33
#define BS_CMD_UPD_GDRV_CLR	0x37

/* Broadsheet register interface defines */
#define BS_REG_REV		0x00
#define BS_REG_PRC		0x02

/* Broadsheet pin interface specific defines */
#define BS_CS	0x01
#define BS_DC 	0x02
#define BS_WR 	0x03

/* Broadsheet IO interface specific defines */
#define BS_MMIO_CMD	0x01
#define BS_MMIO_DATA	0x02

/* struct used by broadsheet. board specific stuff comes from *board */
struct broadsheetfb_par {
	struct fb_info *info;
	struct broadsheet_board *board;
	void (*write_reg)(struct broadsheetfb_par *, u16 reg, u16 val);
	u16 (*read_reg)(struct broadsheetfb_par *, u16 reg);
	wait_queue_head_t waitq;
	int panel_index;
	struct mutex io_lock;
};

/* board specific routines */
struct broadsheet_board {
	struct module *owner;
	int (*init)(struct broadsheetfb_par *);
	int (*wait_for_rdy)(struct broadsheetfb_par *);
	void (*cleanup)(struct broadsheetfb_par *);
	int (*get_panel_type)(void);
	int (*setup_irq)(struct fb_info *);

	/* Functions for boards that use GPIO */
	void (*set_ctl)(struct broadsheetfb_par *, unsigned char, u8);
	void (*set_hdb)(struct broadsheetfb_par *, u16);
	u16 (*get_hdb)(struct broadsheetfb_par *);

	/* Functions for boards that have specialized MMIO */
	void (*mmio_write)(struct broadsheetfb_par *, int type, u16);
	u16 (*mmio_read)(struct broadsheetfb_par *);
};
#endif
