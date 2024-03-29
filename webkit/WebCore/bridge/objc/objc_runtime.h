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
 * Copyright (C) 2004, 2008 Apple Inc. All rights reserved.
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

#ifndef KJS_BINDINGS_OBJC_RUNTIME_H
#define KJS_BINDINGS_OBJC_RUNTIME_H

#include "Bridge.h"
#include "objc_header.h"
#include <runtime/JSGlobalObject.h>
#include <wtf/RetainPtr.h>

namespace JSC {
namespace Bindings {

ClassStructPtr webScriptObjectClass();
ClassStructPtr webUndefinedClass();

class ObjcInstance;

class ObjcField : public Field {
public:
    ObjcField(IvarStructPtr);
    ObjcField(CFStringRef name);
    
    virtual JSValue valueFromInstance(ExecState*, const Instance*) const;
    virtual void setValueToInstance(ExecState*, const Instance*, JSValue) const;

private:
    IvarStructPtr _ivar;
    RetainPtr<CFStringRef> _name;
};

class ObjcMethod : public Method {
public:
    ObjcMethod() : _objcClass(0), _selector(0), _javaScriptName(0) {}
    ObjcMethod(ClassStructPtr aClass, SEL _selector);

    virtual int numParameters() const;

    NSMethodSignature *getMethodSignature() const;
    
    bool isFallbackMethod() const { return _selector == @selector(invokeUndefinedMethodFromWebScript:withArguments:); }
    void setJavaScriptName(CFStringRef n) { _javaScriptName = n; }
    CFStringRef javaScriptName() const { return _javaScriptName.get(); }
    
    SEL selector() const { return _selector; }

private:
    ClassStructPtr _objcClass;
    SEL _selector;
    RetainPtr<CFStringRef> _javaScriptName;
};

class ObjcArray : public Array {
public:
    ObjcArray(ObjectStructPtr, PassRefPtr<RootObject>);

    virtual void setValueAt(ExecState *exec, unsigned int index, JSValue aValue) const;
    virtual JSValue valueAt(ExecState *exec, unsigned int index) const;
    virtual unsigned int getLength() const;
    
    ObjectStructPtr getObjcArray() const { return _array.get(); }

    static JSValue convertObjcArrayToArray(ExecState *exec, ObjectStructPtr anObject);

private:
    RetainPtr<ObjectStructPtr> _array;
};

class ObjcFallbackObjectImp : public JSObject {
public:
    ObjcFallbackObjectImp(ExecState*, ObjcInstance*, const Identifier& propertyName);

    static const ClassInfo s_info;

    const Identifier& propertyName() const { return _item; }

    static ObjectPrototype* createPrototype(ExecState*, JSGlobalObject* globalObject)
    {
        return globalObject->objectPrototype();
    }

    static PassRefPtr<Structure> createStructure(JSValue prototype)
    {
        return Structure::create(prototype, TypeInfo(ObjectType, StructureFlags), AnonymousSlotCount);
    }

private:
    static const unsigned StructureFlags = OverridesGetOwnPropertySlot | JSObject::StructureFlags;
    virtual bool getOwnPropertySlot(ExecState*, const Identifier&, PropertySlot&);
    virtual bool getOwnPropertyDescriptor(ExecState*, const Identifier&, PropertyDescriptor&);
    virtual void put(ExecState*, const Identifier& propertyName, JSValue, PutPropertySlot&);
    virtual CallType getCallData(CallData&);
    virtual bool deleteProperty(ExecState*, const Identifier& propertyName);
    virtual JSValue defaultValue(ExecState*, PreferredPrimitiveType) const;

    virtual bool toBoolean(ExecState*) const;

    virtual const ClassInfo* classInfo() const { return &s_info; }

    RefPtr<ObjcInstance> _instance;
    Identifier _item;
};

} // namespace Bindings
} // namespace JSC

#endif
