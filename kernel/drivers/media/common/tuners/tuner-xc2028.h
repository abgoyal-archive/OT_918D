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

/* tuner-xc2028
 *
 * Copyright (c) 2007-2008 Mauro Carvalho Chehab (mchehab@infradead.org)
 * This code is placed under the terms of the GNU General Public License v2
 */

#ifndef __TUNER_XC2028_H__
#define __TUNER_XC2028_H__

#include "dvb_frontend.h"

#define XC2028_DEFAULT_FIRMWARE "xc3028-v27.fw"
#define XC3028L_DEFAULT_FIRMWARE "xc3028L-v36.fw"

/*      Dmoduler		IF (kHz) */
#define	XC3028_FE_DEFAULT	0		/* Don't load SCODE */
#define XC3028_FE_LG60		6000
#define	XC3028_FE_ATI638	6380
#define	XC3028_FE_OREN538	5380
#define	XC3028_FE_OREN36	3600
#define	XC3028_FE_TOYOTA388	3880
#define	XC3028_FE_TOYOTA794	7940
#define	XC3028_FE_DIBCOM52	5200
#define	XC3028_FE_ZARLINK456	4560
#define	XC3028_FE_CHINA		5200

enum firmware_type {
	XC2028_AUTO = 0,        /* By default, auto-detects */
	XC2028_D2633,
	XC2028_D2620,
};

struct xc2028_ctrl {
	char			*fname;
	int			max_len;
	int			msleep;
	unsigned int		scode_table;
	unsigned int		mts   :1;
	unsigned int		input1:1;
	unsigned int		vhfbw7:1;
	unsigned int		uhfbw8:1;
	unsigned int		disable_power_mgmt:1;
	unsigned int            read_not_reliable:1;
	unsigned int		demod;
	enum firmware_type	type:2;
};

struct xc2028_config {
	struct i2c_adapter *i2c_adap;
	u8 		   i2c_addr;
	struct xc2028_ctrl *ctrl;
};

/* xc2028 commands for callback */
#define XC2028_TUNER_RESET	0
#define XC2028_RESET_CLK	1

#if defined(CONFIG_MEDIA_TUNER_XC2028) || (defined(CONFIG_MEDIA_TUNER_XC2028_MODULE) && defined(MODULE))
extern struct dvb_frontend *xc2028_attach(struct dvb_frontend *fe,
					  struct xc2028_config *cfg);
#else
static inline struct dvb_frontend *xc2028_attach(struct dvb_frontend *fe,
						 struct xc2028_config *cfg)
{
	printk(KERN_INFO "%s: not probed - driver disabled by Kconfig\n",
	       __func__);
	return NULL;
}
#endif

#endif /* __TUNER_XC2028_H__ */