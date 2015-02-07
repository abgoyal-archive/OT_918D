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
 * omap_hwmod common data structures
 *
 * Copyright (C) 2010 Texas Instruments, Inc.
 * Thara Gopinath <thara@ti.com>
 * Benoît Cousson
 *
 * Copyright (C) 2010 Nokia Corporation
 * Paul Walmsley
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This data/structures are to be used while defining OMAP on-chip module
 * data and their integration with other OMAP modules and Linux.
 */

#include <plat/omap_hwmod.h>

#include "omap_hwmod_common_data.h"

/**
 * struct omap_hwmod_sysc_type1 - TYPE1 sysconfig scheme.
 *
 * To be used by hwmod structure to specify the sysconfig offsets
 * if the device ip is compliant with the original PRCM protocol
 * defined for OMAP2420.
 */
struct omap_hwmod_sysc_fields omap_hwmod_sysc_type1 = {
	.midle_shift	= SYSC_TYPE1_MIDLEMODE_SHIFT,
	.clkact_shift	= SYSC_TYPE1_CLOCKACTIVITY_SHIFT,
	.sidle_shift	= SYSC_TYPE1_SIDLEMODE_SHIFT,
	.enwkup_shift	= SYSC_TYPE1_ENAWAKEUP_SHIFT,
	.srst_shift	= SYSC_TYPE1_SOFTRESET_SHIFT,
	.autoidle_shift	= SYSC_TYPE1_AUTOIDLE_SHIFT,
};

/**
 * struct omap_hwmod_sysc_type2 - TYPE2 sysconfig scheme.
 *
 * To be used by hwmod structure to specify the sysconfig offsets if the
 * device ip is compliant with the new PRCM protocol defined for new
 * OMAP4 IPs.
 */
struct omap_hwmod_sysc_fields omap_hwmod_sysc_type2 = {
	.midle_shift	= SYSC_TYPE2_MIDLEMODE_SHIFT,
	.sidle_shift	= SYSC_TYPE2_SIDLEMODE_SHIFT,
	.srst_shift	= SYSC_TYPE2_SOFTRESET_SHIFT,
};


/*
 * omap_hwmod class data
 */

struct omap_hwmod_class l3_hwmod_class = {
	.name = "l3"
};

struct omap_hwmod_class l4_hwmod_class = {
	.name = "l4"
};

struct omap_hwmod_class mpu_hwmod_class = {
	.name = "mpu"
};

