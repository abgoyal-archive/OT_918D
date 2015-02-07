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
 * Copyright (C) 2008-2009 Michal Simek <monstr@monstr.eu>
 * Copyright (C) 2008-2009 PetaLogix
 * Copyright (C) 2007 John Williams
 *
 * Reasonably optimised generic C-code for memcpy on Microblaze
 * This is generic C code to do efficient, alignment-aware memcpy.
 *
 * It is based on demo code originally Copyright 2001 by Intel Corp, taken from
 * http://www.embedded.com/showArticle.jhtml?articleID=19205567
 *
 * Attempts were made, unsuccessfully, to contact the original
 * author of this code (Michael Morrow, Intel).  Below is the original
 * copyright notice.
 *
 * This software has been developed by Intel Corporation.
 * Intel specifically disclaims all warranties, express or
 * implied, and all liability, including consequential and
 * other indirect damages, for the use of this program, including
 * liability for infringement of any proprietary rights,
 * and including the warranties of merchantability and fitness
 * for a particular purpose. Intel does not assume any
 * responsibility for and errors which may appear in this program
 * not any responsibility to update it.
 */

#include <linux/types.h>
#include <linux/stddef.h>
#include <linux/compiler.h>
#include <linux/module.h>

#include <linux/string.h>
#include <asm/system.h>

#ifdef __HAVE_ARCH_MEMCPY
void *memcpy(void *v_dst, const void *v_src, __kernel_size_t c)
{
	const char *src = v_src;
	char *dst = v_dst;
#ifndef CONFIG_OPT_LIB_FUNCTION
	/* Simple, byte oriented memcpy. */
	while (c--)
		*dst++ = *src++;

	return v_dst;
#else
	/* The following code tries to optimize the copy by using unsigned
	 * alignment. This will work fine if both source and destination are
	 * aligned on the same boundary. However, if they are aligned on
	 * different boundaries shifts will be necessary. This might result in
	 * bad performance on MicroBlaze systems without a barrel shifter.
	 */
	const uint32_t *i_src;
	uint32_t *i_dst;

	if (likely(c >= 4)) {
		unsigned  value, buf_hold;

		/* Align the dstination to a word boundry. */
		/* This is done in an endian independant manner. */
		switch ((unsigned long)dst & 3) {
		case 1:
			*dst++ = *src++;
			--c;
		case 2:
			*dst++ = *src++;
			--c;
		case 3:
			*dst++ = *src++;
			--c;
		}

		i_dst = (void *)dst;

		/* Choose a copy scheme based on the source */
		/* alignment relative to dstination. */
		switch ((unsigned long)src & 3) {
		case 0x0:	/* Both byte offsets are aligned */
			i_src  = (const void *)src;

			for (; c >= 4; c -= 4)
				*i_dst++ = *i_src++;

			src  = (const void *)i_src;
			break;
		case 0x1:	/* Unaligned - Off by 1 */
			/* Word align the source */
			i_src = (const void *) ((unsigned)src & ~3);

			/* Load the holding buffer */
			buf_hold = *i_src++ << 8;

			for (; c >= 4; c -= 4) {
				value = *i_src++;
				*i_dst++ = buf_hold | value >> 24;
				buf_hold = value << 8;
			}

			/* Realign the source */
			src = (const void *)i_src;
			src -= 3;
			break;
		case 0x2:	/* Unaligned - Off by 2 */
			/* Word align the source */
			i_src = (const void *) ((unsigned)src & ~3);

			/* Load the holding buffer */
			buf_hold = *i_src++ << 16;

			for (; c >= 4; c -= 4) {
				value = *i_src++;
				*i_dst++ = buf_hold | value >> 16;
				buf_hold = value << 16;
			}

			/* Realign the source */
			src = (const void *)i_src;
			src -= 2;
			break;
		case 0x3:	/* Unaligned - Off by 3 */
			/* Word align the source */
			i_src = (const void *) ((unsigned)src & ~3);

			/* Load the holding buffer */
			buf_hold = *i_src++ << 24;

			for (; c >= 4; c -= 4) {
				value = *i_src++;
				*i_dst++ = buf_hold | value >> 8;
				buf_hold = value << 24;
			}

			/* Realign the source */
			src = (const void *)i_src;
			src -= 1;
			break;
		}
		dst = (void *)i_dst;
	}

	/* Finish off any remaining bytes */
	/* simple fast copy, ... unless a cache boundry is crossed */
	switch (c) {
	case 3:
		*dst++ = *src++;
	case 2:
		*dst++ = *src++;
	case 1:
		*dst++ = *src++;
	}

	return v_dst;
#endif
}
EXPORT_SYMBOL(memcpy);
#endif /* __HAVE_ARCH_MEMCPY */
