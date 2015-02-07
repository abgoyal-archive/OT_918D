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
#include "V8XSLTProcessor.h"

#include "Document.h"
#include "DocumentFragment.h"
#include "Node.h"

#include "V8Binding.h"
#include "V8Document.h"
#include "V8DocumentFragment.h"
#include "V8Node.h"
#include "V8Proxy.h"
#include "XSLTProcessor.h"

#include <wtf/RefPtr.h>

namespace WebCore {

v8::Handle<v8::Value> V8XSLTProcessor::constructorCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.XSLTProcessor.Constructor");
    return V8Proxy::constructDOMObject<V8ClassIndex::XSLTPROCESSOR, XSLTProcessor>(args);
}


v8::Handle<v8::Value> V8XSLTProcessor::importStylesheetCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.XSLTProcessor.importStylesheet");
    if (!V8Node::HasInstance(args[0]))
        return v8::Undefined();

    XSLTProcessor* imp = V8XSLTProcessor::toNative(args.Holder());

    Node* node = V8Node::toNative(v8::Handle<v8::Object>::Cast(args[0]));
    imp->importStylesheet(node);
    return v8::Undefined();
}


v8::Handle<v8::Value> V8XSLTProcessor::transformToFragmentCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.XSLTProcessor.transformToFragment");
    if (!V8Node::HasInstance(args[0]) || !V8Document::HasInstance(args[1]))
        return v8::Undefined();

    XSLTProcessor* imp = V8XSLTProcessor::toNative(args.Holder());

    Node* source = V8Node::toNative(v8::Handle<v8::Object>::Cast(args[0]));
    Document* owner = V8Document::toNative(v8::Handle<v8::Object>::Cast(args[1]));
    RefPtr<DocumentFragment> result = imp->transformToFragment(source, owner);
    return toV8(result.release());
}


v8::Handle<v8::Value> V8XSLTProcessor::transformToDocumentCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.XSLTProcessor.transformToDocument");

    if (!V8Node::HasInstance(args[0]))
        return v8::Undefined();

    XSLTProcessor* imp = V8XSLTProcessor::toNative(args.Holder());

    Node* source = V8Node::toNative(v8::Handle<v8::Object>::Cast(args[0]));
    if (!source)
        return v8::Undefined();

    RefPtr<Document> result = imp->transformToDocument(source);
    if (!result)
        return v8::Undefined();

    return toV8(result.release());
}


v8::Handle<v8::Value> V8XSLTProcessor::setParameterCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.XSLTProcessor.setParameter");
    if (isUndefinedOrNull(args[1]) || isUndefinedOrNull(args[2]))
        return v8::Undefined();

    XSLTProcessor* imp = V8XSLTProcessor::toNative(args.Holder());

    String namespaceURI = toWebCoreString(args[0]);
    String localName = toWebCoreString(args[1]);
    String value = toWebCoreString(args[2]);
    imp->setParameter(namespaceURI, localName, value);

    return v8::Undefined();
}


v8::Handle<v8::Value> V8XSLTProcessor::getParameterCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.XSLTProcessor.getParameter");
    if (isUndefinedOrNull(args[1]))
        return v8::Undefined();

    XSLTProcessor* imp = V8XSLTProcessor::toNative(args.Holder());

    String namespaceURI = toWebCoreString(args[0]);
    String localName = toWebCoreString(args[1]);
    String result = imp->getParameter(namespaceURI, localName);
    if (result.isNull())
        return v8::Undefined();

    return v8String(result);
}

v8::Handle<v8::Value> V8XSLTProcessor::removeParameterCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.XSLTProcessor.removeParameter");
    if (isUndefinedOrNull(args[1]))
        return v8::Undefined();

    XSLTProcessor* imp = V8XSLTProcessor::toNative(args.Holder());

    String namespaceURI = toWebCoreString(args[0]);
    String localName = toWebCoreString(args[1]);
    imp->removeParameter(namespaceURI, localName);
    return v8::Undefined();
}

} // namespace WebCore
