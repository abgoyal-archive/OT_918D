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
 * Copyright (c) 2000-2002,2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write the Free Software Foundation,
 * Inc.,  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef __XFS_ALLOC_H__
#define	__XFS_ALLOC_H__

struct xfs_buf;
struct xfs_mount;
struct xfs_perag;
struct xfs_trans;
struct xfs_busy_extent;

/*
 * Freespace allocation types.  Argument to xfs_alloc_[v]extent.
 */
typedef enum xfs_alloctype
{
	XFS_ALLOCTYPE_ANY_AG,		/* allocate anywhere, use rotor */
	XFS_ALLOCTYPE_FIRST_AG,		/* ... start at ag 0 */
	XFS_ALLOCTYPE_START_AG,		/* anywhere, start in this a.g. */
	XFS_ALLOCTYPE_THIS_AG,		/* anywhere in this a.g. */
	XFS_ALLOCTYPE_START_BNO,	/* near this block else anywhere */
	XFS_ALLOCTYPE_NEAR_BNO,		/* in this a.g. and near this block */
	XFS_ALLOCTYPE_THIS_BNO		/* at exactly this block */
} xfs_alloctype_t;

#define XFS_ALLOC_TYPES \
	{ XFS_ALLOCTYPE_ANY_AG,		"ANY_AG" }, \
	{ XFS_ALLOCTYPE_FIRST_AG,	"FIRST_AG" }, \
	{ XFS_ALLOCTYPE_START_AG,	"START_AG" }, \
	{ XFS_ALLOCTYPE_THIS_AG,	"THIS_AG" }, \
	{ XFS_ALLOCTYPE_START_BNO,	"START_BNO" }, \
	{ XFS_ALLOCTYPE_NEAR_BNO,	"NEAR_BNO" }, \
	{ XFS_ALLOCTYPE_THIS_BNO,	"THIS_BNO" }

/*
 * Flags for xfs_alloc_fix_freelist.
 */
#define	XFS_ALLOC_FLAG_TRYLOCK	0x00000001  /* use trylock for buffer locking */
#define	XFS_ALLOC_FLAG_FREEING	0x00000002  /* indicate caller is freeing extents*/

/*
 * In order to avoid ENOSPC-related deadlock caused by
 * out-of-order locking of AGF buffer (PV 947395), we place
 * constraints on the relationship among actual allocations for
 * data blocks, freelist blocks, and potential file data bmap
 * btree blocks. However, these restrictions may result in no
 * actual space allocated for a delayed extent, for example, a data
 * block in a certain AG is allocated but there is no additional
 * block for the additional bmap btree block due to a split of the
 * bmap btree of the file. The result of this may lead to an
 * infinite loop in xfssyncd when the file gets flushed to disk and
 * all delayed extents need to be actually allocated. To get around
 * this, we explicitly set aside a few blocks which will not be
 * reserved in delayed allocation. Considering the minimum number of
 * needed freelist blocks is 4 fsbs _per AG_, a potential split of file's bmap
 * btree requires 1 fsb, so we set the number of set-aside blocks
 * to 4 + 4*agcount.
 */
#define XFS_ALLOC_SET_ASIDE(mp)  (4 + ((mp)->m_sb.sb_agcount * 4))

/*
 * Argument structure for xfs_alloc routines.
 * This is turned into a structure to avoid having 20 arguments passed
 * down several levels of the stack.
 */
typedef struct xfs_alloc_arg {
	struct xfs_trans *tp;		/* transaction pointer */
	struct xfs_mount *mp;		/* file system mount point */
	struct xfs_buf	*agbp;		/* buffer for a.g. freelist header */
	struct xfs_perag *pag;		/* per-ag struct for this agno */
	xfs_fsblock_t	fsbno;		/* file system block number */
	xfs_agnumber_t	agno;		/* allocation group number */
	xfs_agblock_t	agbno;		/* allocation group-relative block # */
	xfs_extlen_t	minlen;		/* minimum size of extent */
	xfs_extlen_t	maxlen;		/* maximum size of extent */
	xfs_extlen_t	mod;		/* mod value for extent size */
	xfs_extlen_t	prod;		/* prod value for extent size */
	xfs_extlen_t	minleft;	/* min blocks must be left after us */
	xfs_extlen_t	total;		/* total blocks needed in xaction */
	xfs_extlen_t	alignment;	/* align answer to multiple of this */
	xfs_extlen_t	minalignslop;	/* slop for minlen+alignment calcs */
	xfs_extlen_t	len;		/* output: actual size of extent */
	xfs_alloctype_t	type;		/* allocation type XFS_ALLOCTYPE_... */
	xfs_alloctype_t	otype;		/* original allocation type */
	char		wasdel;		/* set if allocation was prev delayed */
	char		wasfromfl;	/* set if allocation is from freelist */
	char		isfl;		/* set if is freelist blocks - !acctg */
	char		userdata;	/* set if this is user data */
	xfs_fsblock_t	firstblock;	/* io first block allocated */
} xfs_alloc_arg_t;

/*
 * Defines for userdata
 */
#define XFS_ALLOC_USERDATA		1	/* allocation is for user data*/
#define XFS_ALLOC_INITIAL_USER_DATA	2	/* special case start of file */

/*
 * Find the length of the longest extent in an AG.
 */
xfs_extlen_t
xfs_alloc_longest_free_extent(struct xfs_mount *mp,
		struct xfs_perag *pag);

#ifdef __KERNEL__

void
xfs_alloc_busy_insert(xfs_trans_t *tp,
		xfs_agnumber_t agno,
		xfs_agblock_t bno,
		xfs_extlen_t len);

void
xfs_alloc_busy_clear(struct xfs_mount *mp, struct xfs_busy_extent *busyp);

#endif	/* __KERNEL__ */

/*
 * Compute and fill in value of m_ag_maxlevels.
 */
void
xfs_alloc_compute_maxlevels(
	struct xfs_mount	*mp);	/* file system mount structure */

/*
 * Get a block from the freelist.
 * Returns with the buffer for the block gotten.
 */
int				/* error */
xfs_alloc_get_freelist(
	struct xfs_trans *tp,	/* transaction pointer */
	struct xfs_buf	*agbp,	/* buffer containing the agf structure */
	xfs_agblock_t	*bnop,	/* block address retrieved from freelist */
	int		btreeblk); /* destination is a AGF btree */

/*
 * Log the given fields from the agf structure.
 */
void
xfs_alloc_log_agf(
	struct xfs_trans *tp,	/* transaction pointer */
	struct xfs_buf	*bp,	/* buffer for a.g. freelist header */
	int		fields);/* mask of fields to be logged (XFS_AGF_...) */

/*
 * Interface for inode allocation to force the pag data to be initialized.
 */
int				/* error */
xfs_alloc_pagf_init(
	struct xfs_mount *mp,	/* file system mount structure */
	struct xfs_trans *tp,	/* transaction pointer */
	xfs_agnumber_t	agno,	/* allocation group number */
	int		flags);	/* XFS_ALLOC_FLAGS_... */

/*
 * Put the block on the freelist for the allocation group.
 */
int				/* error */
xfs_alloc_put_freelist(
	struct xfs_trans *tp,	/* transaction pointer */
	struct xfs_buf	*agbp,	/* buffer for a.g. freelist header */
	struct xfs_buf	*agflbp,/* buffer for a.g. free block array */
	xfs_agblock_t	bno,	/* block being freed */
	int		btreeblk); /* owner was a AGF btree */

/*
 * Read in the allocation group header (free/alloc section).
 */
int					/* error  */
xfs_alloc_read_agf(
	struct xfs_mount *mp,		/* mount point structure */
	struct xfs_trans *tp,		/* transaction pointer */
	xfs_agnumber_t	agno,		/* allocation group number */
	int		flags,		/* XFS_ALLOC_FLAG_... */
	struct xfs_buf	**bpp);		/* buffer for the ag freelist header */

/*
 * Allocate an extent (variable-size).
 */
int				/* error */
xfs_alloc_vextent(
	xfs_alloc_arg_t	*args);	/* allocation argument structure */

/*
 * Free an extent.
 */
int				/* error */
xfs_free_extent(
	struct xfs_trans *tp,	/* transaction pointer */
	xfs_fsblock_t	bno,	/* starting block number of extent */
	xfs_extlen_t	len);	/* length of extent */

#endif	/* __XFS_ALLOC_H__ */
