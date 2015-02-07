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

// Copyright (c) 2008, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "config.h"
#include "V8CustomXPathNSResolver.h"

#if ENABLE(XPATH)

#include "PlatformString.h"
#include "V8Binding.h"
#include "V8Proxy.h"

namespace WebCore {

PassRefPtr<V8CustomXPathNSResolver> V8CustomXPathNSResolver::create(V8Proxy* proxy, v8::Handle<v8::Object> resolver)
{
    return adoptRef(new V8CustomXPathNSResolver(proxy, resolver));
}

V8CustomXPathNSResolver::V8CustomXPathNSResolver(V8Proxy* proxy, v8::Handle<v8::Object> resolver)
        : m_proxy(proxy)
        , m_resolver(resolver)
{
}

V8CustomXPathNSResolver::~V8CustomXPathNSResolver()
{
}

String V8CustomXPathNSResolver::lookupNamespaceURI(const String& prefix)
{
    V8Proxy* proxy = m_proxy;

    if (!proxy) {
        proxy = V8Proxy::retrieve();
        if (!proxy)
            return String();
    }

    v8::Handle<v8::Function> lookupNamespaceURIFunc;
    v8::Handle<v8::String> lookupNamespaceURIName = v8::String::New("lookupNamespaceURI");

    // Check if the resolver has a function property named lookupNamespaceURI.
    if (m_resolver->Has(lookupNamespaceURIName)) {
        v8::Handle<v8::Value> lookupNamespaceURI = m_resolver->Get(lookupNamespaceURIName);
        if (lookupNamespaceURI->IsFunction())
            lookupNamespaceURIFunc = v8::Handle<v8::Function>::Cast(lookupNamespaceURI);
    }

    if (lookupNamespaceURIFunc.IsEmpty() && !m_resolver->IsFunction()) {
        Frame* frame = proxy->frame();
        logInfo(frame, "XPathNSResolver does not have a lookupNamespaceURI method.", String());
        return String();
    }

    // Catch exceptions from calling the namespace resolver.
    v8::TryCatch try_catch;
    try_catch.SetVerbose(true);  // Print exceptions to console.

    const int argc = 1;
    v8::Handle<v8::Value> argv[argc] = { v8String(prefix) };
    v8::Handle<v8::Function> function = lookupNamespaceURIFunc.IsEmpty() ? v8::Handle<v8::Function>::Cast(m_resolver) : lookupNamespaceURIFunc;

    v8::Handle<v8::Value> retval = proxy->callFunction(function, m_resolver, argc, argv);

    // Eat exceptions from namespace resolver and return an empty string. This will most likely cause NAMESPACE_ERR.
    if (try_catch.HasCaught())
        return String();

    return toWebCoreStringWithNullCheck(retval);
}

} // namespace WebCore

#endif // ENABLE(XPATH)
