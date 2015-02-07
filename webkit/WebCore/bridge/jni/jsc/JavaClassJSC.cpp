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
 * Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010 Apple Inc. All rights reserved.
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
#include "JavaClassJSC.h"

#if ENABLE(MAC_JAVA_BRIDGE)

#include "JNIUtility.h"
#include "JSDOMWindow.h"
#include <runtime/Identifier.h>
#include <runtime/JSLock.h>

using namespace JSC::Bindings;

JavaClass::JavaClass(jobject anInstance)
{
    jobject aClass = callJNIMethod<jobject>(anInstance, "getClass", "()Ljava/lang/Class;");

    if (!aClass) {
        fprintf(stderr, "%s:  unable to call getClass on instance %p\n", __PRETTY_FUNCTION__, anInstance);
        m_name = fastStrDup("<Unknown>");
        return;
    }

    if (jstring className = (jstring)callJNIMethod<jobject>(aClass, "getName", "()Ljava/lang/String;")) {
        const char* classNameC = getCharactersFromJString(className);
        m_name = fastStrDup(classNameC);
        releaseCharactersForJString(className, classNameC);
    } else
        m_name = fastStrDup("<Unknown>");

    int i;
    JNIEnv* env = getJNIEnv();

    // Get the fields
    if (jarray fields = (jarray)callJNIMethod<jobject>(aClass, "getFields", "()[Ljava/lang/reflect/Field;")) {
        int numFields = env->GetArrayLength(fields);
        for (i = 0; i < numFields; i++) {
            jobject aJField = env->GetObjectArrayElement((jobjectArray)fields, i);
            JavaField* aField = new JavaField(env, aJField); // deleted in the JavaClass destructor
            {
                JSLock lock(SilenceAssertionsOnly);
                m_fields.set(((UString)aField->name()).rep(), aField);
            }
            env->DeleteLocalRef(aJField);
        }
        env->DeleteLocalRef(fields);
    }

    // Get the methods
    if (jarray methods = (jarray)callJNIMethod<jobject>(aClass, "getMethods", "()[Ljava/lang/reflect/Method;")) {
        int numMethods = env->GetArrayLength(methods);
        for (i = 0; i < numMethods; i++) {
            jobject aJMethod = env->GetObjectArrayElement((jobjectArray)methods, i);
            JavaMethod* aMethod = new JavaMethod(env, aJMethod); // deleted in the JavaClass destructor
            MethodList* methodList;
            {
                JSLock lock(SilenceAssertionsOnly);

                methodList = m_methods.get(((UString)aMethod->name()).rep());
                if (!methodList) {
                    methodList = new MethodList();
                    m_methods.set(((UString)aMethod->name()).rep(), methodList);
                }
            }
            methodList->append(aMethod);
            env->DeleteLocalRef(aJMethod);
        }
        env->DeleteLocalRef(methods);
    }

    env->DeleteLocalRef(aClass);
}

JavaClass::~JavaClass()
{
    fastFree(const_cast<char*>(m_name));

    JSLock lock(SilenceAssertionsOnly);

    deleteAllValues(m_fields);
    m_fields.clear();

    MethodListMap::const_iterator end = m_methods.end();
    for (MethodListMap::const_iterator it = m_methods.begin(); it != end; ++it) {
        const MethodList* methodList = it->second;
        deleteAllValues(*methodList);
        delete methodList;
    }
    m_methods.clear();
}

MethodList JavaClass::methodsNamed(const Identifier& identifier, Instance*) const
{
    MethodList* methodList = m_methods.get(identifier.ustring().rep());

    if (methodList)
        return *methodList;
    return MethodList();
}

Field* JavaClass::fieldNamed(const Identifier& identifier, Instance*) const
{
    return m_fields.get(identifier.ustring().rep());
}

bool JavaClass::isNumberClass() const
{
    return (!strcmp(m_name, "java.lang.Byte")
        || !strcmp(m_name, "java.lang.Short")
        || !strcmp(m_name, "java.lang.Integer")
        || !strcmp(m_name, "java.lang.Long")
        || !strcmp(m_name, "java.lang.Float")
        || !strcmp(m_name, "java.lang.Double"));
}

bool JavaClass::isBooleanClass() const
{
    return !strcmp(m_name, "java.lang.Boolean");
}

bool JavaClass::isStringClass() const
{
    return !strcmp(m_name, "java.lang.String");
}

#endif // ENABLE(MAC_JAVA_BRIDGE)
