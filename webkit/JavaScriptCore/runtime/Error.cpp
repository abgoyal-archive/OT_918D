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
 *  Copyright (C) 1999-2001 Harri Porten (porten@kde.org)
 *  Copyright (C) 2001 Peter Kelly (pmk@post.com)
 *  Copyright (C) 2003, 2004, 2005, 2006, 2008 Apple Inc. All rights reserved.
 *  Copyright (C) 2007 Eric Seidel (eric@webkit.org)
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */

#include "config.h"
#include "Error.h"

#include "ConstructData.h"
#include "ErrorConstructor.h"
#include "JSFunction.h"
#include "JSGlobalObject.h"
#include "JSObject.h"
#include "JSString.h"
#include "NativeErrorConstructor.h"

namespace JSC {

const char* expressionBeginOffsetPropertyName = "expressionBeginOffset";
const char* expressionCaretOffsetPropertyName = "expressionCaretOffset";
const char* expressionEndOffsetPropertyName = "expressionEndOffset";

JSObject* Error::create(ExecState* exec, ErrorType type, const UString& message, int lineNumber, intptr_t sourceID, const UString& sourceURL)
{
    JSObject* constructor;
    const char* name;
    switch (type) {
        case EvalError:
            constructor = exec->lexicalGlobalObject()->evalErrorConstructor();
            name = "Evaluation error";
            break;
        case RangeError:
            constructor = exec->lexicalGlobalObject()->rangeErrorConstructor();
            name = "Range error";
            break;
        case ReferenceError:
            constructor = exec->lexicalGlobalObject()->referenceErrorConstructor();
            name = "Reference error";
            break;
        case SyntaxError:
            constructor = exec->lexicalGlobalObject()->syntaxErrorConstructor();
            name = "Syntax error";
            break;
        case TypeError:
            constructor = exec->lexicalGlobalObject()->typeErrorConstructor();
            name = "Type error";
            break;
        case URIError:
            constructor = exec->lexicalGlobalObject()->URIErrorConstructor();
            name = "URI error";
            break;
        default:
            constructor = exec->lexicalGlobalObject()->errorConstructor();
            name = "Error";
            break;
    }

    MarkedArgumentBuffer args;
    if (message.isEmpty())
        args.append(jsString(exec, name));
    else
        args.append(jsString(exec, message));
    ConstructData constructData;
    ConstructType constructType = constructor->getConstructData(constructData);
    JSObject* error = construct(exec, constructor, constructType, constructData, args);

    if (lineNumber != -1)
        error->putWithAttributes(exec, Identifier(exec, "line"), jsNumber(exec, lineNumber), ReadOnly | DontDelete);
    if (sourceID != -1)
        error->putWithAttributes(exec, Identifier(exec, "sourceId"), jsNumber(exec, sourceID), ReadOnly | DontDelete);
    if (!sourceURL.isNull())
        error->putWithAttributes(exec, Identifier(exec, "sourceURL"), jsString(exec, sourceURL), ReadOnly | DontDelete);

    return error;
}

JSObject* Error::create(ExecState* exec, ErrorType type, const char* message)
{
    return create(exec, type, message, -1, -1, UString());
}

JSObject* throwError(ExecState* exec, JSObject* error)
{
    exec->setException(error);
    return error;
}

JSObject* throwError(ExecState* exec, ErrorType type)
{
    JSObject* error = Error::create(exec, type, UString(), -1, -1, UString());
    exec->setException(error);
    return error;
}

JSObject* throwError(ExecState* exec, ErrorType type, const UString& message)
{
    JSObject* error = Error::create(exec, type, message, -1, -1, UString());
    exec->setException(error);
    return error;
}

JSObject* throwError(ExecState* exec, ErrorType type, const char* message)
{
    JSObject* error = Error::create(exec, type, message, -1, -1, UString());
    exec->setException(error);
    return error;
}

JSObject* throwError(ExecState* exec, ErrorType type, const UString& message, int line, intptr_t sourceID, const UString& sourceURL)
{
    JSObject* error = Error::create(exec, type, message, line, sourceID, sourceURL);
    exec->setException(error);
    return error;
}

} // namespace JSC
