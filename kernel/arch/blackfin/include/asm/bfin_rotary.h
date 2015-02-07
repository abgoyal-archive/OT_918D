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
 * board initialization should put one of these structures into platform_data
 * and place the bfin-rotary onto platform_bus named "bfin-rotary".
 *
 * Copyright 2008 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#ifndef _BFIN_ROTARY_H
#define _BFIN_ROTARY_H

/* mode bitmasks */
#define ROT_QUAD_ENC	CNTMODE_QUADENC	/* quadrature/grey code encoder mode */
#define ROT_BIN_ENC	CNTMODE_BINENC	/* binary encoder mode */
#define ROT_UD_CNT	CNTMODE_UDCNT	/* rotary counter mode */
#define ROT_DIR_CNT	CNTMODE_DIRCNT	/* direction counter mode */

#define ROT_DEBE	DEBE		/* Debounce Enable */

#define ROT_CDGINV	CDGINV		/* CDG Pin Polarity Invert */
#define ROT_CUDINV	CUDINV		/* CUD Pin Polarity Invert */
#define ROT_CZMINV	CZMINV		/* CZM Pin Polarity Invert */

struct bfin_rotary_platform_data {
	/* set rotary UP KEY_### or BTN_### in case you prefer
	 * bfin-rotary to send EV_KEY otherwise set 0
	 */
	unsigned int rotary_up_key;
	/* set rotary DOWN KEY_### or BTN_### in case you prefer
	 * bfin-rotary to send EV_KEY otherwise set 0
	 */
	unsigned int rotary_down_key;
	/* set rotary BUTTON KEY_### or BTN_### */
	unsigned int rotary_button_key;
	/* set rotary Relative Axis REL_### in case you prefer
	 * bfin-rotary to send EV_REL otherwise set 0
	 */
	unsigned int rotary_rel_code;
	unsigned short debounce;	/* 0..17 */
	unsigned short mode;
};
#endif
