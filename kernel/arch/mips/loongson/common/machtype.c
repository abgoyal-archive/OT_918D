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
 * Copyright (C) 2009 Lemote Inc.
 * Author: Wu Zhangjin, wuzhangjin@gmail.com
 *
 * Copyright (c) 2009 Zhang Le <r0bertz@gentoo.org>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */
#include <linux/errno.h>
#include <asm/bootinfo.h>

#include <loongson.h>
#include <machine.h>

/* please ensure the length of the machtype string is less than 50 */
#define MACHTYPE_LEN 50

static const char *system_types[] = {
	[MACH_LOONGSON_UNKNOWN]         "unknown loongson machine",
	[MACH_LEMOTE_FL2E]              "lemote-fuloong-2e-box",
	[MACH_LEMOTE_FL2F]              "lemote-fuloong-2f-box",
	[MACH_LEMOTE_ML2F7]             "lemote-mengloong-2f-7inches",
	[MACH_LEMOTE_YL2F89]            "lemote-yeeloong-2f-8.9inches",
	[MACH_DEXXON_GDIUM2F10]         "dexxon-gdium-2f",
	[MACH_LEMOTE_NAS]		"lemote-nas-2f",
	[MACH_LEMOTE_LL2F]              "lemote-lynloong-2f",
	[MACH_LOONGSON_END]             NULL,
};

const char *get_system_type(void)
{
	return system_types[mips_machtype];
}

void __weak __init mach_prom_init_machtype(void)
{
}

void __init prom_init_machtype(void)
{
	char *p, str[MACHTYPE_LEN];
	int machtype = MACH_LEMOTE_FL2E;

	mips_machtype = LOONGSON_MACHTYPE;

	p = strstr(arcs_cmdline, "machtype=");
	if (!p) {
		mach_prom_init_machtype();
		return;
	}
	p += strlen("machtype=");
	strncpy(str, p, MACHTYPE_LEN);
	p = strstr(str, " ");
	if (p)
		*p = '\0';

	for (; system_types[machtype]; machtype++)
		if (strstr(system_types[machtype], str)) {
			mips_machtype = machtype;
			break;
		}
}
