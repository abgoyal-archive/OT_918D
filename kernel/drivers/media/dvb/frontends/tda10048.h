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
    NXP TDA10048HN DVB OFDM demodulator driver

    Copyright (C) 2009 Steven Toth <stoth@kernellabs.com>

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

#ifndef TDA10048_H
#define TDA10048_H

#include <linux/dvb/frontend.h>
#include <linux/firmware.h>

struct tda10048_config {

	/* the demodulator's i2c address */
	u8 demod_address;

	/* serial/parallel output */
#define TDA10048_PARALLEL_OUTPUT 0
#define TDA10048_SERIAL_OUTPUT   1
	u8 output_mode;

#define TDA10048_BULKWRITE_200	200
#define TDA10048_BULKWRITE_50	50
	u8 fwbulkwritelen;

	/* Spectral Inversion */
#define TDA10048_INVERSION_OFF 0
#define TDA10048_INVERSION_ON  1
	u8 inversion;

#define TDA10048_IF_3300  3300
#define TDA10048_IF_3500  3500
#define TDA10048_IF_3800  3800
#define TDA10048_IF_4000  4000
#define TDA10048_IF_4300  4300
#define TDA10048_IF_4500  4500
#define TDA10048_IF_4750  4750
#define TDA10048_IF_36130 36130
	u16 dtv6_if_freq_khz;
	u16 dtv7_if_freq_khz;
	u16 dtv8_if_freq_khz;

#define TDA10048_CLK_4000  4000
#define TDA10048_CLK_16000 16000
	u16 clk_freq_khz;

	/* Disable I2C gate access */
	u8 disable_gate_access;
};

#if defined(CONFIG_DVB_TDA10048) || \
	(defined(CONFIG_DVB_TDA10048_MODULE) && defined(MODULE))
extern struct dvb_frontend *tda10048_attach(
	const struct tda10048_config *config,
	struct i2c_adapter *i2c);
#else
static inline struct dvb_frontend *tda10048_attach(
	const struct tda10048_config *config,
	struct i2c_adapter *i2c)
{
	printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
	return NULL;
}
#endif /* CONFIG_DVB_TDA10048 */

#endif /* TDA10048_H */
