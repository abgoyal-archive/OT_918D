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
 * Copyright (C) 2006 Benjamin Herrenschmidt, IBM Corp.
 *			<benh@kernel.crashing.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#ifndef _ASM_MICROBLAZE_OF_PLATFORM_H
#define _ASM_MICROBLAZE_OF_PLATFORM_H

/* This is just here during the transition */
#include <linux/of_platform.h>

/*
 * The list of OF IDs below is used for matching bus types in the
 * system whose devices are to be exposed as of_platform_devices.
 *
 * This is the default list valid for most platforms. This file provides
 * functions who can take an explicit list if necessary though
 *
 * The search is always performed recursively looking for children of
 * the provided device_node and recursively if such a children matches
 * a bus type in the list
 */

static const struct of_device_id of_default_bus_ids[] = {
	{ .type = "soc", },
	{ .compatible = "soc", },
	{ .type = "plb5", },
	{ .type = "plb4", },
	{ .type = "opb", },
	{ .type = "simple", },
	{},
};

/* Platform devices and busses creation */
extern struct of_device *of_platform_device_create(struct device_node *np,
						const char *bus_id,
						struct device *parent);
/* pseudo "matches" value to not do deep probe */
#define OF_NO_DEEP_PROBE ((struct of_device_id *)-1)

extern int of_platform_bus_probe(struct device_node *root,
				const struct of_device_id *matches,
				struct device *parent);

extern struct of_device *of_find_device_by_phandle(phandle ph);

extern void of_instantiate_rtc(void);

#endif /* _ASM_MICROBLAZE_OF_PLATFORM_H */
