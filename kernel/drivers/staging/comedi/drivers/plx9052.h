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
    comedi/drivers/plx9052.h
    Definitions for the PLX-9052 PCI interface chip

    Copyright (C) 2002 MEV Ltd. <http://www.mev.co.uk/>

    COMEDI - Linux Control and Measurement Device Interface
    Copyright (C) 2000 David A. Schleef <ds@schleef.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#ifndef _PLX9052_H_
#define _PLX9052_H_

/*
 * PLX PCI9052 INTCSR register.
 */
#define PLX9052_INTCSR	0x4C	/* Offset in Local Configuration Registers */
/* Local Interrupt 1 Enable */
#define PLX9052_INTCSR_LI1ENAB_MASK		0x0001
#define PLX9052_INTCSR_LI1ENAB_DISABLED		0x0000
#define PLX9052_INTCSR_LI1ENAB_ENABLED		0x0001
/* Local Interrupt 1 Polarity */
#define PLX9052_INTCSR_LI1POL_MASK		0x0002
#define PLX9052_INTCSR_LI1POL_LOW		0x0000
#define PLX9052_INTCSR_LI1POL_HIGH		0x0002
/* Local Interrupt 1 Status (read-only) */
#define PLX9052_INTCSR_LI1STAT_MASK		0x0004
#define PLX9052_INTCSR_LI1STAT_INACTIVE		0x0000
#define PLX9052_INTCSR_LI1STAT_ACTIVE		0x0004
/* Local Interrupt 2 Enable */
#define PLX9052_INTCSR_LI2ENAB_MASK		0x0008
#define PLX9052_INTCSR_LI2ENAB_DISABLED		0x0000
#define PLX9052_INTCSR_LI2ENAB_ENABLED		0x0008
/* Local Interrupt 2 Polarity */
#define PLX9052_INTCSR_LI2POL_MASK		0x0010
#define PLX9052_INTCSR_LI2POL_LOW		0x0000
#define PLX9052_INTCSR_LI2POL_HIGH		0x0010
/* Local Interrupt 2 Status (read-only) */
#define PLX9052_INTCSR_LI2STAT_MASK		0x0020
#define PLX9052_INTCSR_LI2STAT_INACTIVE		0x0000
#define PLX9052_INTCSR_LI2STAT_ACTIVE		0x0020
/* PCI Interrupt Enable */
#define PLX9052_INTCSR_PCIENAB_MASK		0x0040
#define PLX9052_INTCSR_PCIENAB_DISABLED		0x0000
#define PLX9052_INTCSR_PCIENAB_ENABLED		0x0040
/* Software Interrupt */
#define PLX9052_INTCSR_SOFTINT_MASK		0x0080
#define PLX9052_INTCSR_SOFTINT_UNASSERTED	0x0000
#define PLX9052_INTCSR_SOFTINT_ASSERTED		0x0080
/* Local Interrupt 1 Select Enable */
#define PLX9052_INTCSR_LI1SEL_MASK		0x0100
#define PLX9052_INTCSR_LI1SEL_LEVEL		0x0000
#define PLX9052_INTCSR_LI1SEL_EDGE		0x0100
/* Local Interrupt 2 Select Enable */
#define PLX9052_INTCSR_LI2SEL_MASK		0x0200
#define PLX9052_INTCSR_LI2SEL_LEVEL		0x0000
#define PLX9052_INTCSR_LI2SEL_EDGE		0x0200
/* Local Edge Triggerable Interrupt 1 Clear Bit */
#define PLX9052_INTCSR_LI1CLRINT_MASK		0x0400
#define PLX9052_INTCSR_LI1CLRINT_UNASSERTED	0x0000
#define PLX9052_INTCSR_LI1CLRINT_ASSERTED	0x0400
/* Local Edge Triggerable Interrupt 2 Clear Bit */
#define PLX9052_INTCSR_LI2CLRINT_MASK		0x0800
#define PLX9052_INTCSR_LI2CLRINT_UNASSERTED	0x0000
#define PLX9052_INTCSR_LI2CLRINT_ASSERTED	0x0800
/* ISA Interface Mode Enable (read-only over PCI bus) */
#define PLX9052_INTCSR_ISAMODE_MASK		0x1000
#define PLX9052_INTCSR_ISAMODE_DISABLED		0x0000
#define PLX9052_INTCSR_ISAMODE_ENABLED		0x1000

#endif /* _PLX9052_H_ */
