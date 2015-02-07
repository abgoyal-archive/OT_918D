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
 * TXx9 SoC AC Link Controller
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __TXX9ACLC_H
#define __TXX9ACLC_H

#include <linux/interrupt.h>
#include <asm/txx9/dmac.h>

#define ACCTLEN			0x00	/* control enable */
#define ACCTLDIS		0x04	/* control disable */
#define   ACCTL_ENLINK		0x00000001	/* enable/disable AC-link */
#define   ACCTL_AUDODMA		0x00000100	/* AUDODMA enable/disable */
#define   ACCTL_AUDIDMA		0x00001000	/* AUDIDMA enable/disable */
#define   ACCTL_AUDOEHLT	0x00010000	/* AUDO error halt
						   enable/disable */
#define   ACCTL_AUDIEHLT	0x00100000	/* AUDI error halt
						   enable/disable */
#define ACREGACC		0x08	/* codec register access */
#define   ACREGACC_DAT_SHIFT	0	/* data field */
#define   ACREGACC_REG_SHIFT	16	/* address field */
#define   ACREGACC_CODECID_SHIFT	24	/* CODEC ID field */
#define   ACREGACC_READ		0x80000000	/* CODEC read */
#define   ACREGACC_WRITE	0x00000000	/* CODEC write */
#define ACINTSTS		0x10	/* interrupt status */
#define ACINTMSTS		0x14	/* interrupt masked status */
#define ACINTEN			0x18	/* interrupt enable */
#define ACINTDIS		0x1c	/* interrupt disable */
#define   ACINT_CODECRDY(n)	(0x00000001 << (n))	/* CODECn ready */
#define   ACINT_REGACCRDY	0x00000010	/* ACREGACC ready */
#define   ACINT_AUDOERR		0x00000100	/* AUDO underrun error */
#define   ACINT_AUDIERR		0x00001000	/* AUDI overrun error */
#define ACDMASTS		0x80	/* DMA request status */
#define   ACDMA_AUDO		0x00000001	/* AUDODMA pending */
#define   ACDMA_AUDI		0x00000010	/* AUDIDMA pending */
#define ACAUDODAT		0xa0	/* audio out data */
#define ACAUDIDAT		0xb0	/* audio in data */
#define ACREVID			0xfc	/* revision ID */

struct txx9aclc_dmadata {
	struct resource *dma_res;
	struct txx9dmac_slave dma_slave;
	struct dma_chan *dma_chan;
	struct tasklet_struct tasklet;
	spinlock_t dma_lock;
	int stream; /* SNDRV_PCM_STREAM_PLAYBACK or SNDRV_PCM_STREAM_CAPTURE */
	struct snd_pcm_substream *substream;
	unsigned long pos;
	dma_addr_t dma_addr;
	unsigned long buffer_bytes;
	unsigned long period_bytes;
	unsigned long frag_bytes;
	int frags;
	int frag_count;
	int dmacount;
};

struct txx9aclc_plat_drvdata {
	void __iomem *base;
	u64 physbase;
};

struct txx9aclc_soc_device {
	struct snd_soc_device soc_dev;
	struct platform_device *aclc_pdev;	/* for ioresources, drvdata */
	struct txx9aclc_dmadata dmadata[2];
};

static inline struct txx9aclc_plat_drvdata *txx9aclc_get_plat_drvdata(
	struct txx9aclc_soc_device *sdev)
{
	return platform_get_drvdata(sdev->aclc_pdev);
}

extern struct snd_soc_platform txx9aclc_soc_platform;
extern struct snd_soc_dai txx9aclc_ac97_dai;

#endif /* __TXX9ACLC_H */
