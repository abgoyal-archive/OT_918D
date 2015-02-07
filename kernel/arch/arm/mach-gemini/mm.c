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
 *  Static mappings for Gemini
 *
 *  Copyright (C) 2001-2006 Storlink, Corp.
 *  Copyright (C) 2008-2009 Paulius Zaleckas <paulius.zaleckas@teltonika.lt>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include <linux/mm.h>
#include <linux/init.h>

#include <asm/mach/map.h>

#include <mach/hardware.h>

/* Page table mapping for I/O region */
static struct map_desc gemini_io_desc[] __initdata = {
	{
		.virtual	= IO_ADDRESS(GEMINI_GLOBAL_BASE),
		.pfn		=__phys_to_pfn(GEMINI_GLOBAL_BASE),
		.length		= SZ_512K,
		.type 		= MT_DEVICE,
	}, {
		.virtual	= IO_ADDRESS(GEMINI_UART_BASE),
		.pfn		= __phys_to_pfn(GEMINI_UART_BASE),
		.length		= SZ_512K,
		.type 		= MT_DEVICE,
	}, {
		.virtual	= IO_ADDRESS(GEMINI_TIMER_BASE),
		.pfn		= __phys_to_pfn(GEMINI_TIMER_BASE),
		.length		= SZ_512K,
		.type 		= MT_DEVICE,
	}, {
		.virtual	= IO_ADDRESS(GEMINI_INTERRUPT_BASE),
		.pfn		= __phys_to_pfn(GEMINI_INTERRUPT_BASE),
		.length		= SZ_512K,
		.type 		= MT_DEVICE,
	}, {
		.virtual	= IO_ADDRESS(GEMINI_POWER_CTRL_BASE),
		.pfn		= __phys_to_pfn(GEMINI_POWER_CTRL_BASE),
		.length		= SZ_512K,
		.type 		= MT_DEVICE,
	}, {
		.virtual	= IO_ADDRESS(GEMINI_GPIO_BASE(0)),
		.pfn		= __phys_to_pfn(GEMINI_GPIO_BASE(0)),
		.length		= SZ_512K,
		.type 		= MT_DEVICE,
	}, {
		.virtual	= IO_ADDRESS(GEMINI_GPIO_BASE(1)),
		.pfn		= __phys_to_pfn(GEMINI_GPIO_BASE(1)),
		.length		= SZ_512K,
		.type 		= MT_DEVICE,
	}, {
		.virtual	= IO_ADDRESS(GEMINI_GPIO_BASE(2)),
		.pfn		= __phys_to_pfn(GEMINI_GPIO_BASE(2)),
		.length		= SZ_512K,
		.type 		= MT_DEVICE,
	}, {
		.virtual	= IO_ADDRESS(GEMINI_FLASH_CTRL_BASE),
		.pfn		= __phys_to_pfn(GEMINI_FLASH_CTRL_BASE),
		.length		= SZ_512K,
		.type 		= MT_DEVICE,
	}, {
		.virtual	= IO_ADDRESS(GEMINI_DRAM_CTRL_BASE),
		.pfn		= __phys_to_pfn(GEMINI_DRAM_CTRL_BASE),
		.length		= SZ_512K,
		.type 		= MT_DEVICE,
	}, {
		.virtual	= IO_ADDRESS(GEMINI_GENERAL_DMA_BASE),
		.pfn		= __phys_to_pfn(GEMINI_GENERAL_DMA_BASE),
		.length		= SZ_512K,
		.type 		= MT_DEVICE,
	},
};

void __init gemini_map_io(void)
{
	iotable_init(gemini_io_desc, ARRAY_SIZE(gemini_io_desc));
}
