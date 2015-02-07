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
 * Copyright (C) 2008 Apple Inc. All Rights Reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "JavaScriptProfile.h"

#if ENABLE(JAVASCRIPT_DEBUGGER) && USE(JSC)

#include "JavaScriptProfileNode.h"
#include <profiler/Profile.h>
#include <JavaScriptCore/APICast.h>
#include <JavaScriptCore/JSObjectRef.h>
#include <JavaScriptCore/JSStringRef.h>
#include <JavaScriptCore/OpaqueJSString.h>
#include <runtime/JSObject.h>
#include <runtime/JSValue.h>
#include <wtf/StdLibExtras.h>

using namespace JSC;

namespace WebCore {

// Cache

typedef HashMap<Profile*, JSObject*> ProfileMap;

static ProfileMap& profileCache()
{ 
    DEFINE_STATIC_LOCAL(ProfileMap, staticProfiles, ());
    return staticProfiles;
}

// Static Values

static JSClassRef ProfileClass();

static JSValueRef getTitleCallback(JSContextRef ctx, JSObjectRef thisObject, JSStringRef, JSValueRef*)
{
    if (!JSValueIsObjectOfClass(ctx, thisObject, ProfileClass()))
        return JSValueMakeUndefined(ctx);

    Profile* profile = static_cast<Profile*>(JSObjectGetPrivate(thisObject));
    return JSValueMakeString(ctx, OpaqueJSString::create(profile->title()).get());
}

static JSValueRef getHeadCallback(JSContextRef ctx, JSObjectRef thisObject, JSStringRef, JSValueRef*)
{
    if (!JSValueIsObjectOfClass(ctx, thisObject, ProfileClass()))
        return JSValueMakeUndefined(ctx);

    ExecState* exec = toJS(ctx);
    Profile* profile = static_cast<Profile*>(JSObjectGetPrivate(thisObject));
    return toRef(exec, toJS(exec, profile->head()));
}

static JSValueRef getUniqueIdCallback(JSContextRef ctx, JSObjectRef thisObject, JSStringRef, JSValueRef*)
{
    if (!JSValueIsObjectOfClass(ctx, thisObject, ProfileClass()))
        return JSValueMakeUndefined(ctx);

    Profile* profile = static_cast<Profile*>(JSObjectGetPrivate(thisObject));
    return JSValueMakeNumber(ctx, profile->uid());
}

// Static Functions

static JSValueRef focus(JSContextRef ctx, JSObjectRef /*function*/, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* /*exception*/)
{
    if (!JSValueIsObjectOfClass(ctx, thisObject, ProfileClass()))
        return JSValueMakeUndefined(ctx);

    if (argumentCount < 1)
        return JSValueMakeUndefined(ctx);

    if (!JSValueIsObjectOfClass(ctx, arguments[0], ProfileNodeClass()))
        return JSValueMakeUndefined(ctx);

    Profile* profile = static_cast<Profile*>(JSObjectGetPrivate(thisObject));
    profile->focus(static_cast<ProfileNode*>(JSObjectGetPrivate(const_cast<JSObjectRef>(arguments[0]))));

    return JSValueMakeUndefined(ctx);
}

static JSValueRef exclude(JSContextRef ctx, JSObjectRef /*function*/, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* /*exception*/)
{
    if (!JSValueIsObjectOfClass(ctx, thisObject, ProfileClass()))
        return JSValueMakeUndefined(ctx);

    if (argumentCount < 1)
        return JSValueMakeUndefined(ctx);

    if (!JSValueIsObjectOfClass(ctx, arguments[0], ProfileNodeClass()))
        return JSValueMakeUndefined(ctx);

    Profile* profile = static_cast<Profile*>(JSObjectGetPrivate(thisObject));
    profile->exclude(static_cast<ProfileNode*>(JSObjectGetPrivate(const_cast<JSObjectRef>(arguments[0]))));

    return JSValueMakeUndefined(ctx);
}

static JSValueRef restoreAll(JSContextRef ctx, JSObjectRef /*function*/, JSObjectRef thisObject, size_t /*argumentCount*/, const JSValueRef[] /*arguments*/, JSValueRef* /*exception*/)
{
    if (!JSValueIsObjectOfClass(ctx, thisObject, ProfileClass()))
        return JSValueMakeUndefined(ctx);

    Profile* profile = static_cast<Profile*>(JSObjectGetPrivate(thisObject));
    profile->restoreAll();

    return JSValueMakeUndefined(ctx);
}

static void finalize(JSObjectRef object)
{
    Profile* profile = static_cast<Profile*>(JSObjectGetPrivate(object));
    profileCache().remove(profile);
    profile->deref();
}

JSClassRef ProfileClass()
{
    static JSStaticValue staticValues[] = {
        { "title", getTitleCallback, 0, kJSPropertyAttributeNone },
        { "head", getHeadCallback, 0, kJSPropertyAttributeNone },
        { "uid", getUniqueIdCallback, 0, kJSPropertyAttributeNone },
        { 0, 0, 0, 0 }
    };

    static JSStaticFunction staticFunctions[] = {
        { "focus", focus, kJSPropertyAttributeNone },
        { "exclude", exclude, kJSPropertyAttributeNone },
        { "restoreAll", restoreAll, kJSPropertyAttributeNone },
        { 0, 0, 0 }
    };

    static JSClassDefinition classDefinition = {
        0, kJSClassAttributeNone, "Profile", 0, staticValues, staticFunctions,
        0, finalize, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    static JSClassRef profileClass = JSClassCreate(&classDefinition);
    return profileClass;
}

JSValue toJS(ExecState* exec, Profile* profile)
{
    if (!profile)
        return jsNull();

    JSObject* profileWrapper = profileCache().get(profile);
    if (profileWrapper)
        return profileWrapper;

    profile->ref();
    profileWrapper = toJS(JSObjectMake(toRef(exec), ProfileClass(), static_cast<void*>(profile)));
    profileCache().set(profile, profileWrapper);
    return profileWrapper;
}

} // namespace WebCore

#endif // ENABLE(JAVASCRIPT_DEBUGGER)
