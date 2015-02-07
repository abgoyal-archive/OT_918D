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
 *  Copyright (C) 2007-2009 Geert Uytterhoeven
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive
 * for more details.
 */

#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/zorro.h>

#include <asm/amigahw.h>
#include <asm/amigayle.h>


#ifdef CONFIG_ZORRO

static const struct resource zorro_resources[] __initconst = {
	/* Zorro II regions (on Zorro II/III) */
	{
		.name	= "Zorro II exp",
		.start	= 0x00e80000,
		.end	= 0x00efffff,
		.flags	= IORESOURCE_MEM,
	}, {
		.name	= "Zorro II mem",
		.start	= 0x00200000,
		.end	= 0x009fffff,
		.flags	= IORESOURCE_MEM,
	},
	/* Zorro III regions (on Zorro III only) */
	{
		.name	= "Zorro III exp",
		.start	= 0xff000000,
		.end	= 0xffffffff,
		.flags	= IORESOURCE_MEM,
	}, {
		.name	= "Zorro III cfg",
		.start	= 0x40000000,
		.end	= 0x7fffffff,
		.flags	= IORESOURCE_MEM,
	}
};


static int __init amiga_init_bus(void)
{
	if (!MACH_IS_AMIGA || !AMIGAHW_PRESENT(ZORRO))
		return -ENODEV;

	platform_device_register_simple("amiga-zorro", -1, zorro_resources,
					AMIGAHW_PRESENT(ZORRO3) ? 4 : 2);
	return 0;
}

subsys_initcall(amiga_init_bus);


static int z_dev_present(zorro_id id)
{
	unsigned int i;

	for (i = 0; i < zorro_num_autocon; i++)
		if (zorro_autocon[i].rom.er_Manufacturer == ZORRO_MANUF(id) &&
		    zorro_autocon[i].rom.er_Product == ZORRO_PROD(id))
			return 1;

	return 0;
}

#else /* !CONFIG_ZORRO */

static inline int z_dev_present(zorro_id id) { return 0; }

#endif /* !CONFIG_ZORRO */


static const struct resource a3000_scsi_resource __initconst = {
	.start	= 0xdd0000,
	.end	= 0xdd00ff,
	.flags	= IORESOURCE_MEM,
};


static const struct resource a4000t_scsi_resource __initconst = {
	.start	= 0xdd0000,
	.end	= 0xdd0fff,
	.flags	= IORESOURCE_MEM,
};


static const struct resource a1200_ide_resource __initconst = {
	.start	= 0xda0000,
	.end	= 0xda1fff,
	.flags	= IORESOURCE_MEM,
};

static const struct gayle_ide_platform_data a1200_ide_pdata __initconst = {
	.base		= 0xda0000,
	.irqport	= 0xda9000,
	.explicit_ack	= 1,
};


static const struct resource a4000_ide_resource __initconst = {
	.start	= 0xdd2000,
	.end	= 0xdd3fff,
	.flags	= IORESOURCE_MEM,
};

static const struct gayle_ide_platform_data a4000_ide_pdata __initconst = {
	.base		= 0xdd2020,
	.irqport	= 0xdd3020,
	.explicit_ack	= 0,
};


static const struct resource amiga_rtc_resource __initconst = {
	.start	= 0x00dc0000,
	.end	= 0x00dcffff,
	.flags	= IORESOURCE_MEM,
};


static int __init amiga_init_devices(void)
{
	struct platform_device *pdev;

	if (!MACH_IS_AMIGA)
		return -ENODEV;

	/* video hardware */
	if (AMIGAHW_PRESENT(AMI_VIDEO))
		platform_device_register_simple("amiga-video", -1, NULL, 0);


	/* sound hardware */
	if (AMIGAHW_PRESENT(AMI_AUDIO))
		platform_device_register_simple("amiga-audio", -1, NULL, 0);


	/* storage interfaces */
	if (AMIGAHW_PRESENT(AMI_FLOPPY))
		platform_device_register_simple("amiga-floppy", -1, NULL, 0);

	if (AMIGAHW_PRESENT(A3000_SCSI))
		platform_device_register_simple("amiga-a3000-scsi", -1,
						&a3000_scsi_resource, 1);

	if (AMIGAHW_PRESENT(A4000_SCSI))
		platform_device_register_simple("amiga-a4000t-scsi", -1,
						&a4000t_scsi_resource, 1);

	if (AMIGAHW_PRESENT(A1200_IDE) ||
	    z_dev_present(ZORRO_PROD_MTEC_VIPER_MK_V_E_MATRIX_530_SCSI_IDE)) {
		pdev = platform_device_register_simple("amiga-gayle-ide", -1,
						       &a1200_ide_resource, 1);
		platform_device_add_data(pdev, &a1200_ide_pdata,
					 sizeof(a1200_ide_pdata));
	}

	if (AMIGAHW_PRESENT(A4000_IDE)) {
		pdev = platform_device_register_simple("amiga-gayle-ide", -1,
						       &a4000_ide_resource, 1);
		platform_device_add_data(pdev, &a4000_ide_pdata,
					 sizeof(a4000_ide_pdata));
	}


	/* other I/O hardware */
	if (AMIGAHW_PRESENT(AMI_KEYBOARD))
		platform_device_register_simple("amiga-keyboard", -1, NULL, 0);

	if (AMIGAHW_PRESENT(AMI_MOUSE))
		platform_device_register_simple("amiga-mouse", -1, NULL, 0);

	if (AMIGAHW_PRESENT(AMI_SERIAL))
		platform_device_register_simple("amiga-serial", -1, NULL, 0);

	if (AMIGAHW_PRESENT(AMI_PARALLEL))
		platform_device_register_simple("amiga-parallel", -1, NULL, 0);


	/* real time clocks */
	if (AMIGAHW_PRESENT(A2000_CLK))
		platform_device_register_simple("rtc-msm6242", -1,
						&amiga_rtc_resource, 1);

	if (AMIGAHW_PRESENT(A3000_CLK))
		platform_device_register_simple("rtc-rp5c01", -1,
						&amiga_rtc_resource, 1);

	return 0;
}

device_initcall(amiga_init_devices);
