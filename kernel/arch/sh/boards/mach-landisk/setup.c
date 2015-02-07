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
 * arch/sh/boards/landisk/setup.c
 *
 * I-O DATA Device, Inc. LANDISK Support.
 *
 * Copyright (C) 2000 Kazumoto Kojima
 * Copyright (C) 2002 Paul Mundt
 * Copylight (C) 2002 Atom Create Engineering Co., Ltd.
 * Copyright (C) 2005-2007 kogiidena
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/ata_platform.h>
#include <linux/pm.h>
#include <linux/mm.h>
#include <asm/machvec.h>
#include <mach-landisk/mach/iodata_landisk.h>
#include <asm/io.h>

void init_landisk_IRQ(void);

static void landisk_power_off(void)
{
        __raw_writeb(0x01, PA_SHUTDOWN);
}

static struct resource cf_ide_resources[3];

static struct pata_platform_info pata_info = {
	.ioport_shift	= 1,
};

static struct platform_device cf_ide_device = {
	.name		= "pata_platform",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(cf_ide_resources),
	.resource	= cf_ide_resources,
	.dev		= {
		.platform_data = &pata_info,
	},
};

static struct platform_device rtc_device = {
	.name		= "rs5c313",
	.id		= -1,
};

static struct platform_device *landisk_devices[] __initdata = {
	&cf_ide_device,
	&rtc_device,
};

static int __init landisk_devices_setup(void)
{
	pgprot_t prot;
	unsigned long paddrbase;
	void *cf_ide_base;

	/* open I/O area window */
	paddrbase = virt_to_phys((void *)PA_AREA5_IO);
	prot = PAGE_KERNEL_PCC(1, _PAGE_PCC_IO16);
	cf_ide_base = ioremap_prot(paddrbase, PAGE_SIZE, pgprot_val(prot));
	if (!cf_ide_base) {
		printk("allocate_cf_area : can't open CF I/O window!\n");
		return -ENOMEM;
	}

	/* IDE cmd address : 0x1f0-0x1f7 and 0x3f6 */
	cf_ide_resources[0].start = (unsigned long)cf_ide_base + 0x40;
	cf_ide_resources[0].end   = (unsigned long)cf_ide_base + 0x40 + 0x0f;
	cf_ide_resources[0].flags = IORESOURCE_IO;
	cf_ide_resources[1].start = (unsigned long)cf_ide_base + 0x2c;
	cf_ide_resources[1].end   = (unsigned long)cf_ide_base + 0x2c + 0x03;
	cf_ide_resources[1].flags = IORESOURCE_IO;
	cf_ide_resources[2].start = IRQ_FATA;
	cf_ide_resources[2].flags = IORESOURCE_IRQ;

	return platform_add_devices(landisk_devices,
				    ARRAY_SIZE(landisk_devices));
}

__initcall(landisk_devices_setup);

static void __init landisk_setup(char **cmdline_p)
{
        /* LED ON */
	__raw_writeb(__raw_readb(PA_LED) | 0x03, PA_LED);

	printk(KERN_INFO "I-O DATA DEVICE, INC. \"LANDISK Series\" support.\n");
	pm_power_off = landisk_power_off;
}

/*
 * The Machine Vector
 */
static struct sh_machine_vector mv_landisk __initmv = {
	.mv_name = "LANDISK",
	.mv_nr_irqs = 72,
	.mv_setup = landisk_setup,
	.mv_init_irq = init_landisk_IRQ,
};
