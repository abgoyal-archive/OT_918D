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

/******************************************************************************

    AudioScience HPI driver
    Copyright (C) 1997-2010  AudioScience Inc. <support@audioscience.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of version 2 of the GNU General Public License as
    published by the Free Software Foundation;

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

HPI Operating System function implementation for Linux

(C) Copyright AudioScience Inc. 1997-2003
******************************************************************************/
#define SOURCEFILE_NAME "hpios.c"
#include "hpi_internal.h"
#include "hpidebug.h"
#include <linux/delay.h>
#include <linux/sched.h>

void hpios_delay_micro_seconds(u32 num_micro_sec)
{
	if ((usecs_to_jiffies(num_micro_sec) > 1) && !in_interrupt()) {
		/* MUST NOT SCHEDULE IN INTERRUPT CONTEXT! */
		schedule_timeout_uninterruptible(usecs_to_jiffies
			(num_micro_sec));
	} else if (num_micro_sec <= 2000)
		udelay(num_micro_sec);
	else
		mdelay(num_micro_sec / 1000);

}

void hpios_locked_mem_init(void)
{
}

/** Allocated an area of locked memory for bus master DMA operations.

On error, return -ENOMEM, and *pMemArea.size = 0
*/
u16 hpios_locked_mem_alloc(struct consistent_dma_area *p_mem_area, u32 size,
	struct pci_dev *pdev)
{
	/*?? any benefit in using managed dmam_alloc_coherent? */
	p_mem_area->vaddr =
		dma_alloc_coherent(&pdev->dev, size, &p_mem_area->dma_handle,
		GFP_DMA32 | GFP_KERNEL);

	if (p_mem_area->vaddr) {
		HPI_DEBUG_LOG(DEBUG, "allocated %d bytes, dma 0x%x vma %p\n",
			size, (unsigned int)p_mem_area->dma_handle,
			p_mem_area->vaddr);
		p_mem_area->pdev = &pdev->dev;
		p_mem_area->size = size;
		return 0;
	} else {
		HPI_DEBUG_LOG(WARNING,
			"failed to allocate %d bytes locked memory\n", size);
		p_mem_area->size = 0;
		return -ENOMEM;
	}
}

u16 hpios_locked_mem_free(struct consistent_dma_area *p_mem_area)
{
	if (p_mem_area->size) {
		dma_free_coherent(p_mem_area->pdev, p_mem_area->size,
			p_mem_area->vaddr, p_mem_area->dma_handle);
		HPI_DEBUG_LOG(DEBUG, "freed %lu bytes, dma 0x%x vma %p\n",
			(unsigned long)p_mem_area->size,
			(unsigned int)p_mem_area->dma_handle,
			p_mem_area->vaddr);
		p_mem_area->size = 0;
		return 0;
	} else {
		return 1;
	}
}

void hpios_locked_mem_free_all(void)
{
}
