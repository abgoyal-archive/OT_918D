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
#include "DRTDesktopNotificationPresenter.h"

#include "DumpRenderTree.h"
#include "LayoutTestController.h"
#include <JavaScriptCore/JSStringRef.h>
#include <JavaScriptCore/JSStringRefBSTR.h>
#include <WebCore/NotificationPresenter.h>

DRTDesktopNotificationPresenter::DRTDesktopNotificationPresenter()
    : m_refCount(1) {} 

HRESULT STDMETHODCALLTYPE DRTDesktopNotificationPresenter::QueryInterface(REFIID riid, void** ppvObject)
{
    *ppvObject = 0;
    if (IsEqualGUID(riid, IID_IUnknown))
        *ppvObject = static_cast<DRTDesktopNotificationPresenter*>(this);
    else if (IsEqualGUID(riid, IID_IWebDesktopNotificationsDelegate))
        *ppvObject = static_cast<DRTDesktopNotificationPresenter*>(this);
    else
        return E_NOINTERFACE;

    AddRef();
    return S_OK;
}

ULONG STDMETHODCALLTYPE DRTDesktopNotificationPresenter::AddRef()
{
    return ++m_refCount;
}

ULONG STDMETHODCALLTYPE DRTDesktopNotificationPresenter::Release()
{
    ULONG newRef = --m_refCount;
    if (!newRef)
        delete(this);

    return newRef;
}

HRESULT STDMETHODCALLTYPE DRTDesktopNotificationPresenter::showDesktopNotification(
    /* [in] */ IWebDesktopNotification* notification)
{
    BSTR title, text, url;
    BOOL html;

    if (!notification->isHTML(&html) && html) {
        notification->contentsURL(&url);    
        printf("DESKTOP NOTIFICATION: contents at %S\n", url ? url : L"");
    } else {
        notification->iconURL(&url);
        notification->title(&title);
        notification->text(&text);
        printf("DESKTOP NOTIFICATION: icon %S, title %S, text %S\n", 
            url ? url : L"", 
            title ? title : L"", 
            text ? text : L"");
    }

    // In this stub implementation, the notification is displayed immediately;
    // we dispatch the display event to mimic that.
    notification->notifyDisplay();

    return S_OK;
}

HRESULT STDMETHODCALLTYPE DRTDesktopNotificationPresenter::cancelDesktopNotification(
    /* [in] */ IWebDesktopNotification* notification)
{
    BSTR identifier;
    BOOL html;
    notification->isHTML(&html);
    if (html)
        notification->contentsURL(&identifier);
    else
        notification->title(&identifier);

    printf("DESKTOP NOTIFICATION CLOSED: %S\n", identifier ? identifier : L"");
    notification->notifyClose(false);

    return S_OK;
}

HRESULT STDMETHODCALLTYPE DRTDesktopNotificationPresenter::notificationDestroyed(
    /* [in] */ IWebDesktopNotification* notification)
{
    // Since in these tests events happen immediately, we don't hold on to
    // Notification pointers.  So there's no cleanup to do.
    return S_OK;
}

HRESULT STDMETHODCALLTYPE DRTDesktopNotificationPresenter::checkNotificationPermission(
        /* [in] */ BSTR origin, 
        /* [out, retval] */ int* result)
{
#if ENABLE(NOTIFICATIONS)
    JSStringRef jsOrigin = JSStringCreateWithBSTR(origin);
    bool allowed = ::gLayoutTestController->checkDesktopNotificationPermission(jsOrigin);

    if (allowed)
        *result = WebCore::NotificationPresenter::PermissionAllowed;
    else
        *result = WebCore::NotificationPresenter::PermissionDenied;

    JSStringRelease(jsOrigin);
#endif
    return S_OK;
}

HRESULT STDMETHODCALLTYPE DRTDesktopNotificationPresenter::requestNotificationPermission(
        /* [in] */ BSTR origin)
{
    printf("DESKTOP NOTIFICATION PERMISSION REQUESTED: %S\n", origin ? origin : L"");
    return S_OK;
}
