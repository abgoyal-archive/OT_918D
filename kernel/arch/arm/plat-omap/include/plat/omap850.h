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

/* arch/arm/plat-omap/include/mach/omap850.h
 *
 * Hardware definitions for TI OMAP850 processor.
 *
 * Derived from omap730.h by Zebediah C. McClure <zmc@lurian.net>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * You should have received a copy of the  GNU General Public License along
 * with this program; if not, write  to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __ASM_ARCH_OMAP850_H
#define __ASM_ARCH_OMAP850_H

/*
 * ----------------------------------------------------------------------------
 * Base addresses
 * ----------------------------------------------------------------------------
 */

/* Syntax: XX_BASE = Virtual base address, XX_START = Physical base address */

#define OMAP850_DSP_BASE	0xE0000000
#define OMAP850_DSP_SIZE	0x50000
#define OMAP850_DSP_START	0xE0000000

#define OMAP850_DSPREG_BASE	0xE1000000
#define OMAP850_DSPREG_SIZE	SZ_128K
#define OMAP850_DSPREG_START	0xE1000000

/*
 * ----------------------------------------------------------------------------
 * OMAP850 specific configuration registers
 * ----------------------------------------------------------------------------
 */
#define OMAP850_CONFIG_BASE	0xfffe1000
#define OMAP850_IO_CONF_0	0xfffe1070
#define OMAP850_IO_CONF_1	0xfffe1074
#define OMAP850_IO_CONF_2	0xfffe1078
#define OMAP850_IO_CONF_3	0xfffe107c
#define OMAP850_IO_CONF_4	0xfffe1080
#define OMAP850_IO_CONF_5	0xfffe1084
#define OMAP850_IO_CONF_6	0xfffe1088
#define OMAP850_IO_CONF_7	0xfffe108c
#define OMAP850_IO_CONF_8	0xfffe1090
#define OMAP850_IO_CONF_9	0xfffe1094
#define OMAP850_IO_CONF_10	0xfffe1098
#define OMAP850_IO_CONF_11	0xfffe109c
#define OMAP850_IO_CONF_12	0xfffe10a0
#define OMAP850_IO_CONF_13	0xfffe10a4

#define OMAP850_MODE_1		0xfffe1010
#define OMAP850_MODE_2		0xfffe1014

/* CSMI specials: in terms of base + offset */
#define OMAP850_MODE2_OFFSET	0x14

/*
 * ----------------------------------------------------------------------------
 * OMAP850 traffic controller configuration registers
 * ----------------------------------------------------------------------------
 */
#define OMAP850_FLASH_CFG_0	0xfffecc10
#define OMAP850_FLASH_ACFG_0	0xfffecc50
#define OMAP850_FLASH_CFG_1	0xfffecc14
#define OMAP850_FLASH_ACFG_1	0xfffecc54

/*
 * ----------------------------------------------------------------------------
 * OMAP850 DSP control registers
 * ----------------------------------------------------------------------------
 */
#define OMAP850_ICR_BASE	0xfffbb800
#define OMAP850_DSP_M_CTL	0xfffbb804
#define OMAP850_DSP_MMU_BASE	0xfffed200

/*
 * ----------------------------------------------------------------------------
 * OMAP850 PCC_UPLD configuration registers
 * ----------------------------------------------------------------------------
 */
#define OMAP850_PCC_UPLD_CTRL_BASE	(0xfffe0900)
#define OMAP850_PCC_UPLD_CTRL		(OMAP850_PCC_UPLD_CTRL_BASE + 0x00)

#endif /*  __ASM_ARCH_OMAP850_H */
