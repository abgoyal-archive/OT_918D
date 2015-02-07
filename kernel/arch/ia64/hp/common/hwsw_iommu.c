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
 * Copyright (c) 2004 Hewlett-Packard Development Company, L.P.
 *   Contributed by David Mosberger-Tang <davidm@hpl.hp.com>
 *
 * This is a pseudo I/O MMU which dispatches to the hardware I/O MMU
 * whenever possible.  We assume that the hardware I/O MMU requires
 * full 32-bit addressability, as is the case, e.g., for HP zx1-based
 * systems (there, the I/O MMU window is mapped at 3-4GB).  If a
 * device doesn't provide full 32-bit addressability, we fall back on
 * the sw I/O TLB.  This is good enough to let us support broken
 * hardware such as soundcards which have a DMA engine that can
 * address only 28 bits.
 */

#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/swiotlb.h>
#include <asm/machvec.h>

extern struct dma_map_ops sba_dma_ops, swiotlb_dma_ops;

/* swiotlb declarations & definitions: */
extern int swiotlb_late_init_with_default_size (size_t size);

/*
 * Note: we need to make the determination of whether or not to use
 * the sw I/O TLB based purely on the device structure.  Anything else
 * would be unreliable or would be too intrusive.
 */
static inline int use_swiotlb(struct device *dev)
{
	return dev && dev->dma_mask &&
		!sba_dma_ops.dma_supported(dev, *dev->dma_mask);
}

struct dma_map_ops *hwsw_dma_get_ops(struct device *dev)
{
	if (use_swiotlb(dev))
		return &swiotlb_dma_ops;
	return &sba_dma_ops;
}
EXPORT_SYMBOL(hwsw_dma_get_ops);

void __init
hwsw_init (void)
{
	/* default to a smallish 2MB sw I/O TLB */
	if (swiotlb_late_init_with_default_size (2 * (1<<20)) != 0) {
#ifdef CONFIG_IA64_GENERIC
		/* Better to have normal DMA than panic */
		printk(KERN_WARNING "%s: Failed to initialize software I/O TLB,"
		       " reverting to hpzx1 platform vector\n", __func__);
		machvec_init("hpzx1");
#else
		panic("Unable to initialize software I/O TLB services");
#endif
	}
}
