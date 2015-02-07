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
 * Copyright (C) 2004,2005  ADDI-DATA GmbH for the source code of this module.
 *
 *	ADDI-DATA GmbH
 *	Dieselstrasse 3
 *	D-77833 Ottersweier
 *	Tel: +19(0)7223/9493-0
 *	Fax: +49(0)7223/9493-92
 *	http://www.addi-data-com
 *	info@addi-data.com
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 */

#define COMEDI_SUBD_TTLIO		11	/* Digital Input Output But TTL */
#define COMEDI_SUBD_PWM			12	/* Pulse width Measurement */
#define COMEDI_SUBD_SSI			13	/* Synchronous serial interface */
#define COMEDI_SUBD_TOR			14	/* Tor counter */
#define COMEDI_SUBD_CHRONO		15	/* Chrono meter */
#define COMEDI_SUBD_PULSEENCODER	16	/* Pulse Encoder INP CPT */
#define COMEDI_SUBD_INCREMENTALCOUNTER	17	/* Incremental Counter */

#define APCI1710_BOARD_NAME		"apci1710"
#define APCI1710_BOARD_VENDOR_ID	0x10E8
#define APCI1710_BOARD_DEVICE_ID	0x818F
#define APCI1710_ADDRESS_RANGE		256
#define APCI1710_CONFIG_ADDRESS_RANGE	8
#define APCI1710_INCREMENTAL_COUNTER	0x53430000UL
#define APCI1710_SSI_COUNTER		0x53490000UL
#define APCI1710_TTL_IO			0x544C0000UL
#define APCI1710_DIGITAL_IO		0x44490000UL
#define APCI1710_82X54_TIMER		0x49430000UL
#define APCI1710_CHRONOMETER		0x43480000UL
#define APCI1710_PULSE_ENCODER		0x495A0000UL
#define APCI1710_TOR_COUNTER		0x544F0000UL
#define APCI1710_PWM			0x50570000UL
#define APCI1710_ETM			0x45540000UL
#define APCI1710_CDA			0x43440000UL
#define APCI1710_DISABLE		0
#define APCI1710_ENABLE			1
#define APCI1710_SYNCHRONOUS_MODE	1
#define APCI1710_ASYNCHRONOUS_MODE	0

/* MODULE INFO STRUCTURE */

static const struct comedi_lrange range_apci1710_ttl = { 4, {
						      BIP_RANGE(10),
						      BIP_RANGE(5),
						      BIP_RANGE(2),
						      BIP_RANGE(1)
						      }
};

static const struct comedi_lrange range_apci1710_ssi = { 4, {
						      BIP_RANGE(10),
						      BIP_RANGE(5),
						      BIP_RANGE(2),
						      BIP_RANGE(1)
						      }
};

static const struct comedi_lrange range_apci1710_inccpt = { 4, {
							 BIP_RANGE(10),
							 BIP_RANGE(5),
							 BIP_RANGE(2),
							 BIP_RANGE(1)
							 }
};
