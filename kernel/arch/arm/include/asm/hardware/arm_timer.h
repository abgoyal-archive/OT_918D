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

#ifndef __ASM_ARM_HARDWARE_ARM_TIMER_H
#define __ASM_ARM_HARDWARE_ARM_TIMER_H

/*
 * ARM timer implementation, found in Integrator, Versatile and Realview
 * platforms.  Not all platforms support all registers and bits in these
 * registers, so we mark them with A for Integrator AP, C for Integrator
 * CP, V for Versatile and R for Realview.
 *
 * Integrator AP has 16-bit timers, Integrator CP, Versatile and Realview
 * can have 16-bit or 32-bit selectable via a bit in the control register.
 */
#define TIMER_LOAD	0x00			/* ACVR rw */
#define TIMER_VALUE	0x04			/* ACVR ro */
#define TIMER_CTRL	0x08			/* ACVR rw */
#define TIMER_CTRL_ONESHOT	(1 << 0)	/*  CVR */
#define TIMER_CTRL_32BIT	(1 << 1)	/*  CVR */
#define TIMER_CTRL_DIV1		(0 << 2)	/* ACVR */
#define TIMER_CTRL_DIV16	(1 << 2)	/* ACVR */
#define TIMER_CTRL_DIV256	(2 << 2)	/* ACVR */
#define TIMER_CTRL_IE		(1 << 5)	/*   VR */
#define TIMER_CTRL_PERIODIC	(1 << 6)	/* ACVR */
#define TIMER_CTRL_ENABLE	(1 << 7)	/* ACVR */

#define TIMER_INTCLR	0x0c			/* ACVR wo */
#define TIMER_RIS	0x10			/*  CVR ro */
#define TIMER_MIS	0x14			/*  CVR ro */
#define TIMER_BGLOAD	0x18			/*  CVR rw */

#endif
