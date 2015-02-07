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
 * Locations of devices in the Zeus ASIC
 *
 * Copyright (C) 2005-2009 Scientific-Atlanta, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * Author:       Ken Eppinett
 *               David Schleef <ds@schleef.org>
 *
 * Description:  Defines the platform resources for the SA settop.
 */

#include <linux/init.h>
#include <asm/mach-powertv/asic.h>

#define ZEUS_ADDR(x)	(ZEUS_IO_BASE + (x))

const struct register_map zeus_register_map __initdata = {
	.eic_slow0_strt_add = {.phys = ZEUS_ADDR(0x000000)},
	.eic_cfg_bits = {.phys = ZEUS_ADDR(0x000038)},
	.eic_ready_status = {.phys = ZEUS_ADDR(0x00004c)},

	.chipver3 = {.phys = ZEUS_ADDR(0x280800)},
	.chipver2 = {.phys = ZEUS_ADDR(0x280804)},
	.chipver1 = {.phys = ZEUS_ADDR(0x280808)},
	.chipver0 = {.phys = ZEUS_ADDR(0x28080c)},

	/* The registers of IRBlaster */
	.uart1_intstat = {.phys = ZEUS_ADDR(0x281800)},
	.uart1_inten = {.phys = ZEUS_ADDR(0x281804)},
	.uart1_config1 = {.phys = ZEUS_ADDR(0x281808)},
	.uart1_config2 = {.phys = ZEUS_ADDR(0x28180C)},
	.uart1_divisorhi = {.phys = ZEUS_ADDR(0x281810)},
	.uart1_divisorlo = {.phys = ZEUS_ADDR(0x281814)},
	.uart1_data = {.phys = ZEUS_ADDR(0x281818)},
	.uart1_status = {.phys = ZEUS_ADDR(0x28181C)},

	.int_stat_3 = {.phys = ZEUS_ADDR(0x282800)},
	.int_stat_2 = {.phys = ZEUS_ADDR(0x282804)},
	.int_stat_1 = {.phys = ZEUS_ADDR(0x282808)},
	.int_stat_0 = {.phys = ZEUS_ADDR(0x28280c)},
	.int_config = {.phys = ZEUS_ADDR(0x282810)},
	.int_int_scan = {.phys = ZEUS_ADDR(0x282818)},
	.ien_int_3 = {.phys = ZEUS_ADDR(0x282830)},
	.ien_int_2 = {.phys = ZEUS_ADDR(0x282834)},
	.ien_int_1 = {.phys = ZEUS_ADDR(0x282838)},
	.ien_int_0 = {.phys = ZEUS_ADDR(0x28283c)},
	.int_level_3_3 = {.phys = ZEUS_ADDR(0x282880)},
	.int_level_3_2 = {.phys = ZEUS_ADDR(0x282884)},
	.int_level_3_1 = {.phys = ZEUS_ADDR(0x282888)},
	.int_level_3_0 = {.phys = ZEUS_ADDR(0x28288c)},
	.int_level_2_3 = {.phys = ZEUS_ADDR(0x282890)},
	.int_level_2_2 = {.phys = ZEUS_ADDR(0x282894)},
	.int_level_2_1 = {.phys = ZEUS_ADDR(0x282898)},
	.int_level_2_0 = {.phys = ZEUS_ADDR(0x28289c)},
	.int_level_1_3 = {.phys = ZEUS_ADDR(0x2828a0)},
	.int_level_1_2 = {.phys = ZEUS_ADDR(0x2828a4)},
	.int_level_1_1 = {.phys = ZEUS_ADDR(0x2828a8)},
	.int_level_1_0 = {.phys = ZEUS_ADDR(0x2828ac)},
	.int_level_0_3 = {.phys = ZEUS_ADDR(0x2828b0)},
	.int_level_0_2 = {.phys = ZEUS_ADDR(0x2828b4)},
	.int_level_0_1 = {.phys = ZEUS_ADDR(0x2828b8)},
	.int_level_0_0 = {.phys = ZEUS_ADDR(0x2828bc)},
	.int_docsis_en = {.phys = ZEUS_ADDR(0x2828F4)},

	.mips_pll_setup = {.phys = ZEUS_ADDR(0x1a0000)},
	.usb_fs = {.phys = ZEUS_ADDR(0x1a0018)},
	.test_bus = {.phys = ZEUS_ADDR(0x1a0238)},
	.crt_spare = {.phys = ZEUS_ADDR(0x1a0090)},
	.usb2_ohci_int_mask = {.phys = ZEUS_ADDR(0x1e000c)},
	.usb2_strap = {.phys = ZEUS_ADDR(0x1e0014)},
	.ehci_hcapbase = {.phys = ZEUS_ADDR(0x1FFE00)},
	.ohci_hc_revision = {.phys = ZEUS_ADDR(0x1FFC00)},
	.bcm1_bs_lmi_steer = {.phys = ZEUS_ADDR(0x2C0008)},
	.usb2_control = {.phys = ZEUS_ADDR(0x2c01a0)},
	.usb2_stbus_obc = {.phys = ZEUS_ADDR(0x1FFF00)},
	.usb2_stbus_mess_size = {.phys = ZEUS_ADDR(0x1FFF04)},
	.usb2_stbus_chunk_size = {.phys = ZEUS_ADDR(0x1FFF08)},

	.pcie_regs = {.phys = ZEUS_ADDR(0x200000)},
	.tim_ch = {.phys = ZEUS_ADDR(0x282C10)},
	.tim_cl = {.phys = ZEUS_ADDR(0x282C14)},
	.gpio_dout = {.phys = ZEUS_ADDR(0x282c20)},
	.gpio_din = {.phys = ZEUS_ADDR(0x282c24)},
	.gpio_dir = {.phys = ZEUS_ADDR(0x282c2C)},
	.watchdog = {.phys = ZEUS_ADDR(0x282c30)},
	.front_panel = {.phys = ZEUS_ADDR(0x283800)},
};