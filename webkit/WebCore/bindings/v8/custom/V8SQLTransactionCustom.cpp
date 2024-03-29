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

#if ENABLE(DATABASE)

#include "V8SQLTransaction.h"

#include "Database.h"
#include "SQLValue.h"
#include "V8Binding.h"
#include "V8CustomSQLStatementCallback.h"
#include "V8CustomSQLStatementErrorCallback.h"
#include "V8Proxy.h"
#include <wtf/Vector.h>

using namespace WTF;

namespace WebCore {

v8::Handle<v8::Value> V8SQLTransaction::executeSqlCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.SQLTransaction.executeSql()");

    if (args.Length() == 0)
        return throwError("SQL statement is required.", V8Proxy::SyntaxError);

    String statement = toWebCoreString(args[0]);

    Vector<SQLValue> sqlValues;

    if (args.Length() > 1 && !isUndefinedOrNull(args[1])) {
        if (args[1]->IsObject()) {
            uint32_t sqlArgsLength = 0;
            v8::Local<v8::Object> sqlArgsObject = args[1]->ToObject();
            v8::Local<v8::Value> lengthGetter;
            {
                v8::TryCatch block;
                lengthGetter = sqlArgsObject->Get(v8::String::New("length"));
                if (block.HasCaught())
                    return throwError(block.Exception());
            }

            if (isUndefinedOrNull(lengthGetter))
                sqlArgsLength = sqlArgsObject->GetPropertyNames()->Length();
            else
                sqlArgsLength = lengthGetter->Uint32Value();

            for (unsigned int i = 0; i < sqlArgsLength; ++i) {
                v8::Local<v8::Integer> key = v8::Integer::New(i);
                v8::Local<v8::Value> value;
                {
                    v8::TryCatch block;
                    value = sqlArgsObject->Get(key);
                    if (block.HasCaught())
                        return throwError(block.Exception());
                }

                if (value.IsEmpty() || value->IsNull())
                    sqlValues.append(SQLValue());
                else if (value->IsNumber())
                    sqlValues.append(SQLValue(value->NumberValue()));
                else
                    sqlValues.append(SQLValue(toWebCoreString(value)));
            }
        } else
            return throwError("sqlArgs should be array or object!", V8Proxy::TypeError);
    }

    SQLTransaction* transaction = V8SQLTransaction::toNative(args.Holder());

    Frame* frame = V8Proxy::retrieveFrameForCurrentContext();

    RefPtr<SQLStatementCallback> callback;
    if (args.Length() > 2 && !isUndefinedOrNull(args[2])) {
        if (!args[2]->IsObject())
            return throwError("Statement callback must be of valid type.", V8Proxy::TypeError);

        if (frame)
            callback = V8CustomSQLStatementCallback::create(args[2], frame);
    }

    RefPtr<SQLStatementErrorCallback> errorCallback;
    if (args.Length() > 3 && !isUndefinedOrNull(args[3])) {
        if (!args[3]->IsObject())
            return throwError("Statement error callback must be of valid type.", V8Proxy::TypeError);

        if (frame)
            errorCallback = V8CustomSQLStatementErrorCallback::create(args[3], frame);
    }

    ExceptionCode ec = 0;
    transaction->executeSQL(statement, sqlValues, callback, errorCallback, ec);
    V8Proxy::setDOMException(ec);

    return v8::Undefined();
}

} // namespace WebCore

#endif

