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
 *  Copyright (C) 1999,2000 Arm Limited
 *  Copyright (C) 2000 Deep Blue Solutions Ltd
 *  Copyright (C) 2002 Shane Nay (shane@minirl.com)
 *  Copyright 2004-2005 Freescale Semiconductor, Inc. All Rights Reserved.
 *    - add MXC specific definitions
 *  Copyright 2006 Motorola, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <linux/mm.h>
#include <linux/init.h>
#include <mach/hardware.h>
#include <mach/common.h>
#include <asm/pgtable.h>
#include <asm/mach/map.h>

/*
 * This structure defines the MXC memory map.
 */
static struct map_desc mxc_io_desc[] __initdata = {
	{
		.virtual	= MXC91231_L2CC_BASE_ADDR_VIRT,
		.pfn		= __phys_to_pfn(MXC91231_L2CC_BASE_ADDR),
		.length		= MXC91231_L2CC_SIZE,
		.type		= MT_DEVICE,
	}, {
		.virtual	= MXC91231_X_MEMC_BASE_ADDR_VIRT,
		.pfn		= __phys_to_pfn(MXC91231_X_MEMC_BASE_ADDR),
		.length		= MXC91231_X_MEMC_SIZE,
		.type		= MT_DEVICE,
	}, {
		.virtual	= MXC91231_ROMP_BASE_ADDR_VIRT,
		.pfn		= __phys_to_pfn(MXC91231_ROMP_BASE_ADDR),
		.length		= MXC91231_ROMP_SIZE,
		.type		= MT_DEVICE,
	}, {
		.virtual	= MXC91231_AVIC_BASE_ADDR_VIRT,
		.pfn		= __phys_to_pfn(MXC91231_AVIC_BASE_ADDR),
		.length		= MXC91231_AVIC_SIZE,
		.type		= MT_DEVICE,
	}, {
		.virtual	= MXC91231_AIPS1_BASE_ADDR_VIRT,
		.pfn		= __phys_to_pfn(MXC91231_AIPS1_BASE_ADDR),
		.length		= MXC91231_AIPS1_SIZE,
		.type		= MT_DEVICE,
	}, {
		.virtual	= MXC91231_SPBA0_BASE_ADDR_VIRT,
		.pfn		= __phys_to_pfn(MXC91231_SPBA0_BASE_ADDR),
		.length		= MXC91231_SPBA0_SIZE,
		.type		= MT_DEVICE,
	}, {
		.virtual	= MXC91231_SPBA1_BASE_ADDR_VIRT,
		.pfn		= __phys_to_pfn(MXC91231_SPBA1_BASE_ADDR),
		.length		= MXC91231_SPBA1_SIZE,
		.type		= MT_DEVICE,
	}, {
		.virtual	= MXC91231_AIPS2_BASE_ADDR_VIRT,
		.pfn		= __phys_to_pfn(MXC91231_AIPS2_BASE_ADDR),
		.length		= MXC91231_AIPS2_SIZE,
		.type		= MT_DEVICE,
	},
};

/*
 * This function initializes the memory map. It is called during the
 * system startup to create static physical to virtual memory map for
 * the IO modules.
 */
void __init mxc91231_map_io(void)
{
	mxc_set_cpu_type(MXC_CPU_MXC91231);

	iotable_init(mxc_io_desc, ARRAY_SIZE(mxc_io_desc));
}

void __init mxc91231_init_irq(void)
{
	mxc_init_irq(MXC91231_IO_ADDRESS(MXC91231_AVIC_BASE_ADDR));
}
