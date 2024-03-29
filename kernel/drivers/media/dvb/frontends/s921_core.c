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
 * Driver for Sharp s921 driver
 *
 * Copyright (C) 2008 Markus Rechberger <mrechberger@sundtek.de>
 *
 */


#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include "s921_core.h"

static int s921_isdb_init(struct s921_isdb_t *dev);
static int s921_isdb_set_parameters(struct s921_isdb_t *dev, struct s921_isdb_t_transmission_mode_params *params);
static int s921_isdb_tune(struct s921_isdb_t *dev, struct s921_isdb_t_tune_params *params);
static int s921_isdb_get_status(struct s921_isdb_t *dev, void *data);

static u8 init_table[]={ 0x01, 0x40, 0x02, 0x00, 0x03, 0x40, 0x04, 0x01,
			 0x05, 0x00, 0x06, 0x00, 0x07, 0x00, 0x08, 0x00,
			 0x09, 0x00, 0x0a, 0x00, 0x0b, 0x5a, 0x0c, 0x00,
			 0x0d, 0x00, 0x0f, 0x00, 0x13, 0x1b, 0x14, 0x80,
			 0x15, 0x40, 0x17, 0x70, 0x18, 0x01, 0x19, 0x12,
			 0x1a, 0x01, 0x1b, 0x12, 0x1c, 0xa0, 0x1d, 0x00,
			 0x1e, 0x0a, 0x1f, 0x08, 0x20, 0x40, 0x21, 0xff,
			 0x22, 0x4c, 0x23, 0x4e, 0x24, 0x4c, 0x25, 0x00,
			 0x26, 0x00, 0x27, 0xf4, 0x28, 0x60, 0x29, 0x88,
			 0x2a, 0x40, 0x2b, 0x40, 0x2c, 0xff, 0x2d, 0x00,
			 0x2e, 0xff, 0x2f, 0x00, 0x30, 0x20, 0x31, 0x06,
			 0x32, 0x0c, 0x34, 0x0f, 0x37, 0xfe, 0x38, 0x00,
			 0x39, 0x63, 0x3a, 0x10, 0x3b, 0x10, 0x47, 0x00,
			 0x49, 0xe5, 0x4b, 0x00, 0x50, 0xc0, 0x52, 0x20,
			 0x54, 0x5a, 0x55, 0x5b, 0x56, 0x40, 0x57, 0x70,
			 0x5c, 0x50, 0x5d, 0x00, 0x62, 0x17, 0x63, 0x2f,
			 0x64, 0x6f, 0x68, 0x00, 0x69, 0x89, 0x6a, 0x00,
			 0x6b, 0x00, 0x6c, 0x00, 0x6d, 0x00, 0x6e, 0x00,
			 0x70, 0x00, 0x71, 0x00, 0x75, 0x00, 0x76, 0x30,
			 0x77, 0x01, 0xaf, 0x00, 0xb0, 0xa0, 0xb2, 0x3d,
			 0xb3, 0x25, 0xb4, 0x8b, 0xb5, 0x4b, 0xb6, 0x3f,
			 0xb7, 0xff, 0xb8, 0xff, 0xb9, 0xfc, 0xba, 0x00,
			 0xbb, 0x00, 0xbc, 0x00, 0xd0, 0x30, 0xe4, 0x84,
			 0xf0, 0x48, 0xf1, 0x19, 0xf2, 0x5a, 0xf3, 0x8e,
			 0xf4, 0x2d, 0xf5, 0x07, 0xf6, 0x5a, 0xf7, 0xba,
			 0xf8, 0xd7 };

static u8 c_table[]={ 0x58, 0x8a, 0x7b, 0x59, 0x8c, 0x7b, 0x5a, 0x8e, 0x5b,
		      0x5b, 0x90, 0x5b, 0x5c, 0x92, 0x5b, 0x5d, 0x94, 0x5b,
		      0x5e, 0x96, 0x5b, 0x5f, 0x98, 0x3b, 0x60, 0x9a, 0x3b,
		      0x61, 0x9c, 0x3b, 0x62, 0x9e, 0x3b, 0x63, 0xa0, 0x3b,
		      0x64, 0xa2, 0x1b, 0x65, 0xa4, 0x1b, 0x66, 0xa6, 0x1b,
		      0x67, 0xa8, 0x1b, 0x68, 0xaa, 0x1b, 0x69, 0xac, 0x1b,
		      0x6a, 0xae, 0x1b, 0x6b, 0xb0, 0x1b, 0x6c, 0xb2, 0x1b,
		      0x6d, 0xb4, 0xfb, 0x6e, 0xb6, 0xfb, 0x6f, 0xb8, 0xfb,
		      0x70, 0xba, 0xfb, 0x71, 0xbc, 0xdb, 0x72, 0xbe, 0xdb,
		      0x73, 0xc0, 0xdb, 0x74, 0xc2, 0xdb, 0x75, 0xc4, 0xdb,
		      0x76, 0xc6, 0xdb, 0x77, 0xc8, 0xbb, 0x78, 0xca, 0xbb,
		      0x79, 0xcc, 0xbb, 0x7a, 0xce, 0xbb, 0x7b, 0xd0, 0xbb,
		      0x7c, 0xd2, 0xbb, 0x7d, 0xd4, 0xbb, 0x7e, 0xd6, 0xbb,
		      0x7f, 0xd8, 0xbb, 0x80, 0xda, 0x9b, 0x81, 0xdc, 0x9b,
		      0x82, 0xde, 0x9b, 0x83, 0xe0, 0x9b, 0x84, 0xe2, 0x9b,
		      0x85, 0xe4, 0x9b, 0x86, 0xe6, 0x9b, 0x87, 0xe8, 0x9b,
		      0x88, 0xea, 0x9b, 0x89, 0xec, 0x9b };

int s921_isdb_cmd(struct s921_isdb_t *dev, u32 cmd, void *data) {
	switch(cmd) {
	case ISDB_T_CMD_INIT:
		s921_isdb_init(dev);
		break;
	case ISDB_T_CMD_SET_PARAM:
		s921_isdb_set_parameters(dev, data);
		break;
	case ISDB_T_CMD_TUNE:
		s921_isdb_tune(dev, data);
		break;
	case ISDB_T_CMD_GET_STATUS:
		s921_isdb_get_status(dev, data);
		break;
	default:
		printk("unhandled command\n");
		return -EINVAL;
	}
	return 0;
}

static int s921_isdb_init(struct s921_isdb_t *dev) {
	unsigned int i;
	unsigned int ret;
	printk("isdb_init\n");
	for (i = 0; i < sizeof(init_table); i+=2) {
		ret = dev->i2c_write(dev->priv_dev, init_table[i], init_table[i+1]);
		if (ret != 0) {
			printk("i2c write failed\n");
			return ret;
		}
	}
	return 0;
}

static int s921_isdb_set_parameters(struct s921_isdb_t *dev, struct s921_isdb_t_transmission_mode_params *params) {

	int ret;
	/* auto is sufficient for now, lateron this should be reflected in an extra interface */



	ret = dev->i2c_write(dev->priv_dev, 0xb0, 0xa0); //mod_b2);
	ret = dev->i2c_write(dev->priv_dev, 0xb2, 0x3d); //mod_b2);

	if (ret < 0)
		return -EINVAL;

	ret = dev->i2c_write(dev->priv_dev, 0xb3, 0x25); //mod_b3);
	if (ret < 0)
		return -EINVAL;

	ret = dev->i2c_write(dev->priv_dev, 0xb4, 0x8b); //mod_b4);
	if (ret < 0)
		return -EINVAL;

	ret = dev->i2c_write(dev->priv_dev, 0xb5, 0x4b); //mod_b5);
	if (ret < 0)
		return -EINVAL;

	ret = dev->i2c_write(dev->priv_dev, 0xb6, 0x3f); //mod_b6);
	if (ret < 0)
		return -EINVAL;

	ret = dev->i2c_write(dev->priv_dev, 0xb7, 0x3f); //mod_b7);
	if (ret < 0)
		return -EINVAL;

	return E_OK;
}

static int s921_isdb_tune(struct s921_isdb_t *dev, struct s921_isdb_t_tune_params *params) {

	int ret;
	int index;

	index = (params->frequency - 473143000)/6000000;

	if (index > 48) {
		return -EINVAL;
	}

	dev->i2c_write(dev->priv_dev, 0x47, 0x60);

	ret = dev->i2c_write(dev->priv_dev, 0x68, 0x00);
	if (ret < 0)
		return -EINVAL;

	ret = dev->i2c_write(dev->priv_dev, 0x69, 0x89);
	if (ret < 0)
		return -EINVAL;

	ret = dev->i2c_write(dev->priv_dev, 0xf0, 0x48);
	if (ret < 0)
		return -EINVAL;

	ret = dev->i2c_write(dev->priv_dev, 0xf1, 0x19);
	if (ret < 0)
		return -EINVAL;

	ret = dev->i2c_write(dev->priv_dev, 0xf2, c_table[index*3]);
	if (ret < 0)
		return -EINVAL;

	ret = dev->i2c_write(dev->priv_dev, 0xf3, c_table[index*3+1]);
	if (ret < 0)
		return -EINVAL;

	ret = dev->i2c_write(dev->priv_dev, 0xf4, c_table[index*3+2]);
	if (ret < 0)
		return -EINVAL;

	ret = dev->i2c_write(dev->priv_dev, 0xf5, 0xae);
	if (ret < 0)
		return -EINVAL;

	ret = dev->i2c_write(dev->priv_dev, 0xf6, 0xb7);
	if (ret < 0)
		return -EINVAL;

	ret = dev->i2c_write(dev->priv_dev, 0xf7, 0xba);
	if (ret < 0)
		return -EINVAL;

	ret = dev->i2c_write(dev->priv_dev, 0xf8, 0xd7);
	if (ret < 0)
		return -EINVAL;

	ret = dev->i2c_write(dev->priv_dev, 0x68, 0x0a);
	if (ret < 0)
		return -EINVAL;

	ret = dev->i2c_write(dev->priv_dev, 0x69, 0x09);
	if (ret < 0)
		return -EINVAL;

	dev->i2c_write(dev->priv_dev, 0x01, 0x40);
	return 0;
}

static int s921_isdb_get_status(struct s921_isdb_t *dev, void *data) {
	unsigned int *ret = (unsigned int*)data;
	u8 ifagc_dt;
	u8 rfagc_dt;

	mdelay(10);
	ifagc_dt = dev->i2c_read(dev->priv_dev, 0x81);
	rfagc_dt = dev->i2c_read(dev->priv_dev, 0x82);
	if (rfagc_dt == 0x40) {
		*ret = 1;
	}
	return 0;
}
