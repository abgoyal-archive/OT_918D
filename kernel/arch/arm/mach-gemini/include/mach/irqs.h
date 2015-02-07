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
 *  Copyright (C) 2001-2006 Storlink, Corp.
 *  Copyright (C) 2008-2009 Paulius Zaleckas <paulius.zaleckas@teltonika.lt>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef __MACH_IRQS_H__
#define __MACH_IRQS_H__

#define IRQ_SERIRQ1	31
#define IRQ_SERIRQ0	30
#define IRQ_PCID	29
#define IRQ_PCIC	28
#define IRQ_PCIB	27
#define IRQ_PWR		26
#define IRQ_CIR		25
#define IRQ_GPIO(x)	(22 + (x))
#define IRQ_SSP		21
#define IRQ_LPC		20
#define IRQ_LCD		19
#define IRQ_UART	18
#define IRQ_RTC		17
#define IRQ_TIMER3	16
#define IRQ_TIMER2	15
#define IRQ_TIMER1	14
#define IRQ_FLASH	12
#define IRQ_USB1	11
#define IRQ_USB0	10
#define IRQ_DMA		9
#define IRQ_PCI		8
#define IRQ_IPSEC	7
#define IRQ_RAID	6
#define IRQ_IDE1	5
#define IRQ_IDE0	4
#define IRQ_WATCHDOG	3
#define IRQ_GMAC1	2
#define IRQ_GMAC0	1
#define IRQ_IPI		0

#define NORMAL_IRQ_NUM	32

#define GPIO_IRQ_BASE	NORMAL_IRQ_NUM
#define GPIO_IRQ_NUM	(3 * 32)

#define ARCH_TIMER_IRQ	IRQ_TIMER2

#define NR_IRQS		(NORMAL_IRQ_NUM + GPIO_IRQ_NUM)

#endif /* __MACH_IRQS_H__ */
