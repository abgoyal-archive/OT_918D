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
 *	include/asm-mips/dec/kn02ba.h
 *
 *	DECstation 5000/1xx (3min or KN02-BA) definitions.
 *
 *	Copyright (C) 2002, 2003  Maciej W. Rozycki
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */
#ifndef __ASM_MIPS_DEC_KN02BA_H
#define __ASM_MIPS_DEC_KN02BA_H

#include <asm/dec/kn02xa.h>		/* For common definitions. */

/*
 * CPU interrupt bits.
 */
#define KN02BA_CPU_INR_HALT	6	/* HALT button */
#define KN02BA_CPU_INR_CASCADE	5	/* I/O ASIC cascade */
#define KN02BA_CPU_INR_TC2	4	/* TURBOchannel slot #2 */
#define KN02BA_CPU_INR_TC1	3	/* TURBOchannel slot #1 */
#define KN02BA_CPU_INR_TC0	2	/* TURBOchannel slot #0 */

/*
 * I/O ASIC interrupt bits.  Star marks denote non-IRQ status bits.
 */
#define KN02BA_IO_INR_RES_15	15	/* unused */
#define KN02BA_IO_INR_NVRAM	14	/* (*) NVRAM clear jumper */
#define KN02BA_IO_INR_RES_13	13	/* unused */
#define KN02BA_IO_INR_BUS	12	/* memory, I/O bus read/write errors */
#define KN02BA_IO_INR_RES_11	11	/* unused */
#define KN02BA_IO_INR_NRMOD	10	/* (*) NRMOD manufacturing jumper */
#define KN02BA_IO_INR_ASC	9	/* ASC (NCR53C94) SCSI */
#define KN02BA_IO_INR_LANCE	8	/* LANCE (Am7990) Ethernet */
#define KN02BA_IO_INR_SCC1	7	/* SCC (Z85C30) serial #1 */
#define KN02BA_IO_INR_SCC0	6	/* SCC (Z85C30) serial #0 */
#define KN02BA_IO_INR_RTC	5	/* DS1287 RTC */
#define KN02BA_IO_INR_PSU	4	/* power supply unit warning */
#define KN02BA_IO_INR_RES_3	3	/* unused */
#define KN02BA_IO_INR_ASC_DATA	2	/* SCSI data ready (for PIO) */
#define KN02BA_IO_INR_PBNC	1	/* ~HALT button debouncer */
#define KN02BA_IO_INR_PBNO	0	/* HALT button debouncer */


/*
 * Memory Error Register bits.
 */
#define KN02BA_MER_RES_27	(1<<27)		/* unused */

/*
 * Memory Size Register bits.
 */
#define KN02BA_MSR_RES_17	(0x3ff<<17)	/* unused */

/*
 * I/O ASIC System Support Register bits.
 */
#define KN02BA_IO_SSR_TXDIS1	(1<<14)		/* SCC1 transmit disable */
#define KN02BA_IO_SSR_TXDIS0	(1<<13)		/* SCC0 transmit disable */
#define KN02BA_IO_SSR_RES_12	(1<<12)		/* unused */

#define KN02BA_IO_SSR_LEDS	(0xff<<0)	/* ~diagnostic LEDs */

#endif /* __ASM_MIPS_DEC_KN02BA_H */
