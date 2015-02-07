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

#if ENABLE(NOTIFICATIONS)
#include "V8NotificationCenter.h"

#include "NotImplemented.h"
#include "Notification.h"
#include "NotificationCenter.h"
#include "V8Binding.h"
#include "V8CustomEventListener.h"
#include "V8CustomVoidCallback.h"
#include "V8Notification.h"
#include "V8Proxy.h"
#include "V8Utilities.h"
#include "WorkerContext.h"

namespace WebCore {

v8::Handle<v8::Value> V8Notification::addEventListenerCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.Notification.addEventListener()");
    Notification* notification = V8Notification::toNative(args.Holder());

    RefPtr<EventListener> listener = V8DOMWrapper::getEventListener(notification, args[1], false, ListenerFindOrCreate);
    if (listener) {
        String type = toWebCoreString(args[0]);
        bool useCapture = args[2]->BooleanValue();
        notification->addEventListener(type, listener, useCapture);
        createHiddenDependency(args.Holder(), args[1], cacheIndex);
    }

    return v8::Undefined();
}

v8::Handle<v8::Value> V8Notification::removeEventListenerCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.Notification.removeEventListener()");
    Notification* notification = V8Notification::toNative(args.Holder());

    RefPtr<EventListener> listener = V8DOMWrapper::getEventListener(notification, args[1], false, ListenerFindOnly);
    if (listener) {
        String type = toWebCoreString(args[0]);
        bool useCapture = args[2]->BooleanValue();
        notification->removeEventListener(type, listener.get(), useCapture);
        removeHiddenDependency(args.Holder(), args[1], cacheIndex);
    }

    return v8::Undefined();
}

v8::Handle<v8::Value> V8NotificationCenter::createHTMLNotificationCallback(const v8::Arguments& args)
{
    INC_STATS(L"DOM.NotificationCenter.CreateHTMLNotification()");
    NotificationCenter* notificationCenter = V8NotificationCenter::toNative(args.Holder());

    ExceptionCode ec = 0;
    String url = toWebCoreString(args[0]);
    RefPtr<Notification> notification = notificationCenter->createHTMLNotification(url, ec);

    if (ec)
        return throwError(ec);

    return toV8(notification.get());
}

v8::Handle<v8::Value> V8NotificationCenter::createNotificationCallback(const v8::Arguments& args)
{
    INC_STATS(L"DOM.NotificationCenter.CreateNotification()");
    NotificationCenter* notificationCenter = V8NotificationCenter::toNative(args.Holder());

    ExceptionCode ec = 0;
    RefPtr<Notification> notification = notificationCenter->createNotification(toWebCoreString(args[0]), toWebCoreString(args[1]), toWebCoreString(args[2]), ec);

    if (ec)
        return throwError(ec);

    return toV8(notification.get());
}

v8::Handle<v8::Value> V8NotificationCenter::requestPermissionCallback(const v8::Arguments& args)
{
    INC_STATS(L"DOM.NotificationCenter.RequestPermission()");
    NotificationCenter* notificationCenter = V8NotificationCenter::toNative(args.Holder());
    ScriptExecutionContext* context = notificationCenter->context();

    // Requesting permission is only valid from a page context.
    if (context->isWorkerContext())
        return throwError(NOT_SUPPORTED_ERR);

    RefPtr<V8CustomVoidCallback> callback;
    if (args.Length() > 0) {
        if (!args[0]->IsObject())
            return throwError("Callback must be of valid type.", V8Proxy::TypeError);
 
        callback = V8CustomVoidCallback::create(args[0], V8Proxy::retrieveFrameForCurrentContext());
    }

    notificationCenter->requestPermission(callback.release());
    return v8::Undefined();
}


}  // namespace WebCore

#endif  // ENABLE(NOTIFICATIONS)
