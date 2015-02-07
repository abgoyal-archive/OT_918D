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
 * driver/dma/coh901318_lli.h
 *
 * Copyright (C) 2007-2009 ST-Ericsson
 * License terms: GNU General Public License (GPL) version 2
 * Support functions for handling lli for coh901318
 * Author: Per Friden <per.friden@stericsson.com>
 */

#ifndef COH901318_LLI_H
#define COH901318_LLI_H

#include <mach/coh901318.h>

struct device;

struct coh901318_pool {
	spinlock_t lock;
	struct dma_pool *dmapool;
	struct device *dev;

#ifdef CONFIG_DEBUG_FS
	int debugfs_pool_counter;
#endif
};

struct device;
/**
 * coh901318_pool_create() - Creates an dma pool for lli:s
 * @pool: pool handle
 * @dev: dma device
 * @lli_nbr: number of lli:s in the pool
 * @algin: address alignemtn of lli:s
 * returns 0 on success otherwise none zero
 */
int coh901318_pool_create(struct coh901318_pool *pool,
			  struct device *dev,
			  size_t lli_nbr, size_t align);

/**
 * coh901318_pool_destroy() - Destroys the dma pool
 * @pool: pool handle
 * returns 0 on success otherwise none zero
 */
int coh901318_pool_destroy(struct coh901318_pool *pool);

/**
 * coh901318_lli_alloc() - Allocates a linked list
 *
 * @pool: pool handle
 * @len: length to list
 * return: none NULL if success otherwise NULL
 */
struct coh901318_lli *
coh901318_lli_alloc(struct coh901318_pool *pool,
		    unsigned int len);

/**
 * coh901318_lli_free() - Returns the linked list items to the pool
 * @pool: pool handle
 * @lli: reference to lli pointer to be freed
 */
void coh901318_lli_free(struct coh901318_pool *pool,
			struct coh901318_lli **lli);

/**
 * coh901318_lli_fill_memcpy() - Prepares the lli:s for dma memcpy
 * @pool: pool handle
 * @lli: allocated lli
 * @src: src address
 * @size: transfer size
 * @dst: destination address
 * @ctrl_chained: ctrl for chained lli
 * @ctrl_last: ctrl for the last lli
 * returns number of CPU interrupts for the lli, negative on error.
 */
int
coh901318_lli_fill_memcpy(struct coh901318_pool *pool,
			  struct coh901318_lli *lli,
			  dma_addr_t src, unsigned int size,
			  dma_addr_t dst, u32 ctrl_chained, u32 ctrl_last);

/**
 * coh901318_lli_fill_single() - Prepares the lli:s for dma single transfer
 * @pool: pool handle
 * @lli: allocated lli
 * @buf: transfer buffer
 * @size: transfer size
 * @dev_addr: address of periphal
 * @ctrl_chained: ctrl for chained lli
 * @ctrl_last: ctrl for the last lli
 * @dir: direction of transfer (to or from device)
 * returns number of CPU interrupts for the lli, negative on error.
 */
int
coh901318_lli_fill_single(struct coh901318_pool *pool,
			  struct coh901318_lli *lli,
			  dma_addr_t buf, unsigned int size,
			  dma_addr_t dev_addr, u32 ctrl_chained, u32 ctrl_last,
			  enum dma_data_direction dir);

/**
 * coh901318_lli_fill_single() - Prepares the lli:s for dma scatter list transfer
 * @pool: pool handle
 * @lli: allocated lli
 * @sg: scatter gather list
 * @nents: number of entries in sg
 * @dev_addr: address of periphal
 * @ctrl_chained: ctrl for chained lli
 * @ctrl: ctrl of middle lli
 * @ctrl_last: ctrl for the last lli
 * @dir: direction of transfer (to or from device)
 * @ctrl_irq_mask: ctrl mask for CPU interrupt
 * returns number of CPU interrupts for the lli, negative on error.
 */
int
coh901318_lli_fill_sg(struct coh901318_pool *pool,
		      struct coh901318_lli *lli,
		      struct scatterlist *sg, unsigned int nents,
		      dma_addr_t dev_addr, u32 ctrl_chained,
		      u32 ctrl, u32 ctrl_last,
		      enum dma_data_direction dir, u32 ctrl_irq_mask);

#endif /* COH901318_LLI_H */
