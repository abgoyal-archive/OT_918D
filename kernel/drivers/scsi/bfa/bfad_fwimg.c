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
 * Copyright (c) 2005-2009 Brocade Communications Systems, Inc.
 * All rights reserved
 * www.brocade.com
 *
 * Linux driver for Brocade Fibre Channel Host Bus Adapter.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License (GPL) Version 2 as
 * published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */

/**
 *  bfad_fwimg.c Linux driver PCI interface module.
 */
#include <bfa_os_inc.h>
#include <bfad_drv.h>
#include <bfad_im_compat.h>
#include <defs/bfa_defs_version.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/fcntl.h>
#include <linux/pci.h>
#include <linux/firmware.h>
#include <bfa_fwimg_priv.h>
#include <bfa.h>

u32 bfi_image_ct_size;
u32 bfi_image_cb_size;
u32 *bfi_image_ct;
u32 *bfi_image_cb;


#define	BFAD_FW_FILE_CT	"ctfw.bin"
#define	BFAD_FW_FILE_CB	"cbfw.bin"
MODULE_FIRMWARE(BFAD_FW_FILE_CT);
MODULE_FIRMWARE(BFAD_FW_FILE_CB);

u32 *
bfad_read_firmware(struct pci_dev *pdev, u32 **bfi_image,
			u32 *bfi_image_size, char *fw_name)
{
	const struct firmware *fw;

	if (request_firmware(&fw, fw_name, &pdev->dev)) {
		printk(KERN_ALERT "Can't locate firmware %s\n", fw_name);
		goto error;
	}

	*bfi_image = vmalloc(fw->size);
	if (NULL == *bfi_image) {
		printk(KERN_ALERT "Fail to allocate buffer for fw image "
			"size=%x!\n", (u32) fw->size);
		goto error;
	}

	memcpy(*bfi_image, fw->data, fw->size);
	*bfi_image_size = fw->size/sizeof(u32);

	return *bfi_image;

error:
	return NULL;
}

u32 *
bfad_get_firmware_buf(struct pci_dev *pdev)
{
	if (pdev->device == BFA_PCI_DEVICE_ID_CT) {
		if (bfi_image_ct_size == 0)
			bfad_read_firmware(pdev, &bfi_image_ct,
				&bfi_image_ct_size, BFAD_FW_FILE_CT);
		return bfi_image_ct;
	} else {
		if (bfi_image_cb_size == 0)
			bfad_read_firmware(pdev, &bfi_image_cb,
				&bfi_image_cb_size, BFAD_FW_FILE_CB);
		return bfi_image_cb;
	}
}

u32 *
bfi_image_ct_get_chunk(u32 off)
{ return (u32 *)(bfi_image_ct + off); }

u32 *
bfi_image_cb_get_chunk(u32 off)
{ return (u32 *)(bfi_image_cb + off); }

