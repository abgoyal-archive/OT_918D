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

/* drivers/video/msm_fb/mddi_client_dummy.c
 *
 * Support for "dummy" mddi client devices which require no
 * special initialization code.
 *
 * Copyright (C) 2007 Google Incorporated
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>

#include <mach/msm_fb.h>

struct panel_info {
	struct platform_device pdev;
	struct msm_panel_data panel_data;
};

static int mddi_dummy_suspend(struct msm_panel_data *panel_data)
{
	return 0;
}

static int mddi_dummy_resume(struct msm_panel_data *panel_data)
{
	return 0;
}

static int mddi_dummy_blank(struct msm_panel_data *panel_data)
{
	return 0;
}

static int mddi_dummy_unblank(struct msm_panel_data *panel_data)
{
	return 0;
}

static int mddi_dummy_probe(struct platform_device *pdev)
{
	struct msm_mddi_client_data *client_data = pdev->dev.platform_data;
	struct panel_info *panel =
		kzalloc(sizeof(struct panel_info), GFP_KERNEL);
	int ret;
	if (!panel)
		return -ENOMEM;
	platform_set_drvdata(pdev, panel);
	panel->panel_data.suspend = mddi_dummy_suspend;
	panel->panel_data.resume = mddi_dummy_resume;
	panel->panel_data.blank = mddi_dummy_blank;
	panel->panel_data.unblank = mddi_dummy_unblank;
	panel->panel_data.caps = MSMFB_CAP_PARTIAL_UPDATES;
	panel->pdev.name = "msm_panel";
	panel->pdev.id = pdev->id;
	platform_device_add_resources(&panel->pdev,
				      client_data->fb_resource, 1);
	panel->panel_data.fb_data = client_data->private_client_data;
	panel->pdev.dev.platform_data = &panel->panel_data;
	ret = platform_device_register(&panel->pdev);
	if (ret) {
		kfree(panel);
		return ret;
	}
	return 0;
}

static int mddi_dummy_remove(struct platform_device *pdev)
{
	struct panel_info *panel = platform_get_drvdata(pdev);
	kfree(panel);
	return 0;
}

static struct platform_driver mddi_client_dummy = {
	.probe = mddi_dummy_probe,
	.remove = mddi_dummy_remove,
	.driver = { .name = "mddi_c_dummy" },
};

static int __init mddi_client_dummy_init(void)
{
	platform_driver_register(&mddi_client_dummy);
	return 0;
}

module_init(mddi_client_dummy_init);

