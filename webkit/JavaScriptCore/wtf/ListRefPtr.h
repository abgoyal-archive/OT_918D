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
 *  Copyright (C) 2005, 2006, 2008 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */

#ifndef WTF_ListRefPtr_h
#define WTF_ListRefPtr_h

#include <wtf/RefPtr.h>

namespace WTF {

    // Specialized version of RefPtr desgined for use in singly-linked lists. 
    // Derefs the list iteratively to avoid recursive derefing that can overflow the stack.
    template <typename T> class ListRefPtr : public RefPtr<T> {
    public:
        ListRefPtr() : RefPtr<T>() {}
        ListRefPtr(T* ptr) : RefPtr<T>(ptr) {}
        ListRefPtr(const RefPtr<T>& o) : RefPtr<T>(o) {}
        // see comment in PassRefPtr.h for why this takes const reference
        template <typename U> ListRefPtr(const PassRefPtr<U>& o) : RefPtr<T>(o) {}
        
        ~ListRefPtr();
        
        ListRefPtr& operator=(T* optr) { RefPtr<T>::operator=(optr); return *this; }
        ListRefPtr& operator=(const RefPtr<T>& o) { RefPtr<T>::operator=(o); return *this; }
        ListRefPtr& operator=(const PassRefPtr<T>& o) { RefPtr<T>::operator=(o); return *this; }
        template <typename U> ListRefPtr& operator=(const RefPtr<U>& o) { RefPtr<T>::operator=(o); return *this; }
        template <typename U> ListRefPtr& operator=(const PassRefPtr<U>& o) { RefPtr<T>::operator=(o); return *this; }
    };

    // Remove inline for winscw compiler to prevent the compiler agressively resolving
    // T::ref() in RefPtr<T>'s copy constructor.  The bug is reported at:
    // https://xdabug001.ext.nokia.com/bugzilla/show_bug.cgi?id=9812.
    template <typename T> 
#if !COMPILER(WINSCW)
    inline
#endif
    ListRefPtr<T>::~ListRefPtr()
    {
        RefPtr<T> reaper = this->release();
        while (reaper && reaper->hasOneRef())
            reaper = reaper->releaseNext(); // implicitly protects reaper->next, then derefs reaper
    }

    template <typename T> inline T* getPtr(const ListRefPtr<T>& p)
    {
        return p.get();
    }

} // namespace WTF

using WTF::ListRefPtr;

#endif // WTF_ListRefPtr_h
