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

/****************************************************************

Siano Mobile Silicon, Inc.
MDTV receiver kernel modules.
Copyright (C) 2006-2009, Uri Shkolnik

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

 This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

****************************************************************/

#ifndef __SMS_IR_H__
#define __SMS_IR_H__

#include <linux/input.h>

#define IR_DEV_NAME_MAX_LEN		23 /* "SMS IR kbd type nn\0" */
#define IR_KEYBOARD_LAYOUT_SIZE	64
#define IR_DEFAULT_TIMEOUT		100

enum ir_kb_type {
	SMS_IR_KB_DEFAULT_TV,
	SMS_IR_KB_HCW_SILVER
};

enum rc5_keyboard_address {
	KEYBOARD_ADDRESS_TV1 = 0,
	KEYBOARD_ADDRESS_TV2 = 1,
	KEYBOARD_ADDRESS_TELETEXT = 2,
	KEYBOARD_ADDRESS_VIDEO = 3,
	KEYBOARD_ADDRESS_LV1 = 4,
	KEYBOARD_ADDRESS_VCR1 = 5,
	KEYBOARD_ADDRESS_VCR2 = 6,
	KEYBOARD_ADDRESS_EXPERIMENTAL = 7,
	KEYBOARD_ADDRESS_SAT1 = 8,
	KEYBOARD_ADDRESS_CAMERA = 9,
	KEYBOARD_ADDRESS_SAT2 = 10,
	KEYBOARD_ADDRESS_CDV = 12,
	KEYBOARD_ADDRESS_CAMCORDER = 13,
	KEYBOARD_ADDRESS_PRE_AMP = 16,
	KEYBOARD_ADDRESS_TUNER = 17,
	KEYBOARD_ADDRESS_RECORDER1 = 18,
	KEYBOARD_ADDRESS_PRE_AMP1 = 19,
	KEYBOARD_ADDRESS_CD_PLAYER = 20,
	KEYBOARD_ADDRESS_PHONO = 21,
	KEYBOARD_ADDRESS_SATA = 22,
	KEYBOARD_ADDRESS_RECORDER2 = 23,
	KEYBOARD_ADDRESS_CDR = 26,
	KEYBOARD_ADDRESS_LIGHTING = 29,
	KEYBOARD_ADDRESS_LIGHTING1 = 30, /* KEYBOARD_ADDRESS_HCW_SILVER */
	KEYBOARD_ADDRESS_PHONE = 31,
	KEYBOARD_ADDRESS_NOT_RC5 = 0xFFFF
};

enum ir_protocol {
	IR_RC5,
	IR_RCMM
};

struct keyboard_layout_map_t {
	enum ir_protocol ir_protocol;
	enum rc5_keyboard_address rc5_kbd_address;
	u16 keyboard_layout_map[IR_KEYBOARD_LAYOUT_SIZE];
};

struct smscore_device_t;

struct ir_t {
	struct input_dev *input_dev;
	enum ir_kb_type ir_kb_type;
	char name[IR_DEV_NAME_MAX_LEN+1];
	u16 *keyboard_layout_map;
	u32 timeout;
	u32 controller;
};

int sms_ir_init(struct smscore_device_t *coredev);
void sms_ir_exit(struct smscore_device_t *coredev);
void sms_ir_event(struct smscore_device_t *coredev,
			const char *buf, int len);

#endif /* __SMS_IR_H__ */

