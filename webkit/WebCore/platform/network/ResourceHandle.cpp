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
 * Copyright (C) 2004, 2006, 2007, 2008 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"
#include "ResourceHandle.h"
#include "ResourceHandleInternal.h"

#include "Logging.h"
#include "ResourceHandleClient.h"
#include "Timer.h"
#include <algorithm>

namespace WebCore {

static bool shouldForceContentSniffing;

ResourceHandle::ResourceHandle(const ResourceRequest& request, ResourceHandleClient* client, bool defersLoading,
         bool shouldContentSniff, bool mightDownloadFromHandle)
    : d(new ResourceHandleInternal(this, request, client, defersLoading, shouldContentSniff, mightDownloadFromHandle))
{
}

PassRefPtr<ResourceHandle> ResourceHandle::create(const ResourceRequest& request, ResourceHandleClient* client,
    Frame* frame, bool defersLoading, bool shouldContentSniff, bool mightDownloadFromHandle)
{
    if (shouldContentSniff)
        shouldContentSniff = shouldContentSniffURL(request.url());

    RefPtr<ResourceHandle> newHandle(adoptRef(new ResourceHandle(request, client, defersLoading, shouldContentSniff, mightDownloadFromHandle)));

    if (!request.url().isValid()) {
        newHandle->scheduleFailure(InvalidURLFailure);
        return newHandle.release();
    }

    if (!portAllowed(request.url())) {
        newHandle->scheduleFailure(BlockedFailure);
        return newHandle.release();
    }
        
    if (newHandle->start(frame))
        return newHandle.release();

    return 0;
}

void ResourceHandle::scheduleFailure(FailureType type)
{
    d->m_failureType = type;
    d->m_failureTimer.startOneShot(0);
}

void ResourceHandle::fireFailure(Timer<ResourceHandle>*)
{
    if (!client())
        return;

    switch (d->m_failureType) {
        case BlockedFailure:
            client()->wasBlocked(this);
            return;
        case InvalidURLFailure:
            client()->cannotShowURL(this);
            return;
    }

    ASSERT_NOT_REACHED();
}

ResourceHandleClient* ResourceHandle::client() const
{
    return d->m_client;
}

void ResourceHandle::setClient(ResourceHandleClient* client)
{
    d->m_client = client;
}

const ResourceRequest& ResourceHandle::request() const
{
    return d->m_request;
}

void ResourceHandle::clearAuthentication()
{
#if PLATFORM(MAC)
    d->m_currentMacChallenge = nil;
#endif
    d->m_currentWebChallenge.nullify();
}
  
bool ResourceHandle::shouldContentSniff() const
{
    return d->m_shouldContentSniff;
}

bool ResourceHandle::shouldContentSniffURL(const KURL& url)
{
#if PLATFORM(MAC)
    if (shouldForceContentSniffing)
        return true;
#endif
    // We shouldn't content sniff file URLs as their MIME type should be established via their extension.
    return !url.protocolIs("file");
}

void ResourceHandle::forceContentSniffing()
{
    shouldForceContentSniffing = true;
}

} // namespace WebCore
