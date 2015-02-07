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
 * Copyright (C) 2009 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "WebAccessibilityCacheImpl.h"

#include "AccessibilityObject.h"
#include "AXObjectCache.h"
#include "Document.h"
#include "Frame.h"

#include "WebAccessibilityObject.h"
#include "WebFrameImpl.h"
#include "WebViewImpl.h"

using namespace WebCore;

namespace WebKit {

const int invalidObjectId = -1;
const int firstObjectId = 1000;

static PassRefPtr<AccessibilityObject> toAccessibilityObject(const WebAccessibilityObject& object)
{
    return object;
}

// WebView ----------------------------------------------------------------

WebAccessibilityCache* WebAccessibilityCache::create()
{
    return new WebAccessibilityCacheImpl();
}

// WeakHandle -------------------------------------------------------------

PassRefPtr<WebAccessibilityCacheImpl::WeakHandle> WebAccessibilityCacheImpl::WeakHandle::create(AccessibilityObject* object)
{
    // FIXME: Remove resetting ref-count from AccessibilityObjectWrapper
    // and convert to use adoptRef.
    return new WebAccessibilityCacheImpl::WeakHandle(object);
}

WebAccessibilityCacheImpl::WeakHandle::WeakHandle(AccessibilityObject* object)
    : AccessibilityObjectWrapper(object)
{
    m_object->setWrapper(this);
}

// WebAccessibilityCacheImpl ----------------------------------------

void WebAccessibilityCacheImpl::WeakHandle::detach()
{
    if (m_object)
        m_object = 0;
}

WebAccessibilityCacheImpl::WebAccessibilityCacheImpl()
    : m_nextNewId(firstObjectId)
    , m_initialized(false)
{
}

WebAccessibilityCacheImpl::~WebAccessibilityCacheImpl()
{
}

void WebAccessibilityCacheImpl::initialize(WebView* view)
{
    AXObjectCache::enableAccessibility();
    WebAccessibilityObject root = view->accessibilityObject();
    if (root.isNull())
        return;

    RefPtr<AccessibilityObject> rootObject = toAccessibilityObject(root);

    // Insert root in hashmaps.
    m_objectMap.set(m_nextNewId, WeakHandle::create(rootObject.get()));
    m_idMap.set(rootObject.get(), m_nextNewId++);

    m_initialized = true;
}

WebAccessibilityObject WebAccessibilityCacheImpl::getObjectById(int id)
{
    ObjectMap::iterator it = m_objectMap.find(id);

    if (it == m_objectMap.end() || !it->second)
        return WebAccessibilityObject();

    return WebAccessibilityObject(it->second->accessibilityObject());
}

bool WebAccessibilityCacheImpl::isValidId(int id) const
{
    return id >= firstObjectId;
}

void WebAccessibilityCacheImpl::remove(int id)
{
    ObjectMap::iterator it = m_objectMap.find(id);

    if (it == m_objectMap.end())
        return;

    if (it->second) {
        // Erase element from reverse hashmap.
        IdMap::iterator it2 = m_idMap.find(it->second->accessibilityObject());
        if (it2 != m_idMap.end())
            m_idMap.remove(it2);
    }

    m_objectMap.remove(it);
}

void WebAccessibilityCacheImpl::clear()
{
    m_objectMap.clear();
    m_idMap.clear();
}

int WebAccessibilityCacheImpl::addOrGetId(const WebAccessibilityObject& object)
{
    if (object.isNull())
        return invalidObjectId;

    RefPtr<AccessibilityObject> o = toAccessibilityObject(object);

    IdMap::iterator it = m_idMap.find(o.get());

    if (it != m_idMap.end())
        return it->second;

    // Insert new accessibility object in hashmaps and return its newly
    // assigned accessibility object id.
    m_objectMap.set(m_nextNewId, WeakHandle::create(o.get()));
    m_idMap.set(o.get(), m_nextNewId);

    return m_nextNewId++;
}

}
