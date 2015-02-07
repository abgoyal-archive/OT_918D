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
 * Copyright (C) 2007, 2008 Apple Inc. All rights reserved.
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
#include "JSEvent.h"

#include "Clipboard.h"
#include "CompositionEvent.h"
#include "Event.h"
#include "JSBeforeLoadEvent.h"
#include "JSClipboard.h"
#include "JSCompositionEvent.h"
#include "JSErrorEvent.h"
#include "JSKeyboardEvent.h"
#include "JSMessageEvent.h"
#include "JSMouseEvent.h"
#include "JSMutationEvent.h"
#include "JSOverflowEvent.h"
#include "JSPageTransitionEvent.h"
#include "JSPopStateEvent.h"
#include "JSProgressEvent.h"
#include "JSTextEvent.h"
#include "JSUIEvent.h"
#include "JSWebKitAnimationEvent.h"
#include "JSWebKitTransitionEvent.h"
#include "JSWheelEvent.h"
#include "JSXMLHttpRequestProgressEvent.h"
#include "BeforeLoadEvent.h"
#include "ErrorEvent.h"
#include "KeyboardEvent.h"
#include "MessageEvent.h"
#include "MouseEvent.h"
#include "MutationEvent.h"
#include "OverflowEvent.h"
#include "PageTransitionEvent.h"
#include "PopStateEvent.h"
#include "ProgressEvent.h"
#include "TextEvent.h"
#include "UIEvent.h"
#include "WebKitAnimationEvent.h"
#include "WebKitTransitionEvent.h"
#include "WheelEvent.h"
#include "XMLHttpRequestProgressEvent.h"
#include <runtime/JSLock.h>

#if ENABLE(DOM_STORAGE)
#include "JSStorageEvent.h"
#include "StorageEvent.h"
#endif

#if ENABLE(SVG)
#include "JSSVGZoomEvent.h"
#include "SVGZoomEvent.h"
#endif

#if ENABLE(TOUCH_EVENTS)
#include "JSTouchEvent.h"
#include "TouchEvent.h"
#endif

using namespace JSC;

namespace WebCore {

JSValue JSEvent::clipboardData(ExecState* exec) const
{
    return impl()->isClipboardEvent() ? toJS(exec, globalObject(), impl()->clipboardData()) : jsUndefined();
}

JSValue toJS(ExecState* exec, JSDOMGlobalObject* globalObject, Event* event)
{
    JSLock lock(SilenceAssertionsOnly);

    if (!event)
        return jsNull();

    DOMObject* wrapper = getCachedDOMObjectWrapper(exec, event);
    if (wrapper)
        return wrapper;

    if (event->isUIEvent()) {
        if (event->isKeyboardEvent())
            wrapper = CREATE_DOM_OBJECT_WRAPPER(exec, globalObject, KeyboardEvent, event);
        else if (event->isTextEvent())
            wrapper = CREATE_DOM_OBJECT_WRAPPER(exec, globalObject, TextEvent, event);
        else if (event->isMouseEvent())
            wrapper = CREATE_DOM_OBJECT_WRAPPER(exec, globalObject, MouseEvent, event);
        else if (event->isWheelEvent())
            wrapper = CREATE_DOM_OBJECT_WRAPPER(exec, globalObject, WheelEvent, event);
#if ENABLE(SVG)
        else if (event->isSVGZoomEvent())
            wrapper = CREATE_DOM_OBJECT_WRAPPER(exec, globalObject, SVGZoomEvent, event);
#endif
        else if (event->isCompositionEvent())
            wrapper = CREATE_DOM_OBJECT_WRAPPER(exec, globalObject, CompositionEvent, event);
#if ENABLE(TOUCH_EVENTS)
        else if (event->isTouchEvent())
            wrapper = CREATE_DOM_OBJECT_WRAPPER(exec, globalObject, TouchEvent, event);
#endif
        else
            wrapper = CREATE_DOM_OBJECT_WRAPPER(exec, globalObject, UIEvent, event);
    } else if (event->isMutationEvent())
        wrapper = CREATE_DOM_OBJECT_WRAPPER(exec, globalObject, MutationEvent, event);
    else if (event->isOverflowEvent())
        wrapper = CREATE_DOM_OBJECT_WRAPPER(exec, globalObject, OverflowEvent, event);
    else if (event->isMessageEvent())
        wrapper = CREATE_DOM_OBJECT_WRAPPER(exec, globalObject, MessageEvent, event);
    else if (event->isPageTransitionEvent())
        wrapper = CREATE_DOM_OBJECT_WRAPPER(exec, globalObject, PageTransitionEvent, event);
    else if (event->isProgressEvent()) {
        if (event->isXMLHttpRequestProgressEvent())
            wrapper = CREATE_DOM_OBJECT_WRAPPER(exec, globalObject, XMLHttpRequestProgressEvent, event);
        else
            wrapper = CREATE_DOM_OBJECT_WRAPPER(exec, globalObject, ProgressEvent, event);
    } else if (event->isBeforeLoadEvent())
        wrapper = CREATE_DOM_OBJECT_WRAPPER(exec, globalObject, BeforeLoadEvent, event);
#if ENABLE(DOM_STORAGE)
    else if (event->isStorageEvent())
        wrapper = CREATE_DOM_OBJECT_WRAPPER(exec, globalObject, StorageEvent, event);
#endif
    else if (event->isWebKitAnimationEvent())
        wrapper = CREATE_DOM_OBJECT_WRAPPER(exec, globalObject, WebKitAnimationEvent, event);
    else if (event->isWebKitTransitionEvent())
        wrapper = CREATE_DOM_OBJECT_WRAPPER(exec, globalObject, WebKitTransitionEvent, event);
#if ENABLE(WORKERS)
    else if (event->isErrorEvent())
        wrapper = CREATE_DOM_OBJECT_WRAPPER(exec, globalObject, ErrorEvent, event);
#endif
    else if (event->isPopStateEvent())
        wrapper = CREATE_DOM_OBJECT_WRAPPER(exec, globalObject, PopStateEvent, event);
    else
        wrapper = CREATE_DOM_OBJECT_WRAPPER(exec, globalObject, Event, event);

    return wrapper;
}

} // namespace WebCore
