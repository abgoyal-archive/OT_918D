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
 * arch/arm/plat-spear/include/plat/padmux.h
 *
 * SPEAr platform specific gpio pads muxing file
 *
 * Copyright (C) 2009 ST Microelectronics
 * Viresh Kumar<viresh.kumar@st.com>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#ifndef __PLAT_PADMUX_H
#define __PLAT_PADMUX_H

#include <linux/types.h>

/*
 * struct pmx_reg: configuration structure for mode reg and mux reg
 *
 * offset: offset of mode reg
 * mask: mask of mode reg
 */
struct pmx_reg {
	u32 offset;
	u32 mask;
};

/*
 * struct pmx_dev_mode: configuration structure every group of modes of a device
 *
 * ids: all modes for this configuration
 * mask: mask for supported mode
 */
struct pmx_dev_mode {
	u32 ids;
	u32 mask;
};

/*
 * struct pmx_mode: mode definition structure
 *
 * name: mode name
 * mask: mode mask
 */
struct pmx_mode {
	char *name;
	u32 id;
	u32 mask;
};

/*
 * struct pmx_dev: device definition structure
 *
 * name: device name
 * modes: device configuration array for different modes supported
 * mode_count: size of modes array
 * is_active: is peripheral active/enabled
 * enb_on_reset: if 1, mask bits to be cleared in reg otherwise to be set in reg
 */
struct pmx_dev {
	char *name;
	struct pmx_dev_mode *modes;
	u8 mode_count;
	bool is_active;
	bool enb_on_reset;
};

/*
 * struct pmx_driver: driver definition structure
 *
 * mode: mode to be set
 * devs: array of pointer to pmx devices
 * devs_count: ARRAY_SIZE of devs
 * base: base address of soc config registers
 * mode_reg: structure of mode config register
 * mux_reg: structure of device mux config register
 */
struct pmx_driver {
	struct pmx_mode *mode;
	struct pmx_dev **devs;
	u8 devs_count;
	u32 *base;
	struct pmx_reg mode_reg;
	struct pmx_reg mux_reg;
};

/* pmx functions */
int pmx_register(struct pmx_driver *driver);

#endif /* __PLAT_PADMUX_H */
