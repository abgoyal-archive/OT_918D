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

/* linux/arch/arm/mach-s5pc100/include/mach/irqs.h
 *
 * Copyright 2009 Samsung Electronics Co.
 *      Byungho Min <bhmin@samsung.com>
 *
 * S5PC100 - IRQ definitions
 */

#ifndef __ASM_ARCH_IRQS_H
#define __ASM_ARCH_IRQS_H __FILE__

#include <plat/irqs.h>

/* VIC0: system, DMA, timer */
#define IRQ_EINT16_31		S5P_IRQ_VIC0(16)
#define IRQ_BATF		S5P_IRQ_VIC0(17)
#define IRQ_MDMA		S5P_IRQ_VIC0(18)
#define IRQ_PDMA0		S5P_IRQ_VIC0(19)
#define IRQ_PDMA1		S5P_IRQ_VIC0(20)
#define IRQ_TIMER0_VIC		S5P_IRQ_VIC0(21)
#define IRQ_TIMER1_VIC		S5P_IRQ_VIC0(22)
#define IRQ_TIMER2_VIC		S5P_IRQ_VIC0(23)
#define IRQ_TIMER3_VIC		S5P_IRQ_VIC0(24)
#define IRQ_TIMER4_VIC		S5P_IRQ_VIC0(25)
#define IRQ_SYSTIMER		S5P_IRQ_VIC0(26)
#define IRQ_WDT			S5P_IRQ_VIC0(27)
#define IRQ_RTC_ALARM		S5P_IRQ_VIC0(28)
#define IRQ_RTC_TIC		S5P_IRQ_VIC0(29)
#define IRQ_GPIOINT		S5P_IRQ_VIC0(30)

/* VIC1: ARM, power, memory, connectivity */
#define IRQ_CORTEX0		S5P_IRQ_VIC1(0)
#define IRQ_CORTEX1		S5P_IRQ_VIC1(1)
#define IRQ_CORTEX2		S5P_IRQ_VIC1(2)
#define IRQ_CORTEX3		S5P_IRQ_VIC1(3)
#define IRQ_CORTEX4		S5P_IRQ_VIC1(4)
#define IRQ_IEMAPC		S5P_IRQ_VIC1(5)
#define IRQ_IEMIEC		S5P_IRQ_VIC1(6)
#define IRQ_ONENAND		S5P_IRQ_VIC1(7)
#define IRQ_NFC			S5P_IRQ_VIC1(8)
#define IRQ_CFC			S5P_IRQ_VIC1(9)
#define IRQ_UART0		S5P_IRQ_VIC1(10)
#define IRQ_UART1		S5P_IRQ_VIC1(11)
#define IRQ_UART2		S5P_IRQ_VIC1(12)
#define IRQ_UART3		S5P_IRQ_VIC1(13)
#define IRQ_IIC			S5P_IRQ_VIC1(14)
#define IRQ_SPI0		S5P_IRQ_VIC1(15)
#define IRQ_SPI1		S5P_IRQ_VIC1(16)
#define IRQ_SPI2		S5P_IRQ_VIC1(17)
#define IRQ_IRDA		S5P_IRQ_VIC1(18)
#define IRQ_CAN0		S5P_IRQ_VIC1(19)
#define IRQ_CAN1		S5P_IRQ_VIC1(20)
#define IRQ_HSIRX		S5P_IRQ_VIC1(21)
#define IRQ_HSITX		S5P_IRQ_VIC1(22)
#define IRQ_UHOST		S5P_IRQ_VIC1(23)
#define IRQ_OTG			S5P_IRQ_VIC1(24)
#define IRQ_MSM			S5P_IRQ_VIC1(25)
#define IRQ_HSMMC0		S5P_IRQ_VIC1(26)
#define IRQ_HSMMC1		S5P_IRQ_VIC1(27)
#define IRQ_HSMMC2		S5P_IRQ_VIC1(28)
#define IRQ_MIPICSI		S5P_IRQ_VIC1(29)
#define IRQ_MIPIDSI		S5P_IRQ_VIC1(30)

/* VIC2: multimedia, audio, security */
#define IRQ_LCD0		S5P_IRQ_VIC2(0)
#define IRQ_LCD1		S5P_IRQ_VIC2(1)
#define IRQ_LCD2		S5P_IRQ_VIC2(2)
#define IRQ_LCD3		S5P_IRQ_VIC2(3)
#define IRQ_ROTATOR		S5P_IRQ_VIC2(4)
#define IRQ_FIMC0		S5P_IRQ_VIC2(5)
#define IRQ_FIMC1		S5P_IRQ_VIC2(6)
#define IRQ_FIMC2		S5P_IRQ_VIC2(7)
#define IRQ_JPEG		S5P_IRQ_VIC2(8)
#define IRQ_2D			S5P_IRQ_VIC2(9)
#define IRQ_3D			S5P_IRQ_VIC2(10)
#define IRQ_MIXER		S5P_IRQ_VIC2(11)
#define IRQ_HDMI		S5P_IRQ_VIC2(12)
#define IRQ_IIC1		S5P_IRQ_VIC2(13)
#define IRQ_MFC			S5P_IRQ_VIC2(14)
#define IRQ_TVENC		S5P_IRQ_VIC2(15)
#define IRQ_I2S0		S5P_IRQ_VIC2(16)
#define IRQ_I2S1		S5P_IRQ_VIC2(17)
#define IRQ_I2S2		S5P_IRQ_VIC2(18)
#define IRQ_AC97		S5P_IRQ_VIC2(19)
#define IRQ_PCM0		S5P_IRQ_VIC2(20)
#define IRQ_PCM1		S5P_IRQ_VIC2(21)
#define IRQ_SPDIF		S5P_IRQ_VIC2(22)
#define IRQ_ADC			S5P_IRQ_VIC2(23)
#define IRQ_PENDN		S5P_IRQ_VIC2(24)
#define IRQ_TC			IRQ_PENDN
#define IRQ_KEYPAD		S5P_IRQ_VIC2(25)
#define IRQ_CG			S5P_IRQ_VIC2(26)
#define IRQ_SEC			S5P_IRQ_VIC2(27)
#define IRQ_SECRX		S5P_IRQ_VIC2(28)
#define IRQ_SECTX		S5P_IRQ_VIC2(29)
#define IRQ_SDMIRQ		S5P_IRQ_VIC2(30)
#define IRQ_SDMFIQ		S5P_IRQ_VIC2(31)
#define IRQ_VIC_END		S5P_IRQ_VIC2(31)

#define S5P_EINT_BASE1		(S5P_IRQ_VIC0(0))
#define S5P_EINT_BASE2		(IRQ_VIC_END + 1)

#define S3C_IRQ_GPIO_BASE	(IRQ_EINT(31) + 1)
#define S3C_IRQ_GPIO(x)		(S3C_IRQ_GPIO_BASE + (x))

/* Until MP04 Groups -> 40 (exactly 39) Groups * 8 ~= 320 GPIOs */
#define NR_IRQS		(S3C_IRQ_GPIO(320) + 1)

/* Compatibility */
#define IRQ_LCD_FIFO		IRQ_LCD0
#define IRQ_LCD_VSYNC		IRQ_LCD1
#define IRQ_LCD_SYSTEM		IRQ_LCD2

#endif /* __ASM_ARCH_IRQS_H */