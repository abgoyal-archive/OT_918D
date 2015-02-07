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
 *  linux/drivers/mfd/ucb1x00-assabet.c
 *
 *  Copyright (C) 2001-2003 Russell King, All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 *  We handle the machine-specific bits of the UCB1x00 driver here.
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <linux/mfd/ucb1x00.h>

#include <mach/dma.h>


#define UCB1X00_ATTR(name,input)\
static ssize_t name##_show(struct device *dev, struct device_attribute *attr, \
			   char *buf)	\
{								\
	struct ucb1x00 *ucb = classdev_to_ucb1x00(dev);		\
	int val;						\
	ucb1x00_adc_enable(ucb);				\
	val = ucb1x00_adc_read(ucb, input, UCB_NOSYNC);		\
	ucb1x00_adc_disable(ucb);				\
	return sprintf(buf, "%d\n", val);			\
}								\
static DEVICE_ATTR(name,0444,name##_show,NULL)

UCB1X00_ATTR(vbatt, UCB_ADC_INP_AD1);
UCB1X00_ATTR(vcharger, UCB_ADC_INP_AD0);
UCB1X00_ATTR(batt_temp, UCB_ADC_INP_AD2);

static int ucb1x00_assabet_add(struct ucb1x00_dev *dev)
{
	device_create_file(&dev->ucb->dev, &dev_attr_vbatt);
	device_create_file(&dev->ucb->dev, &dev_attr_vcharger);
	device_create_file(&dev->ucb->dev, &dev_attr_batt_temp);
	return 0;
}

static void ucb1x00_assabet_remove(struct ucb1x00_dev *dev)
{
	device_remove_file(&dev->ucb->dev, &dev_attr_batt_temp);
	device_remove_file(&dev->ucb->dev, &dev_attr_vcharger);
	device_remove_file(&dev->ucb->dev, &dev_attr_vbatt);
}

static struct ucb1x00_driver ucb1x00_assabet_driver = {
	.add	= ucb1x00_assabet_add,
	.remove	= ucb1x00_assabet_remove,
};

static int __init ucb1x00_assabet_init(void)
{
	return ucb1x00_register_driver(&ucb1x00_assabet_driver);
}

static void __exit ucb1x00_assabet_exit(void)
{
	ucb1x00_unregister_driver(&ucb1x00_assabet_driver);
}

module_init(ucb1x00_assabet_init);
module_exit(ucb1x00_assabet_exit);

MODULE_AUTHOR("Russell King <rmk@arm.linux.org.uk>");
MODULE_DESCRIPTION("Assabet noddy testing only example ADC driver");
MODULE_LICENSE("GPL");
