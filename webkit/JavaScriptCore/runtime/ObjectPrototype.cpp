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
 *  Copyright (C) 1999-2000 Harri Porten (porten@kde.org)
 *  Copyright (C) 2008 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "config.h"
#include "ObjectPrototype.h"

#include "Error.h"
#include "JSFunction.h"
#include "JSString.h"
#include "JSStringBuilder.h"
#include "PrototypeFunction.h"

namespace JSC {

ASSERT_CLASS_FITS_IN_CELL(ObjectPrototype);

static JSValue JSC_HOST_CALL objectProtoFuncValueOf(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue JSC_HOST_CALL objectProtoFuncHasOwnProperty(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue JSC_HOST_CALL objectProtoFuncIsPrototypeOf(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue JSC_HOST_CALL objectProtoFuncDefineGetter(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue JSC_HOST_CALL objectProtoFuncDefineSetter(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue JSC_HOST_CALL objectProtoFuncLookupGetter(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue JSC_HOST_CALL objectProtoFuncLookupSetter(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue JSC_HOST_CALL objectProtoFuncPropertyIsEnumerable(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue JSC_HOST_CALL objectProtoFuncToLocaleString(ExecState*, JSObject*, JSValue, const ArgList&);

ObjectPrototype::ObjectPrototype(ExecState* exec, NonNullPassRefPtr<Structure> stucture, Structure* prototypeFunctionStructure)
    : JSObject(stucture)
    , m_hasNoPropertiesWithUInt32Names(true)
{
    putDirectFunctionWithoutTransition(exec, new (exec) NativeFunctionWrapper(exec, prototypeFunctionStructure, 0, exec->propertyNames().toString, objectProtoFuncToString), DontEnum);
    putDirectFunctionWithoutTransition(exec, new (exec) NativeFunctionWrapper(exec, prototypeFunctionStructure, 0, exec->propertyNames().toLocaleString, objectProtoFuncToLocaleString), DontEnum);
    putDirectFunctionWithoutTransition(exec, new (exec) NativeFunctionWrapper(exec, prototypeFunctionStructure, 0, exec->propertyNames().valueOf, objectProtoFuncValueOf), DontEnum);
    putDirectFunctionWithoutTransition(exec, new (exec) NativeFunctionWrapper(exec, prototypeFunctionStructure, 1, exec->propertyNames().hasOwnProperty, objectProtoFuncHasOwnProperty), DontEnum);
    putDirectFunctionWithoutTransition(exec, new (exec) NativeFunctionWrapper(exec, prototypeFunctionStructure, 1, exec->propertyNames().propertyIsEnumerable, objectProtoFuncPropertyIsEnumerable), DontEnum);
    putDirectFunctionWithoutTransition(exec, new (exec) NativeFunctionWrapper(exec, prototypeFunctionStructure, 1, exec->propertyNames().isPrototypeOf, objectProtoFuncIsPrototypeOf), DontEnum);

    // Mozilla extensions
    putDirectFunctionWithoutTransition(exec, new (exec) NativeFunctionWrapper(exec, prototypeFunctionStructure, 2, exec->propertyNames().__defineGetter__, objectProtoFuncDefineGetter), DontEnum);
    putDirectFunctionWithoutTransition(exec, new (exec) NativeFunctionWrapper(exec, prototypeFunctionStructure, 2, exec->propertyNames().__defineSetter__, objectProtoFuncDefineSetter), DontEnum);
    putDirectFunctionWithoutTransition(exec, new (exec) NativeFunctionWrapper(exec, prototypeFunctionStructure, 1, exec->propertyNames().__lookupGetter__, objectProtoFuncLookupGetter), DontEnum);
    putDirectFunctionWithoutTransition(exec, new (exec) NativeFunctionWrapper(exec, prototypeFunctionStructure, 1, exec->propertyNames().__lookupSetter__, objectProtoFuncLookupSetter), DontEnum);
}

void ObjectPrototype::put(ExecState* exec, const Identifier& propertyName, JSValue value, PutPropertySlot& slot)
{
    JSObject::put(exec, propertyName, value, slot);

    if (m_hasNoPropertiesWithUInt32Names) {
        bool isUInt32;
        propertyName.toStrictUInt32(&isUInt32);
        m_hasNoPropertiesWithUInt32Names = !isUInt32;
    }
}

bool ObjectPrototype::getOwnPropertySlot(ExecState* exec, unsigned propertyName, PropertySlot& slot)
{
    if (m_hasNoPropertiesWithUInt32Names)
        return false;
    return JSObject::getOwnPropertySlot(exec, propertyName, slot);
}

// ------------------------------ Functions --------------------------------

// ECMA 15.2.4.2, 15.2.4.4, 15.2.4.5, 15.2.4.7

JSValue JSC_HOST_CALL objectProtoFuncValueOf(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    return thisValue.toThisObject(exec);
}

JSValue JSC_HOST_CALL objectProtoFuncHasOwnProperty(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    return jsBoolean(thisValue.toThisObject(exec)->hasOwnProperty(exec, Identifier(exec, args.at(0).toString(exec))));
}

JSValue JSC_HOST_CALL objectProtoFuncIsPrototypeOf(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    JSObject* thisObj = thisValue.toThisObject(exec);

    if (!args.at(0).isObject())
        return jsBoolean(false);

    JSValue v = asObject(args.at(0))->prototype();

    while (true) {
        if (!v.isObject())
            return jsBoolean(false);
        if (v == thisObj)
            return jsBoolean(true);
        v = asObject(v)->prototype();
    }
}

JSValue JSC_HOST_CALL objectProtoFuncDefineGetter(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    CallData callData;
    if (args.at(1).getCallData(callData) == CallTypeNone)
        return throwError(exec, SyntaxError, "invalid getter usage");
    thisValue.toThisObject(exec)->defineGetter(exec, Identifier(exec, args.at(0).toString(exec)), asObject(args.at(1)));
    return jsUndefined();
}

JSValue JSC_HOST_CALL objectProtoFuncDefineSetter(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    CallData callData;
    if (args.at(1).getCallData(callData) == CallTypeNone)
        return throwError(exec, SyntaxError, "invalid setter usage");
    thisValue.toThisObject(exec)->defineSetter(exec, Identifier(exec, args.at(0).toString(exec)), asObject(args.at(1)));
    return jsUndefined();
}

JSValue JSC_HOST_CALL objectProtoFuncLookupGetter(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    return thisValue.toThisObject(exec)->lookupGetter(exec, Identifier(exec, args.at(0).toString(exec)));
}

JSValue JSC_HOST_CALL objectProtoFuncLookupSetter(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    return thisValue.toThisObject(exec)->lookupSetter(exec, Identifier(exec, args.at(0).toString(exec)));
}

JSValue JSC_HOST_CALL objectProtoFuncPropertyIsEnumerable(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    return jsBoolean(thisValue.toThisObject(exec)->propertyIsEnumerable(exec, Identifier(exec, args.at(0).toString(exec))));
}

JSValue JSC_HOST_CALL objectProtoFuncToLocaleString(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    return thisValue.toThisJSString(exec);
}

JSValue JSC_HOST_CALL objectProtoFuncToString(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    return jsMakeNontrivialString(exec, "[object ", thisValue.toThisObject(exec)->className(), "]");
}

} // namespace JSC
