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
 * Copyright (C) 2006, 2007 Apple Inc. All rights reserved.
 * Copyright (C) 2009  Jan Michael Alonzo
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "DocumentLoaderGtk.h"

#include "webkitprivate.h"
#include "webkitwebdatasource.h"

using namespace WebCore;

namespace WebKit {

DocumentLoader::DocumentLoader(const ResourceRequest& request, const SubstituteData& substituteData)
    : WebCore::DocumentLoader(request, substituteData)
    , m_isDataSourceReffed(false)
    , m_dataSource(0)
{
}

void DocumentLoader::setDataSource(WebKitWebDataSource* dataSource)
{
    ASSERT(!m_dataSource);

    m_dataSource = dataSource;
    refDataSource();
}

void DocumentLoader::detachDataSource()
{
    unrefDataSource();
}

void DocumentLoader::attachToFrame()
{
    WebCore::DocumentLoader::attachToFrame();

    if (m_dataSource) {
        refDataSource();
        return;
    }

    // We may get to here without having a datasource, when the data
    // is coming from the page cache.
    WebKitWebDataSource* dataSource = webkit_web_data_source_new_with_loader(this);
    setDataSource(dataSource);
    g_object_unref(dataSource);
}

void DocumentLoader::detachFromFrame()
{
    WebCore::DocumentLoader::detachFromFrame();

    if (m_loadingResources.isEmpty())
        unrefDataSource();
}

void DocumentLoader::increaseLoadCount(unsigned long identifier)
{
    ASSERT(m_dataSource);

    if (m_loadingResources.contains(identifier))
        return;
    m_loadingResources.add(identifier);
    refDataSource();
}

void DocumentLoader::decreaseLoadCount(unsigned long identifier)
{
    HashSet<unsigned long>::iterator it = m_loadingResources.find(identifier);

    // It is valid for a load to be cancelled before it's started.
    if (it == m_loadingResources.end())
        return;

    m_loadingResources.remove(it);

    if (m_loadingResources.isEmpty() && !frame())
        unrefDataSource();
}

// helper methos to avoid ref count churn
void DocumentLoader::refDataSource()
{
    if (!m_dataSource || m_isDataSourceReffed)
        return;
    m_isDataSourceReffed = true;
    g_object_ref(m_dataSource);
}
void DocumentLoader::unrefDataSource()
{
    if (!m_isDataSourceReffed)
        return;
    ASSERT(m_dataSource);
    m_isDataSourceReffed = false;
    g_object_unref(m_dataSource);
    m_dataSource = 0;
}

} // end namespace WebKit
