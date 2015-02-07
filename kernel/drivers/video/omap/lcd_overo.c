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
 * LCD panel support for the Gumstix Overo
 *
 * Author: Steve Sakoman <steve@sakoman.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/i2c/twl.h>

#include <mach/gpio.h>
#include <plat/mux.h>
#include <asm/mach-types.h>

#include "omapfb.h"

#define LCD_ENABLE       144

static int overo_panel_init(struct lcd_panel *panel,
				struct omapfb_device *fbdev)
{
	if ((gpio_request(LCD_ENABLE, "LCD_ENABLE") == 0) &&
	    (gpio_direction_output(LCD_ENABLE, 1) == 0))
		gpio_export(LCD_ENABLE, 0);
	else
		printk(KERN_ERR "could not obtain gpio for LCD_ENABLE\n");

	return 0;
}

static void overo_panel_cleanup(struct lcd_panel *panel)
{
	gpio_free(LCD_ENABLE);
}

static int overo_panel_enable(struct lcd_panel *panel)
{
	gpio_set_value(LCD_ENABLE, 1);
	return 0;
}

static void overo_panel_disable(struct lcd_panel *panel)
{
	gpio_set_value(LCD_ENABLE, 0);
}

static unsigned long overo_panel_get_caps(struct lcd_panel *panel)
{
	return 0;
}

struct lcd_panel overo_panel = {
	.name		= "overo",
	.config		= OMAP_LCDC_PANEL_TFT,
	.bpp		= 16,
	.data_lines	= 24,

#if defined CONFIG_FB_OMAP_031M3R

	/* 640 x 480 @ 60 Hz  Reduced blanking VESA CVT 0.31M3-R */
	.x_res		= 640,
	.y_res		= 480,
	.hfp		= 48,
	.hsw		= 32,
	.hbp		= 80,
	.vfp		= 3,
	.vsw		= 4,
	.vbp		= 7,
	.pixel_clock	= 23500,

#elif defined CONFIG_FB_OMAP_048M3R

	/* 800 x 600 @ 60 Hz  Reduced blanking VESA CVT 0.48M3-R */
	.x_res		= 800,
	.y_res		= 600,
	.hfp		= 48,
	.hsw		= 32,
	.hbp		= 80,
	.vfp		= 3,
	.vsw		= 4,
	.vbp		= 11,
	.pixel_clock	= 35500,

#elif defined CONFIG_FB_OMAP_079M3R

	/* 1024 x 768 @ 60 Hz  Reduced blanking VESA CVT 0.79M3-R */
	.x_res		= 1024,
	.y_res		= 768,
	.hfp		= 48,
	.hsw		= 32,
	.hbp		= 80,
	.vfp		= 3,
	.vsw		= 4,
	.vbp		= 15,
	.pixel_clock	= 56000,

#elif defined CONFIG_FB_OMAP_092M9R

	/* 1280 x 720 @ 60 Hz  Reduced blanking VESA CVT 0.92M9-R */
	.x_res		= 1280,
	.y_res		= 720,
	.hfp		= 48,
	.hsw		= 32,
	.hbp		= 80,
	.vfp		= 3,
	.vsw		= 5,
	.vbp		= 13,
	.pixel_clock	= 64000,

#else

	/* use 640 x 480 if no config option */
	/* 640 x 480 @ 60 Hz  Reduced blanking VESA CVT 0.31M3-R */
	.x_res		= 640,
	.y_res		= 480,
	.hfp		= 48,
	.hsw		= 32,
	.hbp		= 80,
	.vfp		= 3,
	.vsw		= 4,
	.vbp		= 7,
	.pixel_clock	= 23500,

#endif

	.init		= overo_panel_init,
	.cleanup	= overo_panel_cleanup,
	.enable		= overo_panel_enable,
	.disable	= overo_panel_disable,
	.get_caps	= overo_panel_get_caps,
};

static int overo_panel_probe(struct platform_device *pdev)
{
	omapfb_register_panel(&overo_panel);
	return 0;
}

static int overo_panel_remove(struct platform_device *pdev)
{
	/* omapfb does not have unregister_panel */
	return 0;
}

static struct platform_driver overo_panel_driver = {
	.probe		= overo_panel_probe,
	.remove		= overo_panel_remove,
	.driver		= {
		.name	= "overo_lcd",
		.owner	= THIS_MODULE,
	},
};

static int __init overo_panel_drv_init(void)
{
	return platform_driver_register(&overo_panel_driver);
}

static void __exit overo_panel_drv_exit(void)
{
	platform_driver_unregister(&overo_panel_driver);
}

module_init(overo_panel_drv_init);
module_exit(overo_panel_drv_exit);
