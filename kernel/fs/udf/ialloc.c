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
 * ialloc.c
 *
 * PURPOSE
 *	Inode allocation handling routines for the OSTA-UDF(tm) filesystem.
 *
 * COPYRIGHT
 *	This file is distributed under the terms of the GNU General Public
 *	License (GPL). Copies of the GPL can be obtained from:
 *		ftp://prep.ai.mit.edu/pub/gnu/GPL
 *	Each contributing author retains all rights to their own work.
 *
 *  (C) 1998-2001 Ben Fennema
 *
 * HISTORY
 *
 *  02/24/99 blf  Created.
 *
 */

#include "udfdecl.h"
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/slab.h>

#include "udf_i.h"
#include "udf_sb.h"

void udf_free_inode(struct inode *inode)
{
	struct super_block *sb = inode->i_sb;
	struct udf_sb_info *sbi = UDF_SB(sb);

	clear_inode(inode);

	mutex_lock(&sbi->s_alloc_mutex);
	if (sbi->s_lvid_bh) {
		struct logicalVolIntegrityDescImpUse *lvidiu =
							udf_sb_lvidiu(sbi);
		if (S_ISDIR(inode->i_mode))
			le32_add_cpu(&lvidiu->numDirs, -1);
		else
			le32_add_cpu(&lvidiu->numFiles, -1);
		udf_updated_lvid(sb);
	}
	mutex_unlock(&sbi->s_alloc_mutex);

	udf_free_blocks(sb, NULL, &UDF_I(inode)->i_location, 0, 1);
}

struct inode *udf_new_inode(struct inode *dir, int mode, int *err)
{
	struct super_block *sb = dir->i_sb;
	struct udf_sb_info *sbi = UDF_SB(sb);
	struct inode *inode;
	int block;
	uint32_t start = UDF_I(dir)->i_location.logicalBlockNum;
	struct udf_inode_info *iinfo;
	struct udf_inode_info *dinfo = UDF_I(dir);

	inode = new_inode(sb);

	if (!inode) {
		*err = -ENOMEM;
		return NULL;
	}
	*err = -ENOSPC;

	iinfo = UDF_I(inode);
	if (UDF_QUERY_FLAG(inode->i_sb, UDF_FLAG_USE_EXTENDED_FE)) {
		iinfo->i_efe = 1;
		if (UDF_VERS_USE_EXTENDED_FE > sbi->s_udfrev)
			sbi->s_udfrev = UDF_VERS_USE_EXTENDED_FE;
		iinfo->i_ext.i_data = kzalloc(inode->i_sb->s_blocksize -
					    sizeof(struct extendedFileEntry),
					    GFP_KERNEL);
	} else {
		iinfo->i_efe = 0;
		iinfo->i_ext.i_data = kzalloc(inode->i_sb->s_blocksize -
					    sizeof(struct fileEntry),
					    GFP_KERNEL);
	}
	if (!iinfo->i_ext.i_data) {
		iput(inode);
		*err = -ENOMEM;
		return NULL;
	}

	block = udf_new_block(dir->i_sb, NULL,
			      dinfo->i_location.partitionReferenceNum,
			      start, err);
	if (*err) {
		iput(inode);
		return NULL;
	}

	mutex_lock(&sbi->s_alloc_mutex);
	if (sbi->s_lvid_bh) {
		struct logicalVolIntegrityDesc *lvid =
			(struct logicalVolIntegrityDesc *)
			sbi->s_lvid_bh->b_data;
		struct logicalVolIntegrityDescImpUse *lvidiu =
							udf_sb_lvidiu(sbi);
		struct logicalVolHeaderDesc *lvhd;
		uint64_t uniqueID;
		lvhd = (struct logicalVolHeaderDesc *)
				(lvid->logicalVolContentsUse);
		if (S_ISDIR(mode))
			le32_add_cpu(&lvidiu->numDirs, 1);
		else
			le32_add_cpu(&lvidiu->numFiles, 1);
		iinfo->i_unique = uniqueID = le64_to_cpu(lvhd->uniqueID);
		if (!(++uniqueID & 0x00000000FFFFFFFFUL))
			uniqueID += 16;
		lvhd->uniqueID = cpu_to_le64(uniqueID);
		udf_updated_lvid(sb);
	}
	mutex_unlock(&sbi->s_alloc_mutex);

	inode_init_owner(inode, dir, mode);

	iinfo->i_location.logicalBlockNum = block;
	iinfo->i_location.partitionReferenceNum =
				dinfo->i_location.partitionReferenceNum;
	inode->i_ino = udf_get_lb_pblock(sb, &iinfo->i_location, 0);
	inode->i_blocks = 0;
	iinfo->i_lenEAttr = 0;
	iinfo->i_lenAlloc = 0;
	iinfo->i_use = 0;
	if (UDF_QUERY_FLAG(inode->i_sb, UDF_FLAG_USE_AD_IN_ICB))
		iinfo->i_alloc_type = ICBTAG_FLAG_AD_IN_ICB;
	else if (UDF_QUERY_FLAG(inode->i_sb, UDF_FLAG_USE_SHORT_AD))
		iinfo->i_alloc_type = ICBTAG_FLAG_AD_SHORT;
	else
		iinfo->i_alloc_type = ICBTAG_FLAG_AD_LONG;
	inode->i_mtime = inode->i_atime = inode->i_ctime =
		iinfo->i_crtime = current_fs_time(inode->i_sb);
	insert_inode_hash(inode);
	mark_inode_dirty(inode);

	*err = 0;
	return inode;
}
