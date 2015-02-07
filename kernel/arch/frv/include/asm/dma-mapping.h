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

#ifndef _ASM_DMA_MAPPING_H
#define _ASM_DMA_MAPPING_H

#include <linux/device.h>
#include <asm/cache.h>
#include <asm/cacheflush.h>
#include <asm/scatterlist.h>
#include <asm/io.h>

/*
 * See Documentation/DMA-API.txt for the description of how the
 * following DMA API should work.
 */

#define dma_alloc_noncoherent(d, s, h, f) dma_alloc_coherent(d, s, h, f)
#define dma_free_noncoherent(d, s, v, h) dma_free_coherent(d, s, v, h)

extern unsigned long __nongprelbss dma_coherent_mem_start;
extern unsigned long __nongprelbss dma_coherent_mem_end;

void *dma_alloc_coherent(struct device *dev, size_t size, dma_addr_t *dma_handle, gfp_t gfp);
void dma_free_coherent(struct device *dev, size_t size, void *vaddr, dma_addr_t dma_handle);

extern dma_addr_t dma_map_single(struct device *dev, void *ptr, size_t size,
				 enum dma_data_direction direction);

static inline
void dma_unmap_single(struct device *dev, dma_addr_t dma_addr, size_t size,
		      enum dma_data_direction direction)
{
	BUG_ON(direction == DMA_NONE);
}

extern int dma_map_sg(struct device *dev, struct scatterlist *sg, int nents,
		      enum dma_data_direction direction);

static inline
void dma_unmap_sg(struct device *dev, struct scatterlist *sg, int nhwentries,
	     enum dma_data_direction direction)
{
	BUG_ON(direction == DMA_NONE);
}

extern
dma_addr_t dma_map_page(struct device *dev, struct page *page, unsigned long offset,
			size_t size, enum dma_data_direction direction);

static inline
void dma_unmap_page(struct device *dev, dma_addr_t dma_address, size_t size,
		    enum dma_data_direction direction)
{
	BUG_ON(direction == DMA_NONE);
}


static inline
void dma_sync_single_for_cpu(struct device *dev, dma_addr_t dma_handle, size_t size,
			     enum dma_data_direction direction)
{
}

static inline
void dma_sync_single_for_device(struct device *dev, dma_addr_t dma_handle, size_t size,
				enum dma_data_direction direction)
{
	flush_write_buffers();
}

static inline
void dma_sync_single_range_for_cpu(struct device *dev, dma_addr_t dma_handle,
				   unsigned long offset, size_t size,
				   enum dma_data_direction direction)
{
}

static inline
void dma_sync_single_range_for_device(struct device *dev, dma_addr_t dma_handle,
				      unsigned long offset, size_t size,
				      enum dma_data_direction direction)
{
	flush_write_buffers();
}

static inline
void dma_sync_sg_for_cpu(struct device *dev, struct scatterlist *sg, int nelems,
			 enum dma_data_direction direction)
{
}

static inline
void dma_sync_sg_for_device(struct device *dev, struct scatterlist *sg, int nelems,
			    enum dma_data_direction direction)
{
	flush_write_buffers();
}

static inline
int dma_mapping_error(struct device *dev, dma_addr_t dma_addr)
{
	return 0;
}

static inline
int dma_supported(struct device *dev, u64 mask)
{
        /*
         * we fall back to GFP_DMA when the mask isn't all 1s,
         * so we can't guarantee allocations that must be
         * within a tighter range than GFP_DMA..
         */
        if (mask < 0x00ffffff)
                return 0;

	return 1;
}

static inline
int dma_set_mask(struct device *dev, u64 mask)
{
	if (!dev->dma_mask || !dma_supported(dev, mask))
		return -EIO;

	*dev->dma_mask = mask;

	return 0;
}

static inline
int dma_get_cache_alignment(void)
{
	return 1 << L1_CACHE_SHIFT;
}

#define dma_is_consistent(d, h)	(1)

static inline
void dma_cache_sync(struct device *dev, void *vaddr, size_t size,
		    enum dma_data_direction direction)
{
	flush_write_buffers();
}

#endif  /* _ASM_DMA_MAPPING_H */
