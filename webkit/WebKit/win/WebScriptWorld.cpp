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
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "WebKitDLL.h"
#include "WebScriptWorld.h"

#include <JavaScriptCore/APICast.h>
#include <WebCore/JSDOMBinding.h>
#include <WebCore/ScriptController.h>

using namespace WebCore;

typedef HashMap<DOMWrapperWorld*, WebScriptWorld*> WorldMap;
static WorldMap& allWorlds()
{
    static WorldMap& map = *new WorldMap;
    return map;
}

inline WebScriptWorld::WebScriptWorld(PassRefPtr<DOMWrapperWorld> world)
    : m_refCount(0)
    , m_world(world)
{
    ASSERT_ARG(world, m_world);

    ASSERT_ARG(world, !allWorlds().contains(m_world.get()));
    allWorlds().add(m_world.get(), this);

    ++gClassCount;
    gClassNameCount.add("WebScriptWorld");
}

WebScriptWorld::~WebScriptWorld()
{
    ASSERT(allWorlds().contains(m_world.get()));
    allWorlds().remove(m_world.get());

    --gClassCount;
    gClassNameCount.remove("WebScriptWorld");
}

WebScriptWorld* WebScriptWorld::standardWorld()
{
    static WebScriptWorld* standardWorld = createInstance(mainThreadNormalWorld()).releaseRef();
    return standardWorld;
}

COMPtr<WebScriptWorld> WebScriptWorld::createInstance()
{
    return createInstance(ScriptController::createWorld());
}

COMPtr<WebScriptWorld> WebScriptWorld::createInstance(PassRefPtr<DOMWrapperWorld> world)
{
    return new WebScriptWorld(world);
}

COMPtr<WebScriptWorld> WebScriptWorld::findOrCreateWorld(DOMWrapperWorld* world)
{
    if (world == mainThreadNormalWorld())
        return standardWorld();

    if (WebScriptWorld* existingWorld = allWorlds().get(world))
        return existingWorld;

    return createInstance(world);
}

ULONG WebScriptWorld::AddRef()
{
    return ++m_refCount;
}

ULONG WebScriptWorld::Release()
{
    ULONG newRefCount = --m_refCount;
    if (!newRefCount)
        delete this;
    return newRefCount;
}

HRESULT WebScriptWorld::QueryInterface(REFIID riid, void** ppvObject)
{
    if (!ppvObject)
        return E_POINTER;
    *ppvObject = 0;

    if (IsEqualIID(riid, __uuidof(WebScriptWorld)))
        *ppvObject = this;
    else if (IsEqualIID(riid, __uuidof(IWebScriptWorld)))
        *ppvObject = static_cast<IWebScriptWorld*>(this);
    else if (IsEqualIID(riid, IID_IUnknown))
        *ppvObject = static_cast<IUnknown*>(this);
    else
        return E_NOINTERFACE;

    AddRef();
    return S_OK;
}

HRESULT WebScriptWorld::standardWorld(IWebScriptWorld** outWorld)
{
    if (!outWorld)
        return E_POINTER;

    *outWorld = standardWorld();
    (*outWorld)->AddRef();
    return S_OK;
}

HRESULT WebScriptWorld::scriptWorldForGlobalContext(JSGlobalContextRef context, IWebScriptWorld** outWorld)
{
    if (!outWorld)
        return E_POINTER;
    return findOrCreateWorld(currentWorld(toJS(context))).copyRefTo(outWorld);
}
