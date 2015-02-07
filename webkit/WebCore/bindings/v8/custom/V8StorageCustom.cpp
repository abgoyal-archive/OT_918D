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

#if ENABLE(DOM_STORAGE)
#include "V8Storage.h"

#include "Storage.h"
#include "V8Binding.h"
#include "V8Proxy.h"

namespace WebCore {

// Get an array containing the names of indexed properties in a collection.
v8::Handle<v8::Array> V8Storage::namedPropertyEnumerator(const v8::AccessorInfo& info)
{
    Storage* storage = V8Storage::toNative(info.Holder());
    unsigned int length = storage->length();
    v8::Handle<v8::Array> properties = v8::Array::New(length);
    for (unsigned int i = 0; i < length; ++i) {
        String key = storage->key(i);
        ASSERT(!key.isNull());
        String val = storage->getItem(key);
        properties->Set(v8::Integer::New(i), v8String(key));
    }

    return properties;
}

static v8::Handle<v8::Value> storageGetter(v8::Local<v8::String> v8Name, const v8::AccessorInfo& info)
{
    Storage* storage = V8Storage::toNative(info.Holder());
    String name = toWebCoreString(v8Name);

    if (storage->contains(name) && name != "length")
        return v8String(storage->getItem(name));

    return notHandledByInterceptor();
}

v8::Handle<v8::Value> V8Storage::indexedPropertyGetter(uint32_t index, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.Storage.IndexedPropertyGetter");
    v8::Local<v8::Integer> indexV8 = v8::Integer::New(index);
    return storageGetter(indexV8->ToString(), info);
}

v8::Handle<v8::Value> V8Storage::namedPropertyGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.Storage.NamedPropertyGetter");
    return storageGetter(name, info);
}

static v8::Handle<v8::Value> storageSetter(v8::Local<v8::String> v8Name, v8::Local<v8::Value> v8Value, const v8::AccessorInfo& info)
{
    Storage* storage = V8Storage::toNative(info.Holder());
    String name = toWebCoreString(v8Name);
    String value = toWebCoreString(v8Value);

    // Silently ignore length (rather than letting the bindings raise an exception).
    if (name == "length")
        return v8Value;

    v8::Handle<v8::Value> prototypeValue = info.Holder()->GetRealNamedPropertyInPrototypeChain(v8Name);
    if (!prototypeValue.IsEmpty())
        return notHandledByInterceptor();

    ExceptionCode ec = 0;
    storage->setItem(name, value, ec);
    if (ec)
        return throwError(ec);

    return v8Value;
}

v8::Handle<v8::Value> V8Storage::indexedPropertySetter(uint32_t index, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.Storage.NamedPropertyGetter");
    v8::Local<v8::Integer> indexV8 = v8::Integer::New(index);
    return storageSetter(indexV8->ToString(), value, info);
}

v8::Handle<v8::Value> V8Storage::namedPropertySetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.Storage.NamedPropertySetter");
    return storageSetter(name, value, info);
}

static v8::Handle<v8::Boolean> storageDeleter(v8::Local<v8::String> v8Name, const v8::AccessorInfo& info)
{
    Storage* storage = V8Storage::toNative(info.Holder());
    String name = toWebCoreString(v8Name);
    
    if (storage->contains(name)) {
        storage->removeItem(name);
        return v8::True();
    }

    return deletionNotHandledByInterceptor();
}

v8::Handle<v8::Boolean> V8Storage::indexedPropertyDeleter(uint32_t index, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.Storage.IndexedPropertyDeleter");
    v8::Local<v8::Integer> indexV8 = v8::Integer::New(index);
    return storageDeleter(indexV8->ToString(), info);
}

v8::Handle<v8::Boolean> V8Storage::namedPropertyDeleter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.Storage.NamedPropertyDeleter");
    return storageDeleter(name, info);
}

} // namespace WebCore

#endif
