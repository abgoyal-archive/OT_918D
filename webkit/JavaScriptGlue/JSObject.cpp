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
 * Copyright (C) 2005 Apple Computer, Inc.  All rights reserved.
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
#include "JSObject.h"

#include "UserObjectImp.h"

JSUserObject::JSUserObject(JSObjectCallBacksPtr callBacks, JSObjectMarkProcPtr markProc, void *data, int dataType)
    : JSBase(kJSObjectTypeID), fCallBacks(*callBacks), fMarkProc(markProc), fData(data), fDataType(dataType)
{
}

JSUserObject::~JSUserObject()
{
    if (fCallBacks.dispose)
    {
        fCallBacks.dispose(fData);
    }
}

CFArrayRef JSUserObject::CopyPropertyNames(void)
{
    CFArrayRef result = 0;
    if (fCallBacks.copyPropertyNames)
    {
        result = fCallBacks.copyPropertyNames(fData);
    }
    return result;
}

JSUserObject* JSUserObject::CopyProperty(CFStringRef propertyName)
{
    JSUserObject* result = 0;
    if (fCallBacks.copyProperty)
    {
        result = (JSUserObject*)fCallBacks.copyProperty(fData, propertyName);
    }
    return result;
}

void JSUserObject::SetProperty(CFStringRef propertyName, JSUserObject* value)
{
    if (fCallBacks.setProperty)
    {
        fCallBacks.setProperty(fData, propertyName, (JSObjectRef)value);
    }

}

static JSValue JSC_HOST_CALL nativeCallFunction(ExecState* exec, JSObject* functionObject, JSValue  thisValue, const ArgList& args);
static JSValue  nativeCallFunction(ExecState* exec, JSObject* functionObject, JSValue  thisValue, const ArgList& args)
{
    return static_cast<UserObjectImp*>(functionObject)->callAsFunction(exec, asObject(thisValue), args);
}

CallType JSUserObject::getCallData(CallData& callData)
{
    if (!fCallBacks.callFunction)
        return CallTypeNone;

    callData.native.function = nativeCallFunction;
    return CallTypeHost;
}

JSUserObject* JSUserObject::CallFunction(JSUserObject* thisObj, CFArrayRef args)
{
    JSUserObject* result = 0;
    if (fCallBacks.callFunction)
    {
        result = (JSUserObject*)fCallBacks.callFunction(fData, (JSObjectRef)thisObj, args);
    }
    return result;

}

CFTypeRef JSUserObject::CopyCFValue() const
{
    CFTypeRef result = 0;
    if (fCallBacks.copyCFValue)
    {
        result = (JSUserObject*)fCallBacks.copyCFValue(fData);
    }
    return result;
}

UInt8 JSUserObject::Equal(JSBase* other)
{
    UInt8 result = false;
    JSUserObject* obj = (JSUserObject*)other;
    if (obj->GetTypeID() == kJSObjectTypeID)
    {
        if (fCallBacks.equal)
        {
            result = fCallBacks.equal(GetData(), obj->GetData());
        }
        else
        {
            CFTypeRef cf1 = CopyCFValue();
            CFTypeRef cf2 = obj->CopyCFValue();
            if (cf1 && cf2)
            {
                result = CFEqual(cf1, cf2);
            }
            ReleaseCFType(cf2);
            ReleaseCFType(cf1);
        }
    }
    return result;
}

void JSUserObject::Mark()
{
    if (fMarkProc)
    {
        fMarkProc(fData);
    }
}

void *JSUserObject::GetData()
{
    return fData;
}


