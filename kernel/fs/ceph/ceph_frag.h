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

#ifndef _FS_CEPH_FRAG_H
#define _FS_CEPH_FRAG_H

/*
 * "Frags" are a way to describe a subset of a 32-bit number space,
 * using a mask and a value to match against that mask.  Any given frag
 * (subset of the number space) can be partitioned into 2^n sub-frags.
 *
 * Frags are encoded into a 32-bit word:
 *   8 upper bits = "bits"
 *  24 lower bits = "value"
 * (We could go to 5+27 bits, but who cares.)
 *
 * We use the _most_ significant bits of the 24 bit value.  This makes
 * values logically sort.
 *
 * Unfortunately, because the "bits" field is still in the high bits, we
 * can't sort encoded frags numerically.  However, it does allow you
 * to feed encoded frags as values into frag_contains_value.
 */
static inline __u32 ceph_frag_make(__u32 b, __u32 v)
{
	return (b << 24) |
		(v & (0xffffffu << (24-b)) & 0xffffffu);
}
static inline __u32 ceph_frag_bits(__u32 f)
{
	return f >> 24;
}
static inline __u32 ceph_frag_value(__u32 f)
{
	return f & 0xffffffu;
}
static inline __u32 ceph_frag_mask(__u32 f)
{
	return (0xffffffu << (24-ceph_frag_bits(f))) & 0xffffffu;
}
static inline __u32 ceph_frag_mask_shift(__u32 f)
{
	return 24 - ceph_frag_bits(f);
}

static inline int ceph_frag_contains_value(__u32 f, __u32 v)
{
	return (v & ceph_frag_mask(f)) == ceph_frag_value(f);
}
static inline int ceph_frag_contains_frag(__u32 f, __u32 sub)
{
	/* is sub as specific as us, and contained by us? */
	return ceph_frag_bits(sub) >= ceph_frag_bits(f) &&
	       (ceph_frag_value(sub) & ceph_frag_mask(f)) == ceph_frag_value(f);
}

static inline __u32 ceph_frag_parent(__u32 f)
{
	return ceph_frag_make(ceph_frag_bits(f) - 1,
			 ceph_frag_value(f) & (ceph_frag_mask(f) << 1));
}
static inline int ceph_frag_is_left_child(__u32 f)
{
	return ceph_frag_bits(f) > 0 &&
		(ceph_frag_value(f) & (0x1000000 >> ceph_frag_bits(f))) == 0;
}
static inline int ceph_frag_is_right_child(__u32 f)
{
	return ceph_frag_bits(f) > 0 &&
		(ceph_frag_value(f) & (0x1000000 >> ceph_frag_bits(f))) == 1;
}
static inline __u32 ceph_frag_sibling(__u32 f)
{
	return ceph_frag_make(ceph_frag_bits(f),
		      ceph_frag_value(f) ^ (0x1000000 >> ceph_frag_bits(f)));
}
static inline __u32 ceph_frag_left_child(__u32 f)
{
	return ceph_frag_make(ceph_frag_bits(f)+1, ceph_frag_value(f));
}
static inline __u32 ceph_frag_right_child(__u32 f)
{
	return ceph_frag_make(ceph_frag_bits(f)+1,
	      ceph_frag_value(f) | (0x1000000 >> (1+ceph_frag_bits(f))));
}
static inline __u32 ceph_frag_make_child(__u32 f, int by, int i)
{
	int newbits = ceph_frag_bits(f) + by;
	return ceph_frag_make(newbits,
			 ceph_frag_value(f) | (i << (24 - newbits)));
}
static inline int ceph_frag_is_leftmost(__u32 f)
{
	return ceph_frag_value(f) == 0;
}
static inline int ceph_frag_is_rightmost(__u32 f)
{
	return ceph_frag_value(f) == ceph_frag_mask(f);
}
static inline __u32 ceph_frag_next(__u32 f)
{
	return ceph_frag_make(ceph_frag_bits(f),
			 ceph_frag_value(f) + (0x1000000 >> ceph_frag_bits(f)));
}

/*
 * comparator to sort frags logically, as when traversing the
 * number space in ascending order...
 */
int ceph_frag_compare(__u32 a, __u32 b);

#endif
