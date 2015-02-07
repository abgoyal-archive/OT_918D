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
 * Definitions for memory preallocations
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
 */

#ifndef _ARCH_MIPS_POWERTV_ASIC_PREALLOC_H
#define _ARCH_MIPS_POWERTV_ASIC_PREALLOC_H

#define KIBIBYTE(n) ((n) * 1024)    /* Number of kibibytes */
#define MEBIBYTE(n) ((n) * KIBIBYTE(1024)) /* Number of mebibytes */

/* "struct resource" array element definition */
#define PREALLOC(NAME, START, END, FLAGS) {	\
		.name = (NAME),			\
		.start = (START),		\
		.end = (END),			\
		.flags = (FLAGS)		\
	},

/* Individual resources in the preallocated resource arrays are defined using
 *  macros.  These macros are conditionally defined based on their
 *  corresponding kernel configuration flag:
 *    - CONFIG_PREALLOC_NORMAL: preallocate resources for a normal settop box
 *    - CONFIG_PREALLOC_TFTP: preallocate the TFTP download resource
 *    - CONFIG_PREALLOC_DOCSIS: preallocate the DOCSIS resource
 *    - CONFIG_PREALLOC_PMEM: reserve space for persistent memory
 */
#ifdef CONFIG_PREALLOC_NORMAL
#define PREALLOC_NORMAL(name, start, end, flags) \
   PREALLOC(name, start, end, flags)
#else
#define PREALLOC_NORMAL(name, start, end, flags)
#endif

#ifdef CONFIG_PREALLOC_TFTP
#define PREALLOC_TFTP(name, start, end, flags) \
   PREALLOC(name, start, end, flags)
#else
#define PREALLOC_TFTP(name, start, end, flags)
#endif

#ifdef CONFIG_PREALLOC_DOCSIS
#define PREALLOC_DOCSIS(name, start, end, flags) \
   PREALLOC(name, start, end, flags)
#else
#define PREALLOC_DOCSIS(name, start, end, flags)
#endif

#ifdef CONFIG_PREALLOC_PMEM
#define PREALLOC_PMEM(name, start, end, flags) \
   PREALLOC(name, start, end, flags)
#else
#define PREALLOC_PMEM(name, start, end, flags)
#endif
#endif
