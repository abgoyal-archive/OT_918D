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
 * Copyright (C) 2003 Apple Computer, Inc.
 *
 * Portions are Copyright (C) 1998 Netscape Communications Corporation.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Alternatively, the contents of this file may be used under the terms
 * of either the Mozilla Public License Version 1.1, found at
 * http://www.mozilla.org/MPL/ (the "MPL") or the GNU General Public
 * License Version 2.0, found at http://www.fsf.org/copyleft/gpl.html
 * (the "GPL"), in which case the provisions of the MPL or the GPL are
 * applicable instead of those above.  If you wish to allow use of your
 * version of this file only under the terms of one of those two
 * licenses (the MPL or the GPL) and not to allow others to use your
 * version of this file under the LGPL, indicate your decision by
 * deletingthe provisions above and replace them with the notice and
 * other provisions required by the MPL or the GPL, as the case may be.
 * If you do not delete the provisions above, a recipient may use your
 * version of this file under any of the LGPL, the MPL or the GPL.
 */

#include "config.h"
#include "RenderArena.h"

#include <stdlib.h>
#include <string.h>
#include <wtf/Assertions.h>

#define ROUNDUP(x, y) ((((x)+((y)-1))/(y))*(y))

namespace WebCore {

#ifndef NDEBUG

const int signature = 0xDBA00AEA;
const int signatureDead = 0xDBA00AED;

typedef struct {
    RenderArena* arena;
    size_t size;
    int signature;
} RenderArenaDebugHeader;

#endif

RenderArena::RenderArena(unsigned arenaSize)
{
    // Initialize the arena pool
    INIT_ARENA_POOL(&m_pool, "RenderArena", arenaSize);

    // Zero out the recyclers array
    memset(m_recyclers, 0, sizeof(m_recyclers));
}

RenderArena::~RenderArena()
{
    FinishArenaPool(&m_pool);
}

void* RenderArena::allocate(size_t size)
{
#ifndef NDEBUG
    // Use standard malloc so that memory debugging tools work.
    ASSERT(this);
    void* block = ::malloc(sizeof(RenderArenaDebugHeader) + size);
    RenderArenaDebugHeader* header = static_cast<RenderArenaDebugHeader*>(block);
    header->arena = this;
    header->size = size;
    header->signature = signature;
    return header + 1;
#else
    void* result = 0;

    // Ensure we have correct alignment for pointers.  Important for Tru64
    size = ROUNDUP(size, sizeof(void*));

    // Check recyclers first
    if (size < gMaxRecycledSize) {
        const int index = size >> 2;

        result = m_recyclers[index];
        if (result) {
            // Need to move to the next object
            void* next = *((void**)result);
            m_recyclers[index] = next;
        }
    }

    if (!result) {
        // Allocate a new chunk from the arena
        ARENA_ALLOCATE(result, &m_pool, size);
    }

    return result;
#endif
}

void RenderArena::free(size_t size, void* ptr)
{
#ifndef NDEBUG
    // Use standard free so that memory debugging tools work.
    RenderArenaDebugHeader* header = static_cast<RenderArenaDebugHeader*>(ptr) - 1;
    ASSERT(header->signature == signature);
    ASSERT_UNUSED(size, header->size == size);
    ASSERT(header->arena == this);
    header->signature = signatureDead;
    ::free(header);
#else
    // Ensure we have correct alignment for pointers.  Important for Tru64
    size = ROUNDUP(size, sizeof(void*));

    // See if it's a size that we recycle
    if (size < gMaxRecycledSize) {
        const int index = size >> 2;
        void* currentTop = m_recyclers[index];
        m_recyclers[index] = ptr;
        *((void**)ptr) = currentTop;
    }
#endif
}

#ifdef ANDROID_INSTRUMENT
size_t RenderArena::reportPoolSize() const
{
    return ReportPoolSize(&m_pool);
}
#endif

} // namespace WebCore