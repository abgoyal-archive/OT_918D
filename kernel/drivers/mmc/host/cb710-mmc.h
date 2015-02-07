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
 *  cb710/cb710-mmc.h
 *
 *  Copyright by Michał Mirosław, 2008-2009
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef LINUX_CB710_MMC_H
#define LINUX_CB710_MMC_H

#include <linux/cb710.h>

/* per-MMC-reader structure */
struct cb710_mmc_reader {
	struct tasklet_struct finish_req_tasklet;
	struct mmc_request *mrq;
	spinlock_t irq_lock;
	unsigned char last_power_mode;
};

/* some device struct walking */

static inline struct mmc_host *cb710_slot_to_mmc(struct cb710_slot *slot)
{
	return dev_get_drvdata(&slot->pdev.dev);
}

static inline struct cb710_slot *cb710_mmc_to_slot(struct mmc_host *mmc)
{
	struct platform_device *pdev = container_of(mmc_dev(mmc),
		struct platform_device, dev);
	return cb710_pdev_to_slot(pdev);
}

/* registers (this might be all wrong ;) */

#define CB710_MMC_DATA_PORT		0x00

#define CB710_MMC_CONFIG_PORT		0x04
#define CB710_MMC_CONFIG0_PORT		0x04
#define CB710_MMC_CONFIG1_PORT		0x05
#define   CB710_MMC_C1_4BIT_DATA_BUS		0x40
#define CB710_MMC_CONFIG2_PORT		0x06
#define   CB710_MMC_C2_READ_PIO_SIZE_MASK	0x0F	/* N-1 */
#define CB710_MMC_CONFIG3_PORT		0x07

#define CB710_MMC_CONFIGB_PORT		0x08

#define CB710_MMC_IRQ_ENABLE_PORT	0x0C
#define   CB710_MMC_IE_TEST_MASK		0x00BF
#define   CB710_MMC_IE_CARD_INSERTION_STATUS	0x1000
#define   CB710_MMC_IE_IRQ_ENABLE		0x8000
#define   CB710_MMC_IE_CISTATUS_MASK		\
		(CB710_MMC_IE_CARD_INSERTION_STATUS|CB710_MMC_IE_IRQ_ENABLE)

#define CB710_MMC_STATUS_PORT		0x10
#define   CB710_MMC_STATUS_ERROR_EVENTS		0x60FF
#define CB710_MMC_STATUS0_PORT		0x10
#define   CB710_MMC_S0_FIFO_UNDERFLOW		0x40
#define CB710_MMC_STATUS1_PORT		0x11
#define   CB710_MMC_S1_COMMAND_SENT		0x01
#define   CB710_MMC_S1_DATA_TRANSFER_DONE	0x02
#define   CB710_MMC_S1_PIO_TRANSFER_DONE	0x04
#define   CB710_MMC_S1_CARD_CHANGED		0x10
#define   CB710_MMC_S1_RESET			0x20
#define CB710_MMC_STATUS2_PORT		0x12
#define   CB710_MMC_S2_FIFO_READY		0x01
#define   CB710_MMC_S2_FIFO_EMPTY		0x02
#define   CB710_MMC_S2_BUSY_10			0x10
#define   CB710_MMC_S2_BUSY_20			0x20
#define CB710_MMC_STATUS3_PORT		0x13
#define   CB710_MMC_S3_CARD_DETECTED		0x02
#define   CB710_MMC_S3_WRITE_PROTECTED		0x04

#define CB710_MMC_CMD_TYPE_PORT		0x14
#define   CB710_MMC_RSP_TYPE_MASK		0x0007
#define     CB710_MMC_RSP_R1			(0)
#define     CB710_MMC_RSP_136			(5)
#define     CB710_MMC_RSP_NO_CRC		(2)
#define   CB710_MMC_RSP_PRESENT_MASK		0x0018
#define     CB710_MMC_RSP_NONE			(0 << 3)
#define     CB710_MMC_RSP_PRESENT		(1 << 3)
#define     CB710_MMC_RSP_PRESENT_X		(2 << 3)
#define   CB710_MMC_CMD_TYPE_MASK		0x0060
#define     CB710_MMC_CMD_BC			(0 << 5)
#define     CB710_MMC_CMD_BCR			(1 << 5)
#define     CB710_MMC_CMD_AC			(2 << 5)
#define     CB710_MMC_CMD_ADTC			(3 << 5)
#define   CB710_MMC_DATA_READ			0x0080
#define   CB710_MMC_CMD_CODE_MASK		0x3F00
#define   CB710_MMC_CMD_CODE_SHIFT		8
#define   CB710_MMC_IS_APP_CMD			0x4000
#define   CB710_MMC_RSP_BUSY			0x8000

#define CB710_MMC_CMD_PARAM_PORT	0x18
#define CB710_MMC_TRANSFER_SIZE_PORT	0x1C
#define CB710_MMC_RESPONSE0_PORT	0x20
#define CB710_MMC_RESPONSE1_PORT	0x24
#define CB710_MMC_RESPONSE2_PORT	0x28
#define CB710_MMC_RESPONSE3_PORT	0x2C

#endif /* LINUX_CB710_MMC_H */
