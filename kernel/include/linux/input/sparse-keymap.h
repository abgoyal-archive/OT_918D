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

#ifndef _SPARSE_KEYMAP_H
#define _SPARSE_KEYMAP_H

/*
 * Copyright (c) 2009 Dmitry Torokhov
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */

#define KE_END		0	/* Indicates end of keymap */
#define KE_KEY		1	/* Ordinary key/button */
#define KE_SW		2	/* Switch (predetermined value) */
#define KE_VSW		3	/* Switch (value supplied at runtime) */
#define KE_IGNORE	4	/* Known entry that should be ignored */
#define KE_LAST		KE_IGNORE

/**
 * struct key_entry - keymap entry for use in sparse keymap
 * @type: Type of the key entry (KE_KEY, KE_SW, KE_VSW, KE_END);
 *	drivers are allowed to extend the list with their own
 *	private definitions.
 * @code: Device-specific data identifying the button/switch
 * @keycode: KEY_* code assigned to a key/button
 * @sw.code: SW_* code assigned to a switch
 * @sw.value: Value that should be sent in an input even when KE_SW
 *	switch is toggled. KE_VSW switches ignore this field and
 *	expect driver to supply value for the event.
 *
 * This structure defines an entry in a sparse keymap used by some
 * input devices for which traditional table-based approach is not
 * suitable.
 */
struct key_entry {
	int type;		/* See KE_* above */
	u32 code;
	union {
		u16 keycode;		/* For KE_KEY */
		struct {		/* For KE_SW, KE_VSW */
			u8 code;
			u8 value;	/* For KE_SW, ignored by KE_VSW */
		} sw;
	};
};

struct key_entry *sparse_keymap_entry_from_scancode(struct input_dev *dev,
						    unsigned int code);
struct key_entry *sparse_keymap_entry_from_keycode(struct input_dev *dev,
						   unsigned int code);
int sparse_keymap_setup(struct input_dev *dev,
			const struct key_entry *keymap,
			int (*setup)(struct input_dev *, struct key_entry *));
void sparse_keymap_free(struct input_dev *dev);

void sparse_keymap_report_entry(struct input_dev *dev, const struct key_entry *ke,
				unsigned int value, bool autorelease);

bool sparse_keymap_report_event(struct input_dev *dev, unsigned int code,
				unsigned int value, bool autorelease);

#endif /* _SPARSE_KEYMAP_H */
