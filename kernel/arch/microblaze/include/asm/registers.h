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
 * Copyright (C) 2008-2009 Michal Simek <monstr@monstr.eu>
 * Copyright (C) 2008-2009 PetaLogix
 * Copyright (C) 2006 Atmark Techno, Inc.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#ifndef _ASM_MICROBLAZE_REGISTERS_H
#define _ASM_MICROBLAZE_REGISTERS_H

#define MSR_BE	(1<<0) /* 0x001 */
#define MSR_IE	(1<<1) /* 0x002 */
#define MSR_C	(1<<2) /* 0x004 */
#define MSR_BIP	(1<<3) /* 0x008 */
#define MSR_FSL	(1<<4) /* 0x010 */
#define MSR_ICE	(1<<5) /* 0x020 */
#define MSR_DZ	(1<<6) /* 0x040 */
#define MSR_DCE	(1<<7) /* 0x080 */
#define MSR_EE	(1<<8) /* 0x100 */
#define MSR_EIP	(1<<9) /* 0x200 */
#define MSR_CC	(1<<31)

/* Floating Point Status Register (FSR) Bits */
#define FSR_IO		(1<<4) /* Invalid operation */
#define FSR_DZ		(1<<3) /* Divide-by-zero */
#define FSR_OF		(1<<2) /* Overflow */
#define FSR_UF		(1<<1) /* Underflow */
#define FSR_DO		(1<<0) /* Denormalized operand error */

# ifdef CONFIG_MMU
/* Machine State Register (MSR) Fields */
# define MSR_UM		(1<<11) /* User Mode */
# define MSR_UMS	(1<<12) /* User Mode Save */
# define MSR_VM		(1<<13) /* Virtual Mode */
# define MSR_VMS	(1<<14) /* Virtual Mode Save */

# define MSR_KERNEL	(MSR_EE | MSR_VM)
/* # define MSR_USER	(MSR_KERNEL | MSR_UM | MSR_IE) */
# define MSR_KERNEL_VMS	(MSR_EE | MSR_VMS)
/* # define MSR_USER_VMS	(MSR_KERNEL_VMS | MSR_UMS | MSR_IE) */

/* Exception State Register (ESR) Fields */
# define	  ESR_DIZ	(1<<11) /* Zone Protection */
# define	  ESR_S		(1<<10) /* Store instruction */

# endif /* CONFIG_MMU */
#endif /* _ASM_MICROBLAZE_REGISTERS_H */
