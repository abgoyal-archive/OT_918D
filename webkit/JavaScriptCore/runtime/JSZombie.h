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
 * Copyright (C) 2009 Apple Inc. All rights reserved.
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

#ifndef JSZombie_h
#define JSZombie_h

#include "JSCell.h"

#if ENABLE(JSC_ZOMBIES)
namespace JSC {

class JSZombie : public JSCell {
public:
    JSZombie(const ClassInfo* oldInfo, Structure* structure)
        : JSCell(structure)
        , m_oldInfo(oldInfo)
    {
    }
    virtual bool isZombie() const { return true; }
    virtual const ClassInfo* classInfo() const { return &s_info; }
    static Structure* leakedZombieStructure();

    virtual bool isGetterSetter() const { ASSERT_NOT_REACHED(); return false; }
    virtual bool isAPIValueWrapper() const { ASSERT_NOT_REACHED(); return false; }
    virtual bool isPropertyNameIterator() const { ASSERT_NOT_REACHED(); return false; }
    virtual CallType getCallData(CallData&) { ASSERT_NOT_REACHED(); return CallTypeNone; }
    virtual ConstructType getConstructData(ConstructData&) { ASSERT_NOT_REACHED(); return ConstructTypeNone; }
    virtual bool getUInt32(uint32_t&) const { ASSERT_NOT_REACHED(); return false; }
    virtual JSValue toPrimitive(ExecState*, PreferredPrimitiveType) const { ASSERT_NOT_REACHED(); return jsNull(); }
    virtual bool getPrimitiveNumber(ExecState*, double&, JSValue&) { ASSERT_NOT_REACHED(); return false; }
    virtual bool toBoolean(ExecState*) const { ASSERT_NOT_REACHED(); return false; }
    virtual double toNumber(ExecState*) const { ASSERT_NOT_REACHED(); return 0.0; }
    virtual UString toString(ExecState*) const { ASSERT_NOT_REACHED(); return ""; }
    virtual JSObject* toObject(ExecState*) const { ASSERT_NOT_REACHED(); return 0; }
    virtual void markChildren(MarkStack&) { ASSERT_NOT_REACHED(); }
    virtual void put(ExecState*, const Identifier&, JSValue, PutPropertySlot&) { ASSERT_NOT_REACHED(); }
    virtual void put(ExecState*, unsigned, JSValue) { ASSERT_NOT_REACHED(); }
    virtual bool deleteProperty(ExecState*, const Identifier&) { ASSERT_NOT_REACHED(); return false; }
    virtual bool deleteProperty(ExecState*, unsigned) { ASSERT_NOT_REACHED(); return false; }
    virtual JSObject* toThisObject(ExecState*) const { ASSERT_NOT_REACHED(); return 0; }
    virtual UString toThisString(ExecState*) const { ASSERT_NOT_REACHED(); return ""; }
    virtual JSString* toThisJSString(ExecState*) { ASSERT_NOT_REACHED(); return 0; }
    virtual JSValue getJSNumber() { ASSERT_NOT_REACHED(); return jsNull(); }
    virtual bool getOwnPropertySlot(ExecState*, const Identifier&, PropertySlot&) { ASSERT_NOT_REACHED(); return false; }
    virtual bool getOwnPropertySlot(ExecState*, unsigned, PropertySlot&) { ASSERT_NOT_REACHED(); return false; }
    
    static const ClassInfo s_info;
private:
    const ClassInfo* m_oldInfo;
};

}

#endif // ENABLE(JSC_ZOMBIES)

#endif // JSZombie_h
