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
 * TI Common Platform Interrupt Controller (cp_intc) definitions
 *
 * Author: Steve Chen <schen@mvista.com>
 * Copyright (C) 2008-2009, MontaVista Software, Inc. <source@mvista.com>
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2. This program is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 */
#ifndef __ASM_HARDWARE_CP_INTC_H
#define __ASM_HARDWARE_CP_INTC_H

#define CP_INTC_REV			0x00
#define CP_INTC_CTRL			0x04
#define CP_INTC_HOST_CTRL		0x0C
#define CP_INTC_GLOBAL_ENABLE		0x10
#define CP_INTC_GLOBAL_NESTING_LEVEL	0x1C
#define CP_INTC_SYS_STAT_IDX_SET	0x20
#define CP_INTC_SYS_STAT_IDX_CLR	0x24
#define CP_INTC_SYS_ENABLE_IDX_SET	0x28
#define CP_INTC_SYS_ENABLE_IDX_CLR	0x2C
#define CP_INTC_GLOBAL_WAKEUP_ENABLE	0x30
#define CP_INTC_HOST_ENABLE_IDX_SET	0x34
#define CP_INTC_HOST_ENABLE_IDX_CLR	0x38
#define CP_INTC_PACING_PRESCALE 	0x40
#define CP_INTC_VECTOR_BASE		0x50
#define CP_INTC_VECTOR_SIZE		0x54
#define CP_INTC_VECTOR_NULL		0x58
#define CP_INTC_PRIO_IDX		0x80
#define CP_INTC_PRIO_VECTOR		0x84
#define CP_INTC_SECURE_ENABLE		0x90
#define CP_INTC_SECURE_PRIO_IDX 	0x94
#define CP_INTC_PACING_PARAM(n) 	(0x0100 + (n << 4))
#define CP_INTC_PACING_DEC(n)		(0x0104 + (n << 4))
#define CP_INTC_PACING_MAP(n)		(0x0108 + (n << 4))
#define CP_INTC_SYS_RAW_STAT(n) 	(0x0200 + (n << 2))
#define CP_INTC_SYS_STAT_CLR(n) 	(0x0280 + (n << 2))
#define CP_INTC_SYS_ENABLE_SET(n)	(0x0300 + (n << 2))
#define CP_INTC_SYS_ENABLE_CLR(n)	(0x0380 + (n << 2))
#define CP_INTC_CHAN_MAP(n)		(0x0400 + (n << 2))
#define CP_INTC_HOST_MAP(n)		(0x0800 + (n << 2))
#define CP_INTC_HOST_PRIO_IDX(n)	(0x0900 + (n << 2))
#define CP_INTC_SYS_POLARITY(n) 	(0x0D00 + (n << 2))
#define CP_INTC_SYS_TYPE(n)		(0x0D80 + (n << 2))
#define CP_INTC_WAKEUP_ENABLE(n)	(0x0E00 + (n << 2))
#define CP_INTC_DEBUG_SELECT(n) 	(0x0F00 + (n << 2))
#define CP_INTC_SYS_SECURE_ENABLE(n)	(0x1000 + (n << 2))
#define CP_INTC_HOST_NESTING_LEVEL(n)	(0x1100 + (n << 2))
#define CP_INTC_HOST_ENABLE(n)		(0x1500 + (n << 2))
#define CP_INTC_HOST_PRIO_VECTOR(n)	(0x1600 + (n << 2))
#define CP_INTC_VECTOR_ADDR(n)		(0x2000 + (n << 2))

void __init cp_intc_init(void);

#endif	/* __ASM_HARDWARE_CP_INTC_H */
