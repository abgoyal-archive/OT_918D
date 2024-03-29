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
#include "WebURLRequest.h"

#include "ResourceRequest.h"

#include "WebHTTPBody.h"
#include "WebHTTPHeaderVisitor.h"
#include "WebURL.h"
#include "WebURLRequestPrivate.h"

using namespace WebCore;

namespace WebKit {

// The standard implementation of WebURLRequestPrivate, which maintains
// ownership of a ResourceRequest instance.
class WebURLRequestPrivateImpl : public WebURLRequestPrivate {
public:
    WebURLRequestPrivateImpl()
    {
        m_resourceRequest = &m_resourceRequestAllocation;
    }

    WebURLRequestPrivateImpl(const WebURLRequestPrivate* p)
        : m_resourceRequestAllocation(*p->m_resourceRequest)
    {
        m_resourceRequest = &m_resourceRequestAllocation;
    }

    virtual void dispose() { delete this; }

    ResourceRequest m_resourceRequestAllocation;
};

void WebURLRequest::initialize()
{
    assign(new WebURLRequestPrivateImpl());
}

void WebURLRequest::reset()
{
    assign(0);
}

void WebURLRequest::assign(const WebURLRequest& r)
{
    if (&r != this)
        assign(r.m_private ? new WebURLRequestPrivateImpl(r.m_private) : 0);
}

bool WebURLRequest::isNull() const
{
    return !m_private || m_private->m_resourceRequest->isNull();
}

WebURL WebURLRequest::url() const
{
    return m_private->m_resourceRequest->url();
}

void WebURLRequest::setURL(const WebURL& url)
{
    m_private->m_resourceRequest->setURL(url);
}

WebURL WebURLRequest::firstPartyForCookies() const
{
    return m_private->m_resourceRequest->firstPartyForCookies();
}

void WebURLRequest::setFirstPartyForCookies(const WebURL& firstPartyForCookies)
{
    m_private->m_resourceRequest->setFirstPartyForCookies(firstPartyForCookies);
}

bool WebURLRequest::allowCookies() const
{
    return m_private->m_resourceRequest->allowCookies();
}

void WebURLRequest::setAllowCookies(bool allowCookies)
{
    m_private->m_resourceRequest->setAllowCookies(allowCookies);
}

bool WebURLRequest::allowStoredCredentials() const
{
    return m_private->m_allowStoredCredentials;
}

void WebURLRequest::setAllowStoredCredentials(bool allowStoredCredentials)
{
    m_private->m_allowStoredCredentials = allowStoredCredentials;
}

WebURLRequest::CachePolicy WebURLRequest::cachePolicy() const
{
    return static_cast<WebURLRequest::CachePolicy>(
        m_private->m_resourceRequest->cachePolicy());
}

void WebURLRequest::setCachePolicy(CachePolicy cachePolicy)
{
    m_private->m_resourceRequest->setCachePolicy(
        static_cast<ResourceRequestCachePolicy>(cachePolicy));
}

WebString WebURLRequest::httpMethod() const
{
    return m_private->m_resourceRequest->httpMethod();
}

void WebURLRequest::setHTTPMethod(const WebString& httpMethod)
{
    m_private->m_resourceRequest->setHTTPMethod(httpMethod);
}

WebString WebURLRequest::httpHeaderField(const WebString& name) const
{
    return m_private->m_resourceRequest->httpHeaderField(name);
}

void WebURLRequest::setHTTPHeaderField(const WebString& name, const WebString& value)
{
    m_private->m_resourceRequest->setHTTPHeaderField(name, value);
}

void WebURLRequest::addHTTPHeaderField(const WebString& name, const WebString& value)
{
    m_private->m_resourceRequest->addHTTPHeaderField(name, value);
}

void WebURLRequest::clearHTTPHeaderField(const WebString& name)
{
    // FIXME: Add a clearHTTPHeaderField method to ResourceRequest.
    const HTTPHeaderMap& map = m_private->m_resourceRequest->httpHeaderFields();
    const_cast<HTTPHeaderMap*>(&map)->remove(name);
}

void WebURLRequest::visitHTTPHeaderFields(WebHTTPHeaderVisitor* visitor) const
{
    const HTTPHeaderMap& map = m_private->m_resourceRequest->httpHeaderFields();
    for (HTTPHeaderMap::const_iterator it = map.begin(); it != map.end(); ++it)
        visitor->visitHeader(it->first, it->second);
}

WebHTTPBody WebURLRequest::httpBody() const
{
    return WebHTTPBody(m_private->m_resourceRequest->httpBody());
}

void WebURLRequest::setHTTPBody(const WebHTTPBody& httpBody)
{
    m_private->m_resourceRequest->setHTTPBody(httpBody);
}

bool WebURLRequest::reportUploadProgress() const
{
    return m_private->m_resourceRequest->reportUploadProgress();
}

void WebURLRequest::setReportUploadProgress(bool reportUploadProgress)
{
    m_private->m_resourceRequest->setReportUploadProgress(reportUploadProgress);
}

WebURLRequest::TargetType WebURLRequest::targetType() const
{
    return static_cast<TargetType>(m_private->m_resourceRequest->targetType());
}

void WebURLRequest::setTargetType(TargetType targetType)
{
    m_private->m_resourceRequest->setTargetType(
        static_cast<ResourceRequest::TargetType>(targetType));
}

int WebURLRequest::requestorID() const
{
    return m_private->m_resourceRequest->requestorID();
}

void WebURLRequest::setRequestorID(int requestorID)
{
    m_private->m_resourceRequest->setRequestorID(requestorID);
}

int WebURLRequest::requestorProcessID() const
{
    return m_private->m_resourceRequest->requestorProcessID();
}

void WebURLRequest::setRequestorProcessID(int requestorProcessID)
{
    m_private->m_resourceRequest->setRequestorProcessID(requestorProcessID);
}

int WebURLRequest::appCacheHostID() const
{
    return m_private->m_resourceRequest->appCacheHostID();
}

void WebURLRequest::setAppCacheHostID(int appCacheHostID)
{
    m_private->m_resourceRequest->setAppCacheHostID(appCacheHostID);
}

ResourceRequest& WebURLRequest::toMutableResourceRequest()
{
    ASSERT(m_private);
    ASSERT(m_private->m_resourceRequest);

    return *m_private->m_resourceRequest;
}

const ResourceRequest& WebURLRequest::toResourceRequest() const
{
    ASSERT(m_private);
    ASSERT(m_private->m_resourceRequest);

    return *m_private->m_resourceRequest;
}

void WebURLRequest::assign(WebURLRequestPrivate* p)
{
    // Subclasses may call this directly so a self-assignment check is needed
    // here as well as in the public assign method.
    if (m_private == p)
        return;
    if (m_private)
        m_private->dispose();
    m_private = p;
}

} // namespace WebKit
