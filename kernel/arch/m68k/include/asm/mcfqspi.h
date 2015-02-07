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
 * Definitions for Freescale Coldfire QSPI module
 *
 * Copyright 2010 Steven King <sfking@fdwdc.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
*/

#ifndef mcfqspi_h
#define mcfqspi_h

#if defined(CONFIG_M523x) || defined(CONFIG_M527x) || defined(CONFIG_M528x)
#define	MCFQSPI_IOBASE		(MCF_IPSBAR + 0x340)
#elif defined(CONFIG_M5249)
#define MCFQSPI_IOBASE		(MCF_MBAR + 0x300)
#elif defined(CONFIG_M520x) || defined(CONFIG_M532x)
#define MCFQSPI_IOBASE		0xFC058000
#endif
#define MCFQSPI_IOSIZE		0x40

/**
 * struct mcfqspi_cs_control - chip select control for the coldfire qspi driver
 * @setup: setup the control; allocate gpio's, etc. May be NULL.
 * @teardown: finish with the control; free gpio's, etc. May be NULL.
 * @select: output the signals to select the device.  Can not be NULL.
 * @deselect: output the signals to deselect the device. Can not be NULL.
 *
 * The QSPI module has 4 hardware chip selects.  We don't use them.  Instead
 * platforms are required to supply a mcfqspi_cs_control as a part of the
 * platform data for each QSPI master controller.  Only the select and
 * deselect functions are required.
*/
struct mcfqspi_cs_control {
	int 	(*setup)(struct mcfqspi_cs_control *);
	void	(*teardown)(struct mcfqspi_cs_control *);
	void	(*select)(struct mcfqspi_cs_control *, u8, bool);
	void	(*deselect)(struct mcfqspi_cs_control *, u8, bool);
};

/**
 * struct mcfqspi_platform_data - platform data for the coldfire qspi driver
 * @bus_num: board specific identifier for this qspi driver.
 * @num_chipselects: number of chip selects supported by this qspi driver.
 * @cs_control: platform dependent chip select control.
*/
struct mcfqspi_platform_data {
	s16	bus_num;
	u16	num_chipselect;
	struct mcfqspi_cs_control *cs_control;
};

#endif /* mcfqspi_h */
