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

/****************************************************************************/
/*
 *      arch/arm/mach-l7200/include/mach/gpio.h
 *
 *      Registers and  helper functions for the L7200 Link-Up Systems
 *      GPIO.
 *
 *      (C) Copyright 2000, S A McConnell  (samcconn@cotw.com)
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License. See the file COPYING in the main directory of this archive for
 *  more details.
 */

/****************************************************************************/

#define GPIO_OFF   0x00005000  /* Offset from IO_START to the GPIO reg's. */

/* IO_START and IO_BASE are defined in hardware.h */

#define GPIO_START (IO_START_2 + GPIO_OFF) /* Physical addr of the GPIO reg. */
#define GPIO_BASE  (IO_BASE_2  + GPIO_OFF) /* Virtual addr of the GPIO reg. */

/* Offsets from the start of the GPIO for all the registers. */
#define PADR_OFF     0x000
#define PADDR_OFF    0x004
#define PASBSR_OFF   0x008
#define PAEENR_OFF   0x00c
#define PAESNR_OFF   0x010
#define PAESTR_OFF   0x014
#define PAIMR_OFF    0x018
#define PAINT_OFF    0x01c

#define PBDR_OFF     0x020
#define PBDDR_OFF    0x024
#define PBSBSR_OFF   0x028
#define PBIMR_OFF    0x038
#define PBINT_OFF    0x03c

#define PCDR_OFF     0x040
#define PCDDR_OFF    0x044
#define PCSBSR_OFF   0x048
#define PCIMR_OFF    0x058
#define PCINT_OFF    0x05c

#define PDDR_OFF     0x060
#define PDDDR_OFF    0x064
#define PDSBSR_OFF   0x068
#define PDEENR_OFF   0x06c
#define PDESNR_OFF   0x070
#define PDESTR_OFF   0x074
#define PDIMR_OFF    0x078
#define PDINT_OFF    0x07c

#define PEDR_OFF     0x080
#define PEDDR_OFF    0x084
#define PESBSR_OFF   0x088
#define PEEENR_OFF   0x08c
#define PEESNR_OFF   0x090
#define PEESTR_OFF   0x094
#define PEIMR_OFF    0x098
#define PEINT_OFF    0x09c

/* Define the GPIO registers for use by device drivers and the kernel. */
#define PADR   (*(volatile unsigned long *)(GPIO_BASE+PADR_OFF))
#define PADDR  (*(volatile unsigned long *)(GPIO_BASE+PADDR_OFF))
#define PASBSR (*(volatile unsigned long *)(GPIO_BASE+PASBSR_OFF))
#define PAEENR (*(volatile unsigned long *)(GPIO_BASE+PAEENR_OFF))
#define PAESNR (*(volatile unsigned long *)(GPIO_BASE+PAESNR_OFF))
#define PAESTR (*(volatile unsigned long *)(GPIO_BASE+PAESTR_OFF))
#define PAIMR  (*(volatile unsigned long *)(GPIO_BASE+PAIMR_OFF))
#define PAINT  (*(volatile unsigned long *)(GPIO_BASE+PAINT_OFF))

#define PBDR   (*(volatile unsigned long *)(GPIO_BASE+PBDR_OFF))
#define PBDDR  (*(volatile unsigned long *)(GPIO_BASE+PBDDR_OFF))
#define PBSBSR (*(volatile unsigned long *)(GPIO_BASE+PBSBSR_OFF))
#define PBIMR  (*(volatile unsigned long *)(GPIO_BASE+PBIMR_OFF))
#define PBINT  (*(volatile unsigned long *)(GPIO_BASE+PBINT_OFF))

#define PCDR   (*(volatile unsigned long *)(GPIO_BASE+PCDR_OFF))
#define PCDDR  (*(volatile unsigned long *)(GPIO_BASE+PCDDR_OFF))
#define PCSBSR (*(volatile unsigned long *)(GPIO_BASE+PCSBSR_OFF))
#define PCIMR  (*(volatile unsigned long *)(GPIO_BASE+PCIMR_OFF))
#define PCINT  (*(volatile unsigned long *)(GPIO_BASE+PCINT_OFF))

#define PDDR   (*(volatile unsigned long *)(GPIO_BASE+PDDR_OFF))
#define PDDDR  (*(volatile unsigned long *)(GPIO_BASE+PDDDR_OFF))
#define PDSBSR (*(volatile unsigned long *)(GPIO_BASE+PDSBSR_OFF))
#define PDEENR (*(volatile unsigned long *)(GPIO_BASE+PDEENR_OFF))
#define PDESNR (*(volatile unsigned long *)(GPIO_BASE+PDESNR_OFF))
#define PDESTR (*(volatile unsigned long *)(GPIO_BASE+PDESTR_OFF))
#define PDIMR  (*(volatile unsigned long *)(GPIO_BASE+PDIMR_OFF))
#define PDINT  (*(volatile unsigned long *)(GPIO_BASE+PDINT_OFF))

#define PEDR   (*(volatile unsigned long *)(GPIO_BASE+PEDR_OFF))
#define PEDDR  (*(volatile unsigned long *)(GPIO_BASE+PEDDR_OFF))
#define PESBSR (*(volatile unsigned long *)(GPIO_BASE+PESBSR_OFF))
#define PEEENR (*(volatile unsigned long *)(GPIO_BASE+PEEENR_OFF))
#define PEESNR (*(volatile unsigned long *)(GPIO_BASE+PEESNR_OFF))
#define PEESTR (*(volatile unsigned long *)(GPIO_BASE+PEESTR_OFF))
#define PEIMR  (*(volatile unsigned long *)(GPIO_BASE+PEIMR_OFF))
#define PEINT  (*(volatile unsigned long *)(GPIO_BASE+PEINT_OFF))

#define VEE_EN         0x02
#define BACKLIGHT_EN   0x04
