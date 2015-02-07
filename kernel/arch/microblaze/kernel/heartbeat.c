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
 * Copyright (C) 2007-2009 Michal Simek <monstr@monstr.eu>
 * Copyright (C) 2007-2009 PetaLogix
 * Copyright (C) 2006 Atmark Techno, Inc.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#include <linux/sched.h>
#include <linux/io.h>

#include <asm/setup.h>
#include <asm/page.h>
#include <asm/prom.h>

static unsigned int base_addr;

void heartbeat(void)
{
	static unsigned int cnt, period, dist;

	if (base_addr) {
		if (cnt == 0 || cnt == dist)
			out_be32(base_addr, 1);
		else if (cnt == 7 || cnt == dist + 7)
			out_be32(base_addr, 0);

		if (++cnt > period) {
			cnt = 0;
			/*
			 * The hyperbolic function below modifies the heartbeat
			 * period length in dependency of the current (5min)
			 * load. It goes through the points f(0)=126, f(1)=86,
			 * f(5)=51, f(inf)->30.
			 */
			period = ((672 << FSHIFT) / (5 * avenrun[0] +
						(7 << FSHIFT))) + 30;
			dist = period / 4;
		}
	}
}

void setup_heartbeat(void)
{
	struct device_node *gpio = NULL;
	int *prop;
	int j;
	char *gpio_list[] = {
				"xlnx,xps-gpio-1.00.a",
				"xlnx,opb-gpio-1.00.a",
				NULL
			};

	for (j = 0; gpio_list[j] != NULL; j++) {
		gpio = of_find_compatible_node(NULL, NULL, gpio_list[j]);
		if (gpio)
			break;
	}

	if (gpio) {
		base_addr = *(int *) of_get_property(gpio, "reg", NULL);
		base_addr = (unsigned long) ioremap(base_addr, PAGE_SIZE);
		printk(KERN_NOTICE "Heartbeat GPIO at 0x%x\n", base_addr);

		/* GPIO is configured as output */
		prop = (int *) of_get_property(gpio, "xlnx,is-bidir", NULL);
		if (prop)
			out_be32(base_addr + 4, 0);
	}
}
