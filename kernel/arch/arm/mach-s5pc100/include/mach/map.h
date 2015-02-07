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

/* linux/arch/arm/mach-s5pc100/include/mach/map.h
 *
 * Copyright 2009 Samsung Electronics Co.
 *	Byungho Min <bhmin@samsung.com>
 *
 * S5PC100 - Memory map definitions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef __ASM_ARCH_MAP_H
#define __ASM_ARCH_MAP_H __FILE__

#include <plat/map-base.h>
#include <plat/map-s5p.h>

/*
 * map-base.h has already defined virtual memory address
 * S3C_VA_IRQ		S3C_ADDR(0x00000000)	irq controller(s)
 * S3C_VA_SYS		S3C_ADDR(0x00100000)	system control
 * S3C_VA_MEM		S3C_ADDR(0x00200000)	system control (not used)
 * S3C_VA_TIMER		S3C_ADDR(0x00300000)	timer block
 * S3C_VA_WATCHDOG	S3C_ADDR(0x00400000)	watchdog
 * S3C_VA_UART		S3C_ADDR(0x01000000)	UART
 *
 * S5PC100 specific virtual memory address can be defined here
 * S5PC1XX_VA_GPIO	S3C_ADDR(0x00500000)	GPIO
 *
 */

#define S5PC100_PA_ONENAND_BUF	(0xB0000000)
#define S5PC100_SZ_ONENAND_BUF	(SZ_256M - SZ_32M)

/* Chip ID */

#define S5PC100_PA_CHIPID	(0xE0000000)
#define S5P_PA_CHIPID		S5PC100_PA_CHIPID

#define S5PC100_PA_SYSCON	(0xE0100000)
#define S5P_PA_SYSCON		S5PC100_PA_SYSCON

#define S5PC100_PA_OTHERS	(0xE0200000)
#define S5PC100_VA_OTHERS	(S3C_VA_SYS + 0x10000)

#define S5P_PA_GPIO		(0xE0300000)
#define S5PC1XX_VA_GPIO		S3C_ADDR(0x00500000)

/* Interrupt */
#define S5PC100_PA_VIC		(0xE4000000)
#define S5PC100_VA_VIC		S3C_VA_IRQ
#define S5PC100_PA_VIC_OFFSET	0x100000
#define S5PC100_VA_VIC_OFFSET	0x10000
#define S5PC1XX_PA_VIC(x)	(S5PC100_PA_VIC + ((x) * S5PC100_PA_VIC_OFFSET))
#define S5PC1XX_VA_VIC(x)	(S5PC100_VA_VIC + ((x) * S5PC100_VA_VIC_OFFSET))
#define S5P_PA_VIC0		S5PC1XX_PA_VIC(0)
#define S5P_PA_VIC1		S5PC1XX_PA_VIC(1)
#define S5P_PA_VIC2		S5PC1XX_PA_VIC(2)


#define S5PC100_PA_ONENAND	(0xE7100000)

/* DMA */
#define S5PC100_PA_MDMA		(0xE8100000)
#define S5PC100_PA_PDMA0	(0xE9000000)
#define S5PC100_PA_PDMA1	(0xE9200000)

/* Timer */
#define S5PC100_PA_TIMER	(0xEA000000)
#define S5P_PA_TIMER		S5PC100_PA_TIMER

#define S5PC100_PA_SYSTIMER	(0xEA100000)

#define S5PC100_PA_UART		(0xEC000000)

#define S5P_PA_UART0		(S5PC100_PA_UART + 0x0)
#define S5P_PA_UART1		(S5PC100_PA_UART + 0x400)
#define S5P_PA_UART2		(S5PC100_PA_UART + 0x800)
#define S5P_PA_UART3		(S5PC100_PA_UART + 0xC00)
#define S5P_SZ_UART		SZ_256

#define S5PC100_PA_IIC0		(0xEC100000)
#define S5PC100_PA_IIC1		(0xEC200000)

/* SPI */
#define S5PC100_PA_SPI0		0xEC300000
#define S5PC100_PA_SPI1		0xEC400000
#define S5PC100_PA_SPI2		0xEC500000

/* USB HS OTG */
#define S5PC100_PA_USB_HSOTG	(0xED200000)
#define S5PC100_PA_USB_HSPHY	(0xED300000)

#define S5PC100_PA_FB		(0xEE000000)

#define S5PC100_PA_I2S0		(0xF2000000)
#define S5PC100_PA_I2S1		(0xF2100000)
#define S5PC100_PA_I2S2		(0xF2200000)

#define S5PC100_PA_AC97		0xF2300000

/* PCM */
#define S5PC100_PA_PCM0		0xF2400000
#define S5PC100_PA_PCM1		0xF2500000

/* KEYPAD */
#define S5PC100_PA_KEYPAD	(0xF3100000)

#define S5PC100_PA_HSMMC(x)	(0xED800000 + ((x) * 0x100000))

#define S5PC100_PA_SDRAM	(0x20000000)
#define S5P_PA_SDRAM		S5PC100_PA_SDRAM

/* compatibiltiy defines. */
#define S3C_PA_UART		S5PC100_PA_UART
#define S3C_PA_IIC		S5PC100_PA_IIC0
#define S3C_PA_IIC1		S5PC100_PA_IIC1
#define S3C_PA_FB		S5PC100_PA_FB
#define S3C_PA_G2D		S5PC100_PA_G2D
#define S3C_PA_G3D		S5PC100_PA_G3D
#define S3C_PA_JPEG		S5PC100_PA_JPEG
#define S3C_PA_ROTATOR		S5PC100_PA_ROTATOR
#define S5P_VA_VIC0		S5PC1XX_VA_VIC(0)
#define S5P_VA_VIC1		S5PC1XX_VA_VIC(1)
#define S5P_VA_VIC2		S5PC1XX_VA_VIC(2)
#define S3C_PA_USB_HSOTG	S5PC100_PA_USB_HSOTG
#define S3C_PA_USB_HSPHY	S5PC100_PA_USB_HSPHY
#define S3C_PA_HSMMC0		S5PC100_PA_HSMMC(0)
#define S3C_PA_HSMMC1		S5PC100_PA_HSMMC(1)
#define S3C_PA_HSMMC2		S5PC100_PA_HSMMC(2)
#define S3C_PA_KEYPAD		S5PC100_PA_KEYPAD
#define S3C_PA_TSADC		S5PC100_PA_TSADC
#define S3C_PA_ONENAND		S5PC100_PA_ONENAND
#define S3C_PA_ONENAND_BUF	S5PC100_PA_ONENAND_BUF
#define S3C_SZ_ONENAND_BUF	S5PC100_SZ_ONENAND_BUF

#endif /* __ASM_ARCH_C100_MAP_H */