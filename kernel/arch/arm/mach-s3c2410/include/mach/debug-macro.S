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

/* arch/arm/mach-s3c2410/include/mach/debug-macro.S
 *
 * Debugging macro include header
 *
 *  Copyright (C) 1994-1999 Russell King
 *  Copyright (C) 2005 Simtec Electronics
 *
 *  Moved from linux/arch/arm/kernel/debug.S by Ben Dooks
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <mach/map.h>
#include <mach/regs-gpio.h>
#include <plat/regs-serial.h>

#define S3C2410_UART1_OFF (0x4000)
#define SHIFT_2440TXF (14-9)

	.macro addruart, rx, tmp
		mrc	p15, 0, \rx, c1, c0
		tst	\rx, #1
		ldreq	\rx, = S3C24XX_PA_UART
		ldrne	\rx, = S3C24XX_VA_UART
#if CONFIG_DEBUG_S3C_UART != 0
		add	\rx, \rx, #(S3C2410_UART1_OFF * CONFIG_DEBUG_S3C_UART)
#endif
	.endm

	.macro fifo_full_s3c24xx rd, rx
		@ check for arm920 vs arm926. currently assume all arm926
		@ devices have an 64 byte FIFO identical to the s3c2440
		mrc	p15, 0, \rd, c0, c0
		and	\rd, \rd, #0xff0
		teq	\rd, #0x260
		beq	1004f
		mrc	p15, 0, \rd, c1, c0
		tst	\rd, #1
		addeq	\rd, \rx, #(S3C24XX_PA_GPIO - S3C24XX_PA_UART)
		addne	\rd, \rx, #(S3C24XX_VA_GPIO - S3C24XX_VA_UART)
		bic	\rd, \rd, #0xff000
		ldr	\rd, [ \rd, # S3C2410_GSTATUS1 - S3C2410_GPIOREG(0) ]
		and	\rd, \rd, #0x00ff0000
		teq	\rd, #0x00440000		@ is it 2440?
1004:
		ldr	\rd, [ \rx, # S3C2410_UFSTAT ]
		moveq	\rd, \rd, lsr #SHIFT_2440TXF
		tst	\rd, #S3C2410_UFSTAT_TXFULL
	.endm

	.macro  fifo_full_s3c2410 rd, rx
		ldr	\rd, [ \rx, # S3C2410_UFSTAT ]
		tst	\rd, #S3C2410_UFSTAT_TXFULL
	.endm

/* fifo level reading */

	.macro fifo_level_s3c24xx rd, rx
		@ check for arm920 vs arm926. currently assume all arm926
		@ devices have an 64 byte FIFO identical to the s3c2440
		mrc	p15, 0, \rd, c0, c0
		and	\rd, \rd, #0xff0
		teq	\rd, #0x260
		beq	10000f
		mrc	p15, 0, \rd, c1, c0
		tst	\rd, #1
		addeq	\rd, \rx, #(S3C24XX_PA_GPIO - S3C24XX_PA_UART)
		addne	\rd, \rx, #(S3C24XX_VA_GPIO - S3C24XX_VA_UART)
		bic	\rd, \rd, #0xff000
		ldr	\rd, [ \rd, # S3C2410_GSTATUS1 - S3C2410_GPIOREG(0) ]
		and	\rd, \rd, #0x00ff0000
		teq	\rd, #0x00440000		@ is it 2440?

10000:
		ldr	\rd, [ \rx, # S3C2410_UFSTAT ]
		andne	\rd, \rd, #S3C2410_UFSTAT_TXMASK
		andeq	\rd, \rd, #S3C2440_UFSTAT_TXMASK
	.endm

	.macro fifo_level_s3c2410 rd, rx
		ldr	\rd, [ \rx, # S3C2410_UFSTAT ]
		and	\rd, \rd, #S3C2410_UFSTAT_TXMASK
	.endm

/* Select the correct implementation depending on the configuration. The
 * S3C2440 will get selected by default, as these are the most widely
 * used variants of these
*/

#if defined(CONFIG_CPU_LLSERIAL_S3C2410_ONLY)
#define fifo_full  fifo_full_s3c2410
#define fifo_level fifo_level_s3c2410
#elif !defined(CONFIG_CPU_LLSERIAL_S3C2440_ONLY)
#define fifo_full  fifo_full_s3c24xx
#define fifo_level fifo_level_s3c24xx
#endif

/* include the reset of the code which will do the work */

#include <plat/debug-macro.S>
