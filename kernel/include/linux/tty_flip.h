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

#ifndef _LINUX_TTY_FLIP_H
#define _LINUX_TTY_FLIP_H

extern int tty_buffer_request_room(struct tty_struct *tty, size_t size);
extern int tty_insert_flip_string_flags(struct tty_struct *tty, const unsigned char *chars, const char *flags, size_t size);
extern int tty_insert_flip_string_fixed_flag(struct tty_struct *tty, const unsigned char *chars, char flag, size_t size);
extern int tty_prepare_flip_string(struct tty_struct *tty, unsigned char **chars, size_t size);
extern int tty_prepare_flip_string_flags(struct tty_struct *tty, unsigned char **chars, char **flags, size_t size);
void tty_schedule_flip(struct tty_struct *tty);

static inline int tty_insert_flip_char(struct tty_struct *tty,
					unsigned char ch, char flag)
{
	struct tty_buffer *tb = tty->buf.tail;
	if (tb && tb->used < tb->size) {
		tb->flag_buf_ptr[tb->used] = flag;
		tb->char_buf_ptr[tb->used++] = ch;
		return 1;
	}
	return tty_insert_flip_string_flags(tty, &ch, &flag, 1);
}

static inline int tty_insert_flip_string(struct tty_struct *tty, const unsigned char *chars, size_t size)
{
	return tty_insert_flip_string_fixed_flag(tty, chars, TTY_NORMAL, size);
}

#endif /* _LINUX_TTY_FLIP_H */
