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

/* bnx2_fw.h: Broadcom NX2 network driver.
 *
 * Copyright (c) 2004, 2005, 2006, 2007 Broadcom Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.
 */

/* Initialized Values for the Completion Processor. */
static const struct cpu_reg cpu_reg_com = {
	.mode = BNX2_COM_CPU_MODE,
	.mode_value_halt = BNX2_COM_CPU_MODE_SOFT_HALT,
	.mode_value_sstep = BNX2_COM_CPU_MODE_STEP_ENA,
	.state = BNX2_COM_CPU_STATE,
	.state_value_clear = 0xffffff,
	.gpr0 = BNX2_COM_CPU_REG_FILE,
	.evmask = BNX2_COM_CPU_EVENT_MASK,
	.pc = BNX2_COM_CPU_PROGRAM_COUNTER,
	.inst = BNX2_COM_CPU_INSTRUCTION,
	.bp = BNX2_COM_CPU_HW_BREAKPOINT,
	.spad_base = BNX2_COM_SCRATCH,
	.mips_view_base = 0x8000000,
};

/* Initialized Values the Command Processor. */
static const struct cpu_reg cpu_reg_cp = {
	.mode = BNX2_CP_CPU_MODE,
	.mode_value_halt = BNX2_CP_CPU_MODE_SOFT_HALT,
	.mode_value_sstep = BNX2_CP_CPU_MODE_STEP_ENA,
	.state = BNX2_CP_CPU_STATE,
	.state_value_clear = 0xffffff,
	.gpr0 = BNX2_CP_CPU_REG_FILE,
	.evmask = BNX2_CP_CPU_EVENT_MASK,
	.pc = BNX2_CP_CPU_PROGRAM_COUNTER,
	.inst = BNX2_CP_CPU_INSTRUCTION,
	.bp = BNX2_CP_CPU_HW_BREAKPOINT,
	.spad_base = BNX2_CP_SCRATCH,
	.mips_view_base = 0x8000000,
};

/* Initialized Values for the RX Processor. */
static const struct cpu_reg cpu_reg_rxp = {
	.mode = BNX2_RXP_CPU_MODE,
	.mode_value_halt = BNX2_RXP_CPU_MODE_SOFT_HALT,
	.mode_value_sstep = BNX2_RXP_CPU_MODE_STEP_ENA,
	.state = BNX2_RXP_CPU_STATE,
	.state_value_clear = 0xffffff,
	.gpr0 = BNX2_RXP_CPU_REG_FILE,
	.evmask = BNX2_RXP_CPU_EVENT_MASK,
	.pc = BNX2_RXP_CPU_PROGRAM_COUNTER,
	.inst = BNX2_RXP_CPU_INSTRUCTION,
	.bp = BNX2_RXP_CPU_HW_BREAKPOINT,
	.spad_base = BNX2_RXP_SCRATCH,
	.mips_view_base = 0x8000000,
};

/* Initialized Values for the TX Patch-up Processor. */
static const struct cpu_reg cpu_reg_tpat = {
	.mode = BNX2_TPAT_CPU_MODE,
	.mode_value_halt = BNX2_TPAT_CPU_MODE_SOFT_HALT,
	.mode_value_sstep = BNX2_TPAT_CPU_MODE_STEP_ENA,
	.state = BNX2_TPAT_CPU_STATE,
	.state_value_clear = 0xffffff,
	.gpr0 = BNX2_TPAT_CPU_REG_FILE,
	.evmask = BNX2_TPAT_CPU_EVENT_MASK,
	.pc = BNX2_TPAT_CPU_PROGRAM_COUNTER,
	.inst = BNX2_TPAT_CPU_INSTRUCTION,
	.bp = BNX2_TPAT_CPU_HW_BREAKPOINT,
	.spad_base = BNX2_TPAT_SCRATCH,
	.mips_view_base = 0x8000000,
};

/* Initialized Values for the TX Processor. */
static const struct cpu_reg cpu_reg_txp = {
	.mode = BNX2_TXP_CPU_MODE,
	.mode_value_halt = BNX2_TXP_CPU_MODE_SOFT_HALT,
	.mode_value_sstep = BNX2_TXP_CPU_MODE_STEP_ENA,
	.state = BNX2_TXP_CPU_STATE,
	.state_value_clear = 0xffffff,
	.gpr0 = BNX2_TXP_CPU_REG_FILE,
	.evmask = BNX2_TXP_CPU_EVENT_MASK,
	.pc = BNX2_TXP_CPU_PROGRAM_COUNTER,
	.inst = BNX2_TXP_CPU_INSTRUCTION,
	.bp = BNX2_TXP_CPU_HW_BREAKPOINT,
	.spad_base = BNX2_TXP_SCRATCH,
	.mips_view_base = 0x8000000,
};
