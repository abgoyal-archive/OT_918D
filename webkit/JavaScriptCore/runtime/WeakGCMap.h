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
 * Copyright (C) 2009 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef WeakGCMap_h
#define WeakGCMap_h

#include "Collector.h"
#include <wtf/HashMap.h>

namespace JSC {

class JSCell;

// A HashMap whose get() function returns emptyValue() for cells awaiting destruction.
template<typename KeyType, typename MappedType>
class WeakGCMap : public FastAllocBase {
    /*
    Invariants:
        * A value enters the WeakGCMap marked. (Guaranteed by set().)
        * A value that becomes unmarked leaves the WeakGCMap before being recycled. (Guaranteed by the value's destructor removing it from the WeakGCMap.)
        * A value that becomes unmarked leaves the WeakGCMap before becoming marked again. (Guaranteed by all destructors running before the mark phase begins.)
        * During the mark phase, all values in the WeakGCMap are valid. (Guaranteed by all destructors running before the mark phase begins.)
    */

public:
    typedef typename HashMap<KeyType, MappedType>::iterator iterator;
    typedef typename HashMap<KeyType, MappedType>::const_iterator const_iterator;
    
    bool isEmpty() { return m_map.isEmpty(); }

    MappedType get(const KeyType& key) const;
    pair<iterator, bool> set(const KeyType&, const MappedType&); 
    MappedType take(const KeyType& key);

    // These unchecked functions provide access to a value even if the value's
    // mark bit is not set. This is used, among other things, to retrieve values
    // during the GC mark phase, which begins by clearing all mark bits.

    MappedType uncheckedGet(const KeyType& key) const { return m_map.get(key); }
    bool uncheckedRemove(const KeyType&, const MappedType&);

    iterator uncheckedBegin() { return m_map.begin(); }
    iterator uncheckedEnd() { return m_map.end(); }

    const_iterator uncheckedBegin() const { return m_map.begin(); }
    const_iterator uncheckedEnd() const { return m_map.end(); }

private:
    HashMap<KeyType, MappedType> m_map;
};

template<typename KeyType, typename MappedType>
inline MappedType WeakGCMap<KeyType, MappedType>::get(const KeyType& key) const
{
    MappedType result = m_map.get(key);
    if (result == HashTraits<MappedType>::emptyValue())
        return result;
    if (!Heap::isCellMarked(result))
        return HashTraits<MappedType>::emptyValue();
    return result;
}

template<typename KeyType, typename MappedType>
MappedType WeakGCMap<KeyType, MappedType>::take(const KeyType& key)
{
    MappedType result = m_map.take(key);
    if (result == HashTraits<MappedType>::emptyValue())
        return result;
    if (!Heap::isCellMarked(result))
        return HashTraits<MappedType>::emptyValue();
    return result;
}

template<typename KeyType, typename MappedType>
pair<typename HashMap<KeyType, MappedType>::iterator, bool> WeakGCMap<KeyType, MappedType>::set(const KeyType& key, const MappedType& value)
{
    Heap::markCell(value); // If value is newly allocated, it's not marked, so mark it now.
    pair<iterator, bool> result = m_map.add(key, value);
    if (!result.second) { // pre-existing entry
        result.second = !Heap::isCellMarked(result.first->second);
        result.first->second = value;
    }
    return result;
}

template<typename KeyType, typename MappedType>
bool WeakGCMap<KeyType, MappedType>::uncheckedRemove(const KeyType& key, const MappedType& value)
{
    iterator it = m_map.find(key);
    if (it == m_map.end())
        return false;
    if (it->second != value)
        return false;
    m_map.remove(it);
    return true;
}

} // namespace JSC

#endif // WeakGCMap_h
