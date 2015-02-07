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
 * Pb1550 board platform device registration
 *
 * Copyright (C) 2009 Manuel Lauss
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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <linux/init.h>

#include <asm/mach-au1x00/au1000.h>
#include <asm/mach-pb1x00/pb1550.h>
#include <asm/mach-db1x00/bcsr.h>

#include "../platform.h"

static int __init pb1550_dev_init(void)
{
	int swapped;

	/* Pb1550, like all others, also has statuschange irqs; however they're
	* wired up on one of the Au1550's shared GPIO201_205 line, which also
	* services the PCMCIA card interrupts.  So we ignore statuschange and
	* use the GPIO201_205 exclusively for card interrupts, since a) pcmcia
	* drivers are used to shared irqs and b) statuschange isn't really use-
	* ful anyway.
	*/
	db1x_register_pcmcia_socket(PCMCIA_ATTR_PHYS_ADDR,
				    PCMCIA_ATTR_PHYS_ADDR + 0x000400000 - 1,
				    PCMCIA_MEM_PHYS_ADDR,
				    PCMCIA_MEM_PHYS_ADDR  + 0x000400000 - 1,
				    PCMCIA_IO_PHYS_ADDR,
				    PCMCIA_IO_PHYS_ADDR   + 0x000010000 - 1,
				    AU1550_GPIO201_205_INT,
				    AU1550_GPIO0_INT,
				    0,
				    0,
				    0);

	db1x_register_pcmcia_socket(PCMCIA_ATTR_PHYS_ADDR + 0x008000000,
				    PCMCIA_ATTR_PHYS_ADDR + 0x008400000 - 1,
				    PCMCIA_MEM_PHYS_ADDR  + 0x008000000,
				    PCMCIA_MEM_PHYS_ADDR  + 0x008400000 - 1,
				    PCMCIA_IO_PHYS_ADDR   + 0x008000000,
				    PCMCIA_IO_PHYS_ADDR   + 0x008010000 - 1,
				    AU1550_GPIO201_205_INT,
				    AU1550_GPIO1_INT,
				    0,
				    0,
				    1);

	swapped = bcsr_read(BCSR_STATUS) & BCSR_STATUS_PB1550_SWAPBOOT;
	db1x_register_norflash(128 * 1024 * 1024, 4, swapped);

	return 0;
}
device_initcall(pb1550_dev_init);