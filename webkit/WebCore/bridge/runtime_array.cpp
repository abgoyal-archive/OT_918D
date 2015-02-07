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
 * Copyright (C) 2003, 2008 Apple Inc. All rights reserved.
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
#include "runtime_array.h"

#include <runtime/ArrayPrototype.h>
#include <runtime/Error.h>
#include <runtime/PropertyNameArray.h>
#include "JSDOMBinding.h"

using namespace WebCore;

namespace JSC {

const ClassInfo RuntimeArray::s_info = { "RuntimeArray", &JSArray::info, 0, 0 };

RuntimeArray::RuntimeArray(ExecState* exec, Bindings::Array* array)
    // FIXME: deprecatedGetDOMStructure uses the prototype off of the wrong global object
    // We need to pass in the right global object for "array".
    : JSObject(deprecatedGetDOMStructure<RuntimeArray>(exec))
    , _array(array)
{
}

JSValue RuntimeArray::lengthGetter(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    RuntimeArray* thisObj = static_cast<RuntimeArray*>(asObject(slot.slotBase()));
    return jsNumber(exec, thisObj->getLength());
}

JSValue RuntimeArray::indexGetter(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    RuntimeArray* thisObj = static_cast<RuntimeArray*>(asObject(slot.slotBase()));
    return thisObj->getConcreteArray()->valueAt(exec, slot.index());
}

void RuntimeArray::getOwnPropertyNames(ExecState* exec, PropertyNameArray& propertyNames, EnumerationMode mode)
{
    unsigned length = getLength();
    for (unsigned i = 0; i < length; ++i)
        propertyNames.add(Identifier::from(exec, i));

    if (mode == IncludeDontEnumProperties)
        propertyNames.add(exec->propertyNames().length);

    JSObject::getOwnPropertyNames(exec, propertyNames, mode);
}

bool RuntimeArray::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    if (propertyName == exec->propertyNames().length) {
        slot.setCustom(this, lengthGetter);
        return true;
    }
    
    bool ok;
    unsigned index = propertyName.toArrayIndex(&ok);
    if (ok) {
        if (index < getLength()) {
            slot.setCustomIndex(this, index, indexGetter);
            return true;
        }
    }
    
    return JSObject::getOwnPropertySlot(exec, propertyName, slot);
}

bool RuntimeArray::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    if (propertyName == exec->propertyNames().length) {
        PropertySlot slot;
        slot.setCustom(this, lengthGetter);
        descriptor.setDescriptor(slot.getValue(exec, propertyName), ReadOnly | DontDelete | DontEnum);
        return true;
    }
    
    bool ok;
    unsigned index = propertyName.toArrayIndex(&ok);
    if (ok) {
        if (index < getLength()) {
            PropertySlot slot;
            slot.setCustomIndex(this, index, indexGetter);
            descriptor.setDescriptor(slot.getValue(exec, propertyName), DontDelete | DontEnum);
            return true;
        }
    }
    
    return JSObject::getOwnPropertyDescriptor(exec, propertyName, descriptor);
}

bool RuntimeArray::getOwnPropertySlot(ExecState *exec, unsigned index, PropertySlot& slot)
{
    if (index < getLength()) {
        slot.setCustomIndex(this, index, indexGetter);
        return true;
    }
    
    return JSObject::getOwnPropertySlot(exec, index, slot);
}

void RuntimeArray::put(ExecState* exec, const Identifier& propertyName, JSValue value, PutPropertySlot& slot)
{
    if (propertyName == exec->propertyNames().length) {
        throwError(exec, RangeError);
        return;
    }
    
    bool ok;
    unsigned index = propertyName.toArrayIndex(&ok);
    if (ok) {
        getConcreteArray()->setValueAt(exec, index, value);
        return;
    }
    
    JSObject::put(exec, propertyName, value, slot);
}

void RuntimeArray::put(ExecState* exec, unsigned index, JSValue value)
{
    if (index >= getLength()) {
        throwError(exec, RangeError);
        return;
    }
    
    getConcreteArray()->setValueAt(exec, index, value);
}

bool RuntimeArray::deleteProperty(ExecState*, const Identifier&)
{
    return false;
}

bool RuntimeArray::deleteProperty(ExecState*, unsigned)
{
    return false;
}

}