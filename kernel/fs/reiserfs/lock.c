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

#include <linux/reiserfs_fs.h>
#include <linux/mutex.h>

/*
 * The previous reiserfs locking scheme was heavily based on
 * the tricky properties of the Bkl:
 *
 * - it was acquired recursively by a same task
 * - the performances relied on the release-while-schedule() property
 *
 * Now that we replace it by a mutex, we still want to keep the same
 * recursive property to avoid big changes in the code structure.
 * We use our own lock_owner here because the owner field on a mutex
 * is only available in SMP or mutex debugging, also we only need this field
 * for this mutex, no need for a system wide mutex facility.
 *
 * Also this lock is often released before a call that could block because
 * reiserfs performances were partialy based on the release while schedule()
 * property of the Bkl.
 */
void reiserfs_write_lock(struct super_block *s)
{
	struct reiserfs_sb_info *sb_i = REISERFS_SB(s);

	if (sb_i->lock_owner != current) {
		mutex_lock(&sb_i->lock);
		sb_i->lock_owner = current;
	}

	/* No need to protect it, only the current task touches it */
	sb_i->lock_depth++;
}

void reiserfs_write_unlock(struct super_block *s)
{
	struct reiserfs_sb_info *sb_i = REISERFS_SB(s);

	/*
	 * Are we unlocking without even holding the lock?
	 * Such a situation must raise a BUG() if we don't want
	 * to corrupt the data.
	 */
	BUG_ON(sb_i->lock_owner != current);

	if (--sb_i->lock_depth == -1) {
		sb_i->lock_owner = NULL;
		mutex_unlock(&sb_i->lock);
	}
}

/*
 * If we already own the lock, just exit and don't increase the depth.
 * Useful when we don't want to lock more than once.
 *
 * We always return the lock_depth we had before calling
 * this function.
 */
int reiserfs_write_lock_once(struct super_block *s)
{
	struct reiserfs_sb_info *sb_i = REISERFS_SB(s);

	if (sb_i->lock_owner != current) {
		mutex_lock(&sb_i->lock);
		sb_i->lock_owner = current;
		return sb_i->lock_depth++;
	}

	return sb_i->lock_depth;
}

void reiserfs_write_unlock_once(struct super_block *s, int lock_depth)
{
	if (lock_depth == -1)
		reiserfs_write_unlock(s);
}

/*
 * Utility function to force a BUG if it is called without the superblock
 * write lock held.  caller is the string printed just before calling BUG()
 */
void reiserfs_check_lock_depth(struct super_block *sb, char *caller)
{
	struct reiserfs_sb_info *sb_i = REISERFS_SB(sb);

	if (sb_i->lock_depth < 0)
		reiserfs_panic(sb, "%s called without kernel lock held %d",
			       caller);
}

#ifdef CONFIG_REISERFS_CHECK
void reiserfs_lock_check_recursive(struct super_block *sb)
{
	struct reiserfs_sb_info *sb_i = REISERFS_SB(sb);

	WARN_ONCE((sb_i->lock_depth > 0), "Unwanted recursive reiserfs lock!\n");
}
#endif
