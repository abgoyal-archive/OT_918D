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
 * Copyright 1998-2008 VIA Technologies, Inc. All Rights Reserved.
 * Copyright 2001-2008 S3 Graphics, Inc. All Rights Reserved.
 * Copyright 2010 Florian Tobias Schandinat <FlorianSchandinat@gmx.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTIES OR REPRESENTATIONS; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
/*
 * basic modesetting functions
 */

#include <linux/kernel.h>
#include <linux/via-core.h>
#include "via_modesetting.h"
#include "share.h"
#include "debug.h"

void via_set_primary_address(u32 addr)
{
	DEBUG_MSG(KERN_DEBUG "via_set_primary_address(0x%08X)\n", addr);
	via_write_reg(VIACR, 0x0D, addr & 0xFF);
	via_write_reg(VIACR, 0x0C, (addr >> 8) & 0xFF);
	via_write_reg(VIACR, 0x34, (addr >> 16) & 0xFF);
	via_write_reg_mask(VIACR, 0x48, (addr >> 24) & 0x1F, 0x1F);
}

void via_set_secondary_address(u32 addr)
{
	DEBUG_MSG(KERN_DEBUG "via_set_secondary_address(0x%08X)\n", addr);
	/* secondary display supports only quadword aligned memory */
	via_write_reg_mask(VIACR, 0x62, (addr >> 2) & 0xFE, 0xFE);
	via_write_reg(VIACR, 0x63, (addr >> 10) & 0xFF);
	via_write_reg(VIACR, 0x64, (addr >> 18) & 0xFF);
	via_write_reg_mask(VIACR, 0xA3, (addr >> 26) & 0x07, 0x07);
}

void via_set_primary_pitch(u32 pitch)
{
	DEBUG_MSG(KERN_DEBUG "via_set_primary_pitch(0x%08X)\n", pitch);
	/* spec does not say that first adapter skips 3 bits but old
	 * code did it and seems to be reasonable in analogy to 2nd adapter
	 */
	pitch = pitch >> 3;
	via_write_reg(VIACR, 0x13, pitch & 0xFF);
	via_write_reg_mask(VIACR, 0x35, (pitch >> (8 - 5)) & 0xE0, 0xE0);
}

void via_set_secondary_pitch(u32 pitch)
{
	DEBUG_MSG(KERN_DEBUG "via_set_secondary_pitch(0x%08X)\n", pitch);
	pitch = pitch >> 3;
	via_write_reg(VIACR, 0x66, pitch & 0xFF);
	via_write_reg_mask(VIACR, 0x67, (pitch >> 8) & 0x03, 0x03);
	via_write_reg_mask(VIACR, 0x71, (pitch >> (10 - 7)) & 0x80, 0x80);
}

void via_set_primary_color_depth(u8 depth)
{
	u8 value;

	DEBUG_MSG(KERN_DEBUG "via_set_primary_color_depth(%d)\n", depth);
	switch (depth) {
	case 8:
		value = 0x00;
		break;
	case 15:
		value = 0x04;
		break;
	case 16:
		value = 0x14;
		break;
	case 24:
		value = 0x0C;
		break;
	case 30:
		value = 0x08;
		break;
	default:
		printk(KERN_WARNING "via_set_primary_color_depth: "
			"Unsupported depth: %d\n", depth);
		return;
	}

	via_write_reg_mask(VIASR, 0x15, value, 0x1C);
}

void via_set_secondary_color_depth(u8 depth)
{
	u8 value;

	DEBUG_MSG(KERN_DEBUG "via_set_secondary_color_depth(%d)\n", depth);
	switch (depth) {
	case 8:
		value = 0x00;
		break;
	case 16:
		value = 0x40;
		break;
	case 24:
		value = 0xC0;
		break;
	case 30:
		value = 0x80;
		break;
	default:
		printk(KERN_WARNING "via_set_secondary_color_depth: "
			"Unsupported depth: %d\n", depth);
		return;
	}

	via_write_reg_mask(VIACR, 0x67, value, 0xC0);
}
