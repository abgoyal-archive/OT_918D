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

/**************************************************************************
 *
 * Copyright 2010 Pauli Nieminen.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *
 **************************************************************************/
/*
 * Multipart buffer for coping data which is larger than the page size.
 *
 * Authors:
 * Pauli Nieminen <suokkos-at-gmail-dot-com>
 */

#ifndef _DRM_BUFFER_H_
#define _DRM_BUFFER_H_

#include "drmP.h"

struct drm_buffer {
	int iterator;
	int size;
	char *data[];
};


/**
 * Return the index of page that buffer is currently pointing at.
 */
static inline int drm_buffer_page(struct drm_buffer *buf)
{
	return buf->iterator / PAGE_SIZE;
}
/**
 * Return the index of the current byte in the page
 */
static inline int drm_buffer_index(struct drm_buffer *buf)
{
	return buf->iterator & (PAGE_SIZE - 1);
}
/**
 * Return number of bytes that is left to process
 */
static inline int drm_buffer_unprocessed(struct drm_buffer *buf)
{
	return buf->size - buf->iterator;
}

/**
 * Advance the buffer iterator number of bytes that is given.
 */
static inline void drm_buffer_advance(struct drm_buffer *buf, int bytes)
{
	buf->iterator += bytes;
}

/**
 * Allocate the drm buffer object.
 *
 *   buf: A pointer to a pointer where the object is stored.
 *   size: The number of bytes to allocate.
 */
extern int drm_buffer_alloc(struct drm_buffer **buf, int size);

/**
 * Copy the user data to the begin of the buffer and reset the processing
 * iterator.
 *
 *   user_data: A pointer the data that is copied to the buffer.
 *   size: The Number of bytes to copy.
 */
extern int drm_buffer_copy_from_user(struct drm_buffer *buf,
		void __user *user_data, int size);

/**
 * Free the drm buffer object
 */
extern void drm_buffer_free(struct drm_buffer *buf);

/**
 * Read an object from buffer that may be split to multiple parts. If object
 * is not split function just returns the pointer to object in buffer. But in
 * case of split object data is copied to given stack object that is suplied
 * by caller.
 *
 * The processing location of the buffer is also advanced to the next byte
 * after the object.
 *
 *   objsize: The size of the objet in bytes.
 *   stack_obj: A pointer to a memory location where object can be copied.
 */
extern void *drm_buffer_read_object(struct drm_buffer *buf,
		int objsize, void *stack_obj);

/**
 * Returns the pointer to the dword which is offset number of elements from the
 * current processing location.
 *
 * Caller must make sure that dword is not split in the buffer. This
 * requirement is easily met if all the sizes of objects in buffer are
 * multiples of dword and PAGE_SIZE is multiple dword.
 *
 * Call to this function doesn't change the processing location.
 *
 *   offset: The index of the dword relative to the internat iterator.
 */
static inline void *drm_buffer_pointer_to_dword(struct drm_buffer *buffer,
		int offset)
{
	int iter = buffer->iterator + offset * 4;
	return &buffer->data[iter / PAGE_SIZE][iter & (PAGE_SIZE - 1)];
}
/**
 * Returns the pointer to the dword which is offset number of elements from
 * the current processing location.
 *
 * Call to this function doesn't change the processing location.
 *
 *   offset: The index of the byte relative to the internat iterator.
 */
static inline void *drm_buffer_pointer_to_byte(struct drm_buffer *buffer,
		int offset)
{
	int iter = buffer->iterator + offset;
	return &buffer->data[iter / PAGE_SIZE][iter & (PAGE_SIZE - 1)];
}

#endif
