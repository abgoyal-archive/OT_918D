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
#include "V8Database.h"

#include "Database.h"
#include "V8Binding.h"
#include "V8CustomSQLTransactionCallback.h"
#include "V8CustomSQLTransactionErrorCallback.h"
#include "V8CustomVoidCallback.h"
#include "V8Proxy.h"

namespace WebCore {

v8::Handle<v8::Value> V8Database::changeVersionCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.Database.changeVersion()");

    if (args.Length() < 2)
        return throwError("The old and new version strings are required.", V8Proxy::SyntaxError);

    if (!(args[0]->IsString() && args[1]->IsString()))
        return throwError("The old and new versions must be strings.");

    Database* database = V8Database::toNative(args.Holder());

    Frame* frame = V8Proxy::retrieveFrameForCurrentContext();
    if (!frame)
        return v8::Undefined();

    RefPtr<V8CustomSQLTransactionCallback> callback;
    if (args.Length() > 2) {
        if (!args[2]->IsObject())
            return throwError("changeVersion transaction callback must be of valid type.");

        callback = V8CustomSQLTransactionCallback::create(args[2], frame);
    }

    RefPtr<V8CustomSQLTransactionErrorCallback> errorCallback;
    if (args.Length() > 3) {
        if (!args[3]->IsObject())
            return throwError("changeVersion error callback must be of valid type.");

        errorCallback = V8CustomSQLTransactionErrorCallback::create(args[3], frame);
    }

    RefPtr<V8CustomVoidCallback> successCallback;
    if (args.Length() > 4) {
        if (!args[4]->IsObject())
            return throwError("changeVersion success callback must be of valid type.");

        successCallback = V8CustomVoidCallback::create(args[4], frame);
    }

    database->changeVersion(toWebCoreString(args[0]), toWebCoreString(args[1]), callback.release(), errorCallback.release(), successCallback.release());

    return v8::Undefined();
}

static v8::Handle<v8::Value> createTransaction(const v8::Arguments& args, bool readOnly)
{
    if (!args.Length())
        return throwError("Transaction callback is required.", V8Proxy::SyntaxError);

    if (!args[0]->IsObject())
        return throwError("Transaction callback must be of valid type.");

    Database* database = V8Database::toNative(args.Holder());

    Frame* frame = V8Proxy::retrieveFrameForCurrentContext();
    if (!frame)
        return v8::Undefined();

    RefPtr<V8CustomSQLTransactionCallback> callback = V8CustomSQLTransactionCallback::create(args[0], frame);

    RefPtr<V8CustomSQLTransactionErrorCallback> errorCallback;
    if (args.Length() > 1) {
        if (!args[1]->IsObject())
            return throwError("Transaction error callback must be of valid type.");

        errorCallback = V8CustomSQLTransactionErrorCallback::create(args[1], frame);
    }

    RefPtr<V8CustomVoidCallback> successCallback;
    if (args.Length() > 2) {
        if (!args[2]->IsObject())
            return throwError("Transaction success callback must be of valid type.");

        successCallback = V8CustomVoidCallback::create(args[2], frame);
    }

    database->transaction(callback.release(), errorCallback.release(), successCallback.release(), readOnly);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8Database::transactionCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.Database.transaction()");
    return createTransaction(args, false);
}

v8::Handle<v8::Value> V8Database::readTransactionCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.Database.readTransaction()");
    return createTransaction(args, true);
}

} // namespace WebCore

#endif
