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
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#ifndef _LINUX_IMMEDIATE_H
#define _LINUX_IMMEDIATE_H

/*
 * Immediate values, can be updated at runtime and save cache lines.
 *
 * (C) Copyright 2007 Mathieu Desnoyers <mathieu.desnoyers@polymtl.ca>
 *
 * Dual BSD/GPL v2 license.
 */

#ifdef USE_IMMEDIATE

#include <asm/immediate.h>

/**
 * imv_set - set immediate variable (with locking)
 * @name: immediate value name
 * @i: required value
 *
 * Sets the value of @name, taking the module_mutex if required by
 * the architecture.
 */
#define imv_set(name, i)						\
	do {								\
		name##__imv = (i);					\
		core_imv_update();					\
		module_imv_update();					\
	} while (0)

/*
 * Internal update functions.
 */
extern void core_imv_update(void);
extern void imv_update_range(const struct __imv *begin,
	const struct __imv *end);
extern void imv_unref_core_init(void);
extern void imv_unref(struct __imv *begin, struct __imv *end, void *start,
		unsigned long size);

#else

/*
 * Generic immediate values: a simple, standard, memory load.
 */

/**
 * imv_read - read immediate variable
 * @name: immediate value name
 *
 * Reads the value of @name.
 */
#define imv_read(name)			_imv_read(name)

/**
 * imv_set - set immediate variable (with locking)
 * @name: immediate value name
 * @i: required value
 *
 * Sets the value of @name, taking the module_mutex if required by
 * the architecture.
 */
#define imv_set(name, i)		(name##__imv = (i))

static inline void core_imv_update(void) { }
static inline void imv_unref_core_init(void) { }

#endif

#define DECLARE_IMV(type, name) extern __typeof__(type) name##__imv
#define DEFINE_IMV(type, name)  __typeof__(type) name##__imv

#define EXPORT_IMV_SYMBOL(name) EXPORT_SYMBOL(name##__imv)
#define EXPORT_IMV_SYMBOL_GPL(name) EXPORT_SYMBOL_GPL(name##__imv)

/**
 * _imv_read - Read immediate value with standard memory load.
 * @name: immediate value name
 *
 * Force a data read of the immediate value instead of the immediate value
 * based mechanism. Useful for __init and __exit section data read.
 */
#define _imv_read(name)		(name##__imv)

#endif
