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
 * Copyright (C) 2007 Apple Inc. All rights reserved.
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
#include "JSDatabase.h"

#if ENABLE(DATABASE)

#include "DOMWindow.h"
#include "Database.h"
#include "Document.h"
#include "ExceptionCode.h"
#include "JSCustomSQLTransactionCallback.h"
#include "JSCustomSQLTransactionErrorCallback.h"
#include "JSCustomVoidCallback.h"
#include "JSDOMWindowCustom.h"
#include "PlatformString.h"
#include "SQLValue.h"
#include <runtime/JSArray.h>

namespace WebCore {

using namespace JSC;

JSValue JSDatabase::changeVersion(ExecState* exec, const ArgList& args)
{
    String oldVersion = args.at(0).toString(exec);
    String newVersion = args.at(1).toString(exec);

    JSObject* object;
    if (!(object = args.at(2).getObject())) {
        setDOMException(exec, TYPE_MISMATCH_ERR);
        return jsUndefined();
    }
    
    RefPtr<SQLTransactionCallback> callback(JSCustomSQLTransactionCallback::create(object, static_cast<JSDOMGlobalObject*>(exec->dynamicGlobalObject())));
    
    RefPtr<SQLTransactionErrorCallback> errorCallback;
    if (!args.at(3).isNull()) {
        if (!(object = args.at(3).getObject())) {
            setDOMException(exec, TYPE_MISMATCH_ERR);
            return jsUndefined();
        }
        
        errorCallback = JSCustomSQLTransactionErrorCallback::create(object, static_cast<JSDOMGlobalObject*>(exec->dynamicGlobalObject()));
    }
    
    RefPtr<VoidCallback> successCallback;
    if (!args.at(4).isNull()) {
        if (!(object = args.at(4).getObject())) {
            setDOMException(exec, TYPE_MISMATCH_ERR);
            return jsUndefined();
        }

        successCallback = JSCustomVoidCallback::create(object, static_cast<JSDOMGlobalObject*>(exec->dynamicGlobalObject()));
    }

    m_impl->changeVersion(oldVersion, newVersion, callback.release(), errorCallback.release(), successCallback.release());
    
    return jsUndefined();
}

static JSValue createTransaction(ExecState* exec, const ArgList& args, Database* database, JSDOMGlobalObject* globalObject, bool readOnly)
{
    JSObject* object;
    
    if (!(object = args.at(0).getObject())) {
        setDOMException(exec, TYPE_MISMATCH_ERR);
        return jsUndefined();
    }        
     
    RefPtr<SQLTransactionCallback> callback(JSCustomSQLTransactionCallback::create(object, globalObject));
    RefPtr<SQLTransactionErrorCallback> errorCallback;
    
    if (args.size() > 1 && !args.at(1).isNull()) {
        if (!(object = args.at(1).getObject())) {
            setDOMException(exec, TYPE_MISMATCH_ERR);
            return jsUndefined();
        }

        errorCallback = JSCustomSQLTransactionErrorCallback::create(object, globalObject);
    }

    RefPtr<VoidCallback> successCallback;
    if (args.size() > 2 && !args.at(2).isNull()) {
        if (!(object = args.at(2).getObject())) {
            setDOMException(exec, TYPE_MISMATCH_ERR);
            return jsUndefined();
        }

        successCallback = JSCustomVoidCallback::create(object, globalObject);
    }
    
    database->transaction(callback.release(), errorCallback.release(), successCallback.release(), readOnly);
    return jsUndefined();
}

JSValue JSDatabase::transaction(ExecState* exec, const ArgList& args)
{
    return createTransaction(exec, args, m_impl.get(), static_cast<JSDOMGlobalObject*>(exec->dynamicGlobalObject()), false);
}
    
JSValue JSDatabase::readTransaction(ExecState* exec, const ArgList& args)
{
    return createTransaction(exec, args, m_impl.get(), static_cast<JSDOMGlobalObject*>(exec->dynamicGlobalObject()), true);
}

}

#endif // ENABLE(DATABASE)
