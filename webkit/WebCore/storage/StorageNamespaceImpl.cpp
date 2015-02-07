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
 * Copyright (C) 2008 Apple Inc. All Rights Reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "StorageNamespaceImpl.h"

#if ENABLE(DOM_STORAGE)

#include "SecurityOriginHash.h"
#include "StringHash.h"
#include "StorageAreaImpl.h"
#include "StorageMap.h"
#include "StorageSyncManager.h"
#include <wtf/StdLibExtras.h>

#ifdef ANDROID
#include "Page.h"
#endif

namespace WebCore {

typedef HashMap<String, StorageNamespace*> LocalStorageNamespaceMap;

static LocalStorageNamespaceMap& localStorageNamespaceMap()
{
    DEFINE_STATIC_LOCAL(LocalStorageNamespaceMap, localStorageNamespaceMap, ());
    return localStorageNamespaceMap;
}

PassRefPtr<StorageNamespace> StorageNamespaceImpl::localStorageNamespace(const String& path, unsigned quota)
{
    const String lookupPath = path.isNull() ? String("") : path;
    LocalStorageNamespaceMap::iterator it = localStorageNamespaceMap().find(lookupPath);
    if (it == localStorageNamespaceMap().end()) {
        RefPtr<StorageNamespace> storageNamespace = adoptRef(new StorageNamespaceImpl(LocalStorage, lookupPath, quota));
        localStorageNamespaceMap().set(lookupPath, storageNamespace.get());
        return storageNamespace.release();
    }

    return it->second;
}

PassRefPtr<StorageNamespace> StorageNamespaceImpl::sessionStorageNamespace()
{
    return adoptRef(new StorageNamespaceImpl(SessionStorage, String(), StorageMap::noQuota));
}

StorageNamespaceImpl::StorageNamespaceImpl(StorageType storageType, const String& path, unsigned quota)
    : m_storageType(storageType)
    , m_path(path.crossThreadString())
    , m_syncManager(0)
    , m_quota(quota)
    , m_isShutdown(false)
{
    if (m_storageType == LocalStorage && !m_path.isEmpty())
        m_syncManager = StorageSyncManager::create(m_path);
}

StorageNamespaceImpl::~StorageNamespaceImpl()
{
    ASSERT(isMainThread());

    if (m_storageType == LocalStorage) {
        ASSERT(localStorageNamespaceMap().get(m_path) == this);
        localStorageNamespaceMap().remove(m_path);
    }

    if (!m_isShutdown)
        close();
}

PassRefPtr<StorageNamespace> StorageNamespaceImpl::copy()
{
    ASSERT(isMainThread());
    ASSERT(!m_isShutdown);
    ASSERT(m_storageType == SessionStorage);

    StorageNamespaceImpl* newNamespace = new StorageNamespaceImpl(m_storageType, m_path, m_quota);

    StorageAreaMap::iterator end = m_storageAreaMap.end();
    for (StorageAreaMap::iterator i = m_storageAreaMap.begin(); i != end; ++i)
        newNamespace->m_storageAreaMap.set(i->first, i->second->copy());
    return adoptRef(newNamespace);
}

PassRefPtr<StorageArea> StorageNamespaceImpl::storageArea(PassRefPtr<SecurityOrigin> prpOrigin)
{
    ASSERT(isMainThread());
    ASSERT(!m_isShutdown);

    RefPtr<SecurityOrigin> origin = prpOrigin;
    RefPtr<StorageAreaImpl> storageArea;
    if (storageArea = m_storageAreaMap.get(origin))
        return storageArea.release();

    storageArea = StorageAreaImpl::create(m_storageType, origin, m_syncManager, m_quota);
    m_storageAreaMap.set(origin.release(), storageArea);
    return storageArea.release();
}

void StorageNamespaceImpl::close()
{
    ASSERT(isMainThread());
    ASSERT(!m_isShutdown);

    // If we're session storage, we shouldn't need to do any work here.
    if (m_storageType == SessionStorage) {
        ASSERT(!m_syncManager);
        return;
    }

    StorageAreaMap::iterator end = m_storageAreaMap.end();
    for (StorageAreaMap::iterator it = m_storageAreaMap.begin(); it != end; ++it)
        it->second->close();

    if (m_syncManager)
        m_syncManager->close();

    m_isShutdown = true;
}

#ifdef ANDROID
void StorageNamespaceImpl::clear(Page* page)
{
    ASSERT(isMainThread());
    if (m_isShutdown)
        return;

    // Clear all the keys for each of the storage areas.
    StorageAreaMap::iterator end = m_storageAreaMap.end();
    for (StorageAreaMap::iterator it = m_storageAreaMap.begin(); it != end; ++it) {
        // if there is no page provided, then the user tried to clear storage
        // with only pages in private browsing mode open. So we do not need to
        // provide a Frame* here (as the frame is only used to dispatch storage events
        // and private browsing pages won't be using them).
        it->second->clear(page ? page->mainFrame() : 0);
    }
}
#endif

void StorageNamespaceImpl::unlock()
{
    // Because there's a single event loop per-process, this is a no-op.
}

} // namespace WebCore

#endif // ENABLE(DOM_STORAGE)
