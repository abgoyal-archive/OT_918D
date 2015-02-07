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

#include <linux/kmemcheck.h>
#include <linux/module.h>
#include <linux/mm.h>

#include <asm/page.h>
#include <asm/pgtable.h>

#include "pte.h"
#include "shadow.h"

/*
 * Return the shadow address for the given address. Returns NULL if the
 * address is not tracked.
 *
 * We need to be extremely careful not to follow any invalid pointers,
 * because this function can be called for *any* possible address.
 */
void *kmemcheck_shadow_lookup(unsigned long address)
{
	pte_t *pte;
	struct page *page;

	if (!virt_addr_valid(address))
		return NULL;

	pte = kmemcheck_pte_lookup(address);
	if (!pte)
		return NULL;

	page = virt_to_page(address);
	if (!page->shadow)
		return NULL;
	return page->shadow + (address & (PAGE_SIZE - 1));
}

static void mark_shadow(void *address, unsigned int n,
	enum kmemcheck_shadow status)
{
	unsigned long addr = (unsigned long) address;
	unsigned long last_addr = addr + n - 1;
	unsigned long page = addr & PAGE_MASK;
	unsigned long last_page = last_addr & PAGE_MASK;
	unsigned int first_n;
	void *shadow;

	/* If the memory range crosses a page boundary, stop there. */
	if (page == last_page)
		first_n = n;
	else
		first_n = page + PAGE_SIZE - addr;

	shadow = kmemcheck_shadow_lookup(addr);
	if (shadow)
		memset(shadow, status, first_n);

	addr += first_n;
	n -= first_n;

	/* Do full-page memset()s. */
	while (n >= PAGE_SIZE) {
		shadow = kmemcheck_shadow_lookup(addr);
		if (shadow)
			memset(shadow, status, PAGE_SIZE);

		addr += PAGE_SIZE;
		n -= PAGE_SIZE;
	}

	/* Do the remaining page, if any. */
	if (n > 0) {
		shadow = kmemcheck_shadow_lookup(addr);
		if (shadow)
			memset(shadow, status, n);
	}
}

void kmemcheck_mark_unallocated(void *address, unsigned int n)
{
	mark_shadow(address, n, KMEMCHECK_SHADOW_UNALLOCATED);
}

void kmemcheck_mark_uninitialized(void *address, unsigned int n)
{
	mark_shadow(address, n, KMEMCHECK_SHADOW_UNINITIALIZED);
}

/*
 * Fill the shadow memory of the given address such that the memory at that
 * address is marked as being initialized.
 */
void kmemcheck_mark_initialized(void *address, unsigned int n)
{
	mark_shadow(address, n, KMEMCHECK_SHADOW_INITIALIZED);
}
EXPORT_SYMBOL_GPL(kmemcheck_mark_initialized);

void kmemcheck_mark_freed(void *address, unsigned int n)
{
	mark_shadow(address, n, KMEMCHECK_SHADOW_FREED);
}

void kmemcheck_mark_unallocated_pages(struct page *p, unsigned int n)
{
	unsigned int i;

	for (i = 0; i < n; ++i)
		kmemcheck_mark_unallocated(page_address(&p[i]), PAGE_SIZE);
}

void kmemcheck_mark_uninitialized_pages(struct page *p, unsigned int n)
{
	unsigned int i;

	for (i = 0; i < n; ++i)
		kmemcheck_mark_uninitialized(page_address(&p[i]), PAGE_SIZE);
}

void kmemcheck_mark_initialized_pages(struct page *p, unsigned int n)
{
	unsigned int i;

	for (i = 0; i < n; ++i)
		kmemcheck_mark_initialized(page_address(&p[i]), PAGE_SIZE);
}

enum kmemcheck_shadow kmemcheck_shadow_test(void *shadow, unsigned int size)
{
#ifdef CONFIG_KMEMCHECK_PARTIAL_OK
	uint8_t *x;
	unsigned int i;

	x = shadow;

	/*
	 * Make sure _some_ bytes are initialized. Gcc frequently generates
	 * code to access neighboring bytes.
	 */
	for (i = 0; i < size; ++i) {
		if (x[i] == KMEMCHECK_SHADOW_INITIALIZED)
			return x[i];
	}

	return x[0];
#else
	return kmemcheck_shadow_test_all(shadow, size);
#endif
}

enum kmemcheck_shadow kmemcheck_shadow_test_all(void *shadow, unsigned int size)
{
	uint8_t *x;
	unsigned int i;

	x = shadow;

	/* All bytes must be initialized. */
	for (i = 0; i < size; ++i) {
		if (x[i] != KMEMCHECK_SHADOW_INITIALIZED)
			return x[i];
	}

	return x[0];
}

void kmemcheck_shadow_set(void *shadow, unsigned int size)
{
	uint8_t *x;
	unsigned int i;

	x = shadow;
	for (i = 0; i < size; ++i)
		x[i] = KMEMCHECK_SHADOW_INITIALIZED;
}
