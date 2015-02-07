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
 *				asic_reg_map.h
 *
 * A macro-enclosed list of the elements for the register_map structure for
 * use in defining and manipulating the structure.
 *
 * Copyright (C) 2009  Cisco Systems, Inc.
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

REGISTER_MAP_ELEMENT(eic_slow0_strt_add)
REGISTER_MAP_ELEMENT(eic_cfg_bits)
REGISTER_MAP_ELEMENT(eic_ready_status)
REGISTER_MAP_ELEMENT(chipver3)
REGISTER_MAP_ELEMENT(chipver2)
REGISTER_MAP_ELEMENT(chipver1)
REGISTER_MAP_ELEMENT(chipver0)
REGISTER_MAP_ELEMENT(uart1_intstat)
REGISTER_MAP_ELEMENT(uart1_inten)
REGISTER_MAP_ELEMENT(uart1_config1)
REGISTER_MAP_ELEMENT(uart1_config2)
REGISTER_MAP_ELEMENT(uart1_divisorhi)
REGISTER_MAP_ELEMENT(uart1_divisorlo)
REGISTER_MAP_ELEMENT(uart1_data)
REGISTER_MAP_ELEMENT(uart1_status)
REGISTER_MAP_ELEMENT(int_stat_3)
REGISTER_MAP_ELEMENT(int_stat_2)
REGISTER_MAP_ELEMENT(int_stat_1)
REGISTER_MAP_ELEMENT(int_stat_0)
REGISTER_MAP_ELEMENT(int_config)
REGISTER_MAP_ELEMENT(int_int_scan)
REGISTER_MAP_ELEMENT(ien_int_3)
REGISTER_MAP_ELEMENT(ien_int_2)
REGISTER_MAP_ELEMENT(ien_int_1)
REGISTER_MAP_ELEMENT(ien_int_0)
REGISTER_MAP_ELEMENT(int_level_3_3)
REGISTER_MAP_ELEMENT(int_level_3_2)
REGISTER_MAP_ELEMENT(int_level_3_1)
REGISTER_MAP_ELEMENT(int_level_3_0)
REGISTER_MAP_ELEMENT(int_level_2_3)
REGISTER_MAP_ELEMENT(int_level_2_2)
REGISTER_MAP_ELEMENT(int_level_2_1)
REGISTER_MAP_ELEMENT(int_level_2_0)
REGISTER_MAP_ELEMENT(int_level_1_3)
REGISTER_MAP_ELEMENT(int_level_1_2)
REGISTER_MAP_ELEMENT(int_level_1_1)
REGISTER_MAP_ELEMENT(int_level_1_0)
REGISTER_MAP_ELEMENT(int_level_0_3)
REGISTER_MAP_ELEMENT(int_level_0_2)
REGISTER_MAP_ELEMENT(int_level_0_1)
REGISTER_MAP_ELEMENT(int_level_0_0)
REGISTER_MAP_ELEMENT(int_docsis_en)
REGISTER_MAP_ELEMENT(mips_pll_setup)
REGISTER_MAP_ELEMENT(usb_fs)
REGISTER_MAP_ELEMENT(test_bus)
REGISTER_MAP_ELEMENT(crt_spare)
REGISTER_MAP_ELEMENT(usb2_ohci_int_mask)
REGISTER_MAP_ELEMENT(usb2_strap)
REGISTER_MAP_ELEMENT(ehci_hcapbase)
REGISTER_MAP_ELEMENT(ohci_hc_revision)
REGISTER_MAP_ELEMENT(bcm1_bs_lmi_steer)
REGISTER_MAP_ELEMENT(usb2_control)
REGISTER_MAP_ELEMENT(usb2_stbus_obc)
REGISTER_MAP_ELEMENT(usb2_stbus_mess_size)
REGISTER_MAP_ELEMENT(usb2_stbus_chunk_size)
REGISTER_MAP_ELEMENT(pcie_regs)
REGISTER_MAP_ELEMENT(tim_ch)
REGISTER_MAP_ELEMENT(tim_cl)
REGISTER_MAP_ELEMENT(gpio_dout)
REGISTER_MAP_ELEMENT(gpio_din)
REGISTER_MAP_ELEMENT(gpio_dir)
REGISTER_MAP_ELEMENT(watchdog)
REGISTER_MAP_ELEMENT(front_panel)
REGISTER_MAP_ELEMENT(misc_clk_ctl1)
REGISTER_MAP_ELEMENT(misc_clk_ctl2)
REGISTER_MAP_ELEMENT(crt_ext_ctl)
REGISTER_MAP_ELEMENT(register_maps)
