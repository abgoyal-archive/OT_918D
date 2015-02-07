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

#include <linux/mm.h>
#include <asm/mmu_context.h>
#include <asm/cacheflush.h>

/*
 * Write back the dirty D-caches, but not invalidate them.
 *
 * START: Virtual Address (U0, P1, or P3)
 * SIZE: Size of the region.
 */
static void sh4__flush_wback_region(void *start, int size)
{
	reg_size_t aligned_start, v, cnt, end;

	aligned_start = register_align(start);
	v = aligned_start & ~(L1_CACHE_BYTES-1);
	end = (aligned_start + size + L1_CACHE_BYTES-1)
		& ~(L1_CACHE_BYTES-1);
	cnt = (end - v) / L1_CACHE_BYTES;

	while (cnt >= 8) {
		__ocbwb(v); v += L1_CACHE_BYTES;
		__ocbwb(v); v += L1_CACHE_BYTES;
		__ocbwb(v); v += L1_CACHE_BYTES;
		__ocbwb(v); v += L1_CACHE_BYTES;
		__ocbwb(v); v += L1_CACHE_BYTES;
		__ocbwb(v); v += L1_CACHE_BYTES;
		__ocbwb(v); v += L1_CACHE_BYTES;
		__ocbwb(v); v += L1_CACHE_BYTES;
		cnt -= 8;
	}

	while (cnt) {
		__ocbwb(v); v += L1_CACHE_BYTES;
		cnt--;
	}
}

/*
 * Write back the dirty D-caches and invalidate them.
 *
 * START: Virtual Address (U0, P1, or P3)
 * SIZE: Size of the region.
 */
static void sh4__flush_purge_region(void *start, int size)
{
	reg_size_t aligned_start, v, cnt, end;

	aligned_start = register_align(start);
	v = aligned_start & ~(L1_CACHE_BYTES-1);
	end = (aligned_start + size + L1_CACHE_BYTES-1)
		& ~(L1_CACHE_BYTES-1);
	cnt = (end - v) / L1_CACHE_BYTES;

	while (cnt >= 8) {
		__ocbp(v); v += L1_CACHE_BYTES;
		__ocbp(v); v += L1_CACHE_BYTES;
		__ocbp(v); v += L1_CACHE_BYTES;
		__ocbp(v); v += L1_CACHE_BYTES;
		__ocbp(v); v += L1_CACHE_BYTES;
		__ocbp(v); v += L1_CACHE_BYTES;
		__ocbp(v); v += L1_CACHE_BYTES;
		__ocbp(v); v += L1_CACHE_BYTES;
		cnt -= 8;
	}
	while (cnt) {
		__ocbp(v); v += L1_CACHE_BYTES;
		cnt--;
	}
}

/*
 * No write back please
 */
static void sh4__flush_invalidate_region(void *start, int size)
{
	reg_size_t aligned_start, v, cnt, end;

	aligned_start = register_align(start);
	v = aligned_start & ~(L1_CACHE_BYTES-1);
	end = (aligned_start + size + L1_CACHE_BYTES-1)
		& ~(L1_CACHE_BYTES-1);
	cnt = (end - v) / L1_CACHE_BYTES;

	while (cnt >= 8) {
		__ocbi(v); v += L1_CACHE_BYTES;
		__ocbi(v); v += L1_CACHE_BYTES;
		__ocbi(v); v += L1_CACHE_BYTES;
		__ocbi(v); v += L1_CACHE_BYTES;
		__ocbi(v); v += L1_CACHE_BYTES;
		__ocbi(v); v += L1_CACHE_BYTES;
		__ocbi(v); v += L1_CACHE_BYTES;
		__ocbi(v); v += L1_CACHE_BYTES;
		cnt -= 8;
	}

	while (cnt) {
		__ocbi(v); v += L1_CACHE_BYTES;
		cnt--;
	}
}

void __init sh4__flush_region_init(void)
{
	__flush_wback_region		= sh4__flush_wback_region;
	__flush_invalidate_region	= sh4__flush_invalidate_region;
	__flush_purge_region		= sh4__flush_purge_region;
}
