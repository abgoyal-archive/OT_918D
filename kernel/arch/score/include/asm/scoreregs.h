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

#ifndef _ASM_SCORE_SCOREREGS_H
#define _ASM_SCORE_SCOREREGS_H

#include <linux/linkage.h>

/* TIMER register */
#define TIME0BASE		0x96080000
#define P_TIMER0_CTRL		(TIME0BASE + 0x00)
#define P_TIMER0_CPP_CTRL	(TIME0BASE + 0x04)
#define P_TIMER0_PRELOAD	(TIME0BASE + 0x08)
#define P_TIMER0_CPP_REG	(TIME0BASE + 0x0C)
#define P_TIMER0_UPCNT		(TIME0BASE + 0x10)

/* Timer Controller Register */
/* bit 0 Timer enable */
#define TMR_DISABLE	0x0000
#define TMR_ENABLE	0x0001

/* bit 1 Interrupt enable */
#define TMR_IE_DISABLE	0x0000
#define TMR_IE_ENABLE	0x0002

/* bit 2 Output enable */
#define TMR_OE_DISABLE	0x0004
#define TMR_OE_ENABLE	0x0000

/* bit4 Up/Down counting selection */
#define TMR_UD_DOWN	0x0000
#define TMR_UD_UP	0x0010

/* bit5 Up/Down counting control selection */
#define TMR_UDS_UD	0x0000
#define TMR_UDS_EXTUD	0x0020

/* bit6 Time output mode */
#define TMR_OM_TOGGLE	0x0000
#define TMR_OM_PILSE	0x0040

/* bit 8..9 External input active edge selection */
#define TMR_ES_PE	0x0000
#define TMR_ES_NE	0x0100
#define TMR_ES_BOTH	0x0200

/* bit 10..11 Operating mode */
#define TMR_M_FREE	0x0000 /* free running timer mode */
#define TMR_M_PERIODIC	0x0400 /* periodic timer mode */
#define TMR_M_FC	0x0800 /* free running counter mode */
#define TMR_M_PC	0x0c00 /* periodic counter mode */

#define SYSTEM_CLOCK		(27*1000000/4)		/* 27 MHz */
#endif /* _ASM_SCORE_SCOREREGS_H */
