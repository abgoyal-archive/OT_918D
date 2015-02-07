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

/* Key management controls
 *
 * Copyright (C) 2008 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */

#include <linux/key.h>
#include <linux/sysctl.h>
#include "internal.h"

static const int zero, one = 1, max = INT_MAX;

ctl_table key_sysctls[] = {
	{
		.procname = "maxkeys",
		.data = &key_quota_maxkeys,
		.maxlen = sizeof(unsigned),
		.mode = 0644,
		.proc_handler = proc_dointvec_minmax,
		.extra1 = (void *) &one,
		.extra2 = (void *) &max,
	},
	{
		.procname = "maxbytes",
		.data = &key_quota_maxbytes,
		.maxlen = sizeof(unsigned),
		.mode = 0644,
		.proc_handler = proc_dointvec_minmax,
		.extra1 = (void *) &one,
		.extra2 = (void *) &max,
	},
	{
		.procname = "root_maxkeys",
		.data = &key_quota_root_maxkeys,
		.maxlen = sizeof(unsigned),
		.mode = 0644,
		.proc_handler = proc_dointvec_minmax,
		.extra1 = (void *) &one,
		.extra2 = (void *) &max,
	},
	{
		.procname = "root_maxbytes",
		.data = &key_quota_root_maxbytes,
		.maxlen = sizeof(unsigned),
		.mode = 0644,
		.proc_handler = proc_dointvec_minmax,
		.extra1 = (void *) &one,
		.extra2 = (void *) &max,
	},
	{
		.procname = "gc_delay",
		.data = &key_gc_delay,
		.maxlen = sizeof(unsigned),
		.mode = 0644,
		.proc_handler = proc_dointvec_minmax,
		.extra1 = (void *) &zero,
		.extra2 = (void *) &max,
	},
	{ }
};
