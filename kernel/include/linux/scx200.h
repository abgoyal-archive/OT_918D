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

/* linux/include/linux/scx200.h

   Copyright (c) 2001,2002 Christer Weinigel <wingel@nano-system.com>

   Defines for the National Semiconductor SCx200 Processors
*/

/* Interesting stuff for the National Semiconductor SCx200 CPU */

extern unsigned scx200_cb_base;

#define scx200_cb_present() (scx200_cb_base!=0)

/* F0 PCI Header/Bridge Configuration Registers */
#define SCx200_DOCCS_BASE 0x78	/* DOCCS Base Address Register */
#define SCx200_DOCCS_CTRL 0x7c	/* DOCCS Control Register */

/* GPIO Register Block */
#define SCx200_GPIO_SIZE 0x2c	/* Size of GPIO register block */

/* General Configuration Block */
#define SCx200_CB_BASE_FIXED 0x9000	/* Base fixed at 0x9000 according to errata? */

/* Watchdog Timer */
#define SCx200_WDT_OFFSET 0x00	/* offset within configuration block */
#define SCx200_WDT_SIZE 0x05	/* size */

#define SCx200_WDT_WDTO 0x00	/* Time-Out Register */
#define SCx200_WDT_WDCNFG 0x02	/* Configuration Register */
#define SCx200_WDT_WDSTS 0x04	/* Status Register */
#define SCx200_WDT_WDSTS_WDOVF (1<<0) /* Overflow bit */

/* High Resolution Timer */
#define SCx200_TIMER_OFFSET 0x08
#define SCx200_TIMER_SIZE 0x06

/* Clock Generators */
#define SCx200_CLOCKGEN_OFFSET 0x10
#define SCx200_CLOCKGEN_SIZE 0x10

/* Pin Multiplexing and Miscellaneous Configuration Registers */
#define SCx200_MISC_OFFSET 0x30
#define SCx200_MISC_SIZE 0x10

#define SCx200_PMR 0x30		/* Pin Multiplexing Register */
#define SCx200_MCR 0x34		/* Miscellaneous Configuration Register */
#define SCx200_INTSEL 0x38	/* Interrupt Selection Register */
#define SCx200_IID 0x3c		/* IA On a Chip Identification Number Reg */
#define SCx200_REV 0x3d		/* Revision Register */
#define SCx200_CBA 0x3e		/* Configuration Base Address Register */
#define SCx200_CBA_SCRATCH 0x64	/* Configuration Base Address Scratchpad */
