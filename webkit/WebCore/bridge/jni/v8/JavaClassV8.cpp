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
 * Copyright 2010, The Android Open Source Project
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "JavaClassV8.h"


using namespace JSC::Bindings;

JavaClass::JavaClass(jobject anInstance)
{
    jobject aClass = callJNIMethod<jobject>(anInstance, "getClass", "()Ljava/lang/Class;");

    if (!aClass) {
        fprintf(stderr, "%s:  unable to call getClass on instance %p\n", __PRETTY_FUNCTION__, anInstance);
        return;
    }

    jstring className = static_cast<jstring>(callJNIMethod<jobject>(aClass, "getName", "()Ljava/lang/String;"));
    const char* classNameC = getCharactersFromJString(className);
    m_name = strdup(classNameC);
    releaseCharactersForJString(className, classNameC);

    int i;
    JNIEnv* env = getJNIEnv();

    // Get the fields
    jarray fields = static_cast<jarray>(callJNIMethod<jobject>(aClass, "getFields", "()[Ljava/lang/reflect/Field;"));
    int numFields = env->GetArrayLength(fields);
    for (i = 0; i < numFields; i++) {
        jobject aJField = env->GetObjectArrayElement(static_cast<jobjectArray>(fields), i);
        JavaField* aField = new JavaField(env, aJField); // deleted in the JavaClass destructor
        {
            m_fields.set(aField->name().UTF8String(), aField);
        }
        env->DeleteLocalRef(aJField);
    }

    // Get the methods
    jarray methods = static_cast<jarray>(callJNIMethod<jobject>(aClass, "getMethods", "()[Ljava/lang/reflect/Method;"));
    int numMethods = env->GetArrayLength(methods);
    for (i = 0; i < numMethods; i++) {
        jobject aJMethod = env->GetObjectArrayElement(static_cast<jobjectArray>(methods), i);
        JavaMethod* aMethod = new JavaMethod(env, aJMethod); // deleted in the JavaClass destructor
        MethodList* methodList;
        {
            methodList = m_methods.get(aMethod->name().UTF8String());
            if (!methodList) {
                methodList = new MethodList();
                m_methods.set(aMethod->name().UTF8String(), methodList);
            }
        }
        methodList->append(aMethod);
        env->DeleteLocalRef(aJMethod);
    }
    env->DeleteLocalRef(fields);
    env->DeleteLocalRef(methods);
    env->DeleteLocalRef(aClass);
}

JavaClass::~JavaClass()
{
    free(const_cast<char*>(m_name));

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

MethodList JavaClass::methodsNamed(const char* name) const
{
    MethodList* methodList = m_methods.get(name);

    if (methodList)
        return *methodList;
    return MethodList();
}

JavaField* JavaClass::fieldNamed(const char* name) const
{
    return m_fields.get(name);
}
