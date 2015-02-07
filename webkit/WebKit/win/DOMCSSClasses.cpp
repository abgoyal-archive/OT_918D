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
 * Copyright (C) 2006, 2007 Apple Inc.  All rights reserved.
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
#include "WebKitDLL.h"
#include "DOMCSSClasses.h"

#pragma warning(push, 0)
#include <WebCore/PlatformString.h>
#pragma warning(pop)

// DOMCSSStyleDeclaration - DOMCSSStyleDeclaration ----------------------------

DOMCSSStyleDeclaration::DOMCSSStyleDeclaration(WebCore::CSSStyleDeclaration* s)
: m_style(0)
{
    if (s)
        s->ref();

    m_style = s;
}

DOMCSSStyleDeclaration::~DOMCSSStyleDeclaration()
{
    if (m_style)
        m_style->deref();
}

IDOMCSSStyleDeclaration* DOMCSSStyleDeclaration::createInstance(WebCore::CSSStyleDeclaration* s)
{
    if (!s)
        return 0;

    HRESULT hr;
    IDOMCSSStyleDeclaration* domStyle = 0;

    DOMCSSStyleDeclaration* newStyle = new DOMCSSStyleDeclaration(s);
    hr = newStyle->QueryInterface(IID_IDOMCSSStyleDeclaration, (void**)&domStyle);

    if (FAILED(hr))
        return 0;

    return domStyle;
}

// DOMCSSStyleDeclaration - IUnknown ------------------------------------------

HRESULT STDMETHODCALLTYPE DOMCSSStyleDeclaration::QueryInterface(REFIID riid, void** ppvObject)
{
    *ppvObject = 0;
    if (IsEqualGUID(riid, IID_IDOMCSSStyleDeclaration))
        *ppvObject = static_cast<IDOMCSSStyleDeclaration*>(this);
    else
        return DOMObject::QueryInterface(riid, ppvObject);

    AddRef();
    return S_OK;
}

// DOMCSSStyleDeclaration - IDOMCSSStyleDeclaration ---------------------------

HRESULT STDMETHODCALLTYPE DOMCSSStyleDeclaration::cssText( 
    /* [retval][out] */ BSTR* /*result*/)
{
    ASSERT_NOT_REACHED();
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE DOMCSSStyleDeclaration::setCssText( 
    /* [in] */ BSTR cssText)
{
    WebCore::String cssTextString(cssText);
    // FIXME: <rdar://5148045> return DOM exception info
    WebCore::ExceptionCode ec;
    m_style->setCssText(cssTextString, ec);
    return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMCSSStyleDeclaration::getPropertyValue( 
    /* [in] */ BSTR propertyName,
    /* [retval][out] */ BSTR* result)
{
    WebCore::String propertyNameString(propertyName);
    WebCore::String value = m_style->getPropertyValue(propertyNameString);
    *result = SysAllocStringLen(value.characters(), value.length());
    if (value.length() && !*result)
        return E_OUTOFMEMORY;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMCSSStyleDeclaration::getPropertyCSSValue( 
    /* [in] */ BSTR /*propertyName*/,
    /* [retval][out] */ IDOMCSSValue** /*result*/)
{
    ASSERT_NOT_REACHED();
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE DOMCSSStyleDeclaration::removeProperty( 
    /* [in] */ BSTR /*propertyName*/,
    /* [retval][out] */ BSTR* /*result*/)
{
    ASSERT_NOT_REACHED();
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE DOMCSSStyleDeclaration::getPropertyPriority( 
    /* [in] */ BSTR /*propertyName*/,
    /* [retval][out] */ BSTR* /*result*/)
{
    ASSERT_NOT_REACHED();
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE DOMCSSStyleDeclaration::setProperty( 
    /* [in] */ BSTR propertyName,
    /* [in] */ BSTR value,
    /* [in] */ BSTR priority)
{
    WebCore::String propertyNameString(propertyName);
    WebCore::String valueString(value);
    WebCore::String priorityString(priority);
    // FIXME: <rdar://5148045> return DOM exception info
    WebCore::ExceptionCode code;
    m_style->setProperty(propertyNameString, valueString, priorityString, code);
    return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMCSSStyleDeclaration::length( 
    /* [retval][out] */ UINT* /*result*/)
{
    ASSERT_NOT_REACHED();
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE DOMCSSStyleDeclaration::item( 
    /* [in] */ UINT /*index*/,
    /* [retval][out] */ BSTR* /*result*/)
{
    ASSERT_NOT_REACHED();
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE DOMCSSStyleDeclaration::parentRule( 
    /* [retval][out] */ IDOMCSSRule** /*result*/)
{
    ASSERT_NOT_REACHED();
    return E_NOTIMPL;
}