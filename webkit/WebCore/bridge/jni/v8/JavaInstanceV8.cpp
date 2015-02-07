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
 * Copyright (C) 2003, 2008, 2010 Apple Inc. All rights reserved.
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
#include "JavaInstanceV8.h"

#include "JNIBridge.h"
#include "JNIUtilityPrivate.h"
#include "JavaClassV8.h"

#include <assert.h>

#define LOG_TAG "v8binding"
#include <utils/Log.h>

using namespace JSC::Bindings;

JavaInstance::JavaInstance(jobject instance)
{
    m_instance = new JObjectWrapper(instance);
    m_class = 0;
}

JavaInstance::~JavaInstance()
{
    m_instance = 0;
    delete m_class;
}

#define NUM_LOCAL_REFS 64

void JavaInstance::virtualBegin() {
    getJNIEnv()->PushLocalFrame(NUM_LOCAL_REFS);
}

void JavaInstance::virtualEnd() {
    getJNIEnv()->PopLocalFrame(0);
}

JavaClass* JavaInstance::getClass() const
{
    if (!m_class)
        m_class = new JavaClass(javaInstance());
    return m_class;
}

bool JavaInstance::invokeMethod(const char* methodName, const NPVariant* args, int count, NPVariant* resultValue)
{
    VOID_TO_NPVARIANT(*resultValue);

    MethodList methodList = getClass()->methodsNamed(methodName);

    size_t numMethods = methodList.size();

    // Try to find a good match for the overloaded method.  The
    // fundamental problem is that JavaScript doesn't have the
    // notion of method overloading and Java does.  We could
    // get a bit more sophisticated and attempt to does some
    // type checking as we as checking the number of parameters.
    JavaMethod* aMethod;
    JavaMethod* method = 0;
    for (size_t methodIndex = 0; methodIndex < numMethods; methodIndex++) {
        aMethod = methodList[methodIndex];
        if (aMethod->numParameters() == count) {
            method = aMethod;
            break;
        }
    }
    if (!method) {
        LOGW("unable to find an appropiate method\n");
        return false;
    }

    const JavaMethod* jMethod = static_cast<const JavaMethod*>(method);

    jvalue* jArgs = 0;
    if (count > 0)
        jArgs = static_cast<jvalue*>(malloc(count * sizeof(jvalue)));

    for (int i = 0; i < count; i++) {
        JavaParameter* aParameter = jMethod->parameterAt(i);
        jArgs[i] = convertNPVariantToJValue(args[i], aParameter->getJNIType(), aParameter->type());
    }

    jvalue result;

    // The following code can be conditionally removed once we have a Tiger update that
    // contains the new Java plugin.  It is needed for builds prior to Tiger.
    {
        jobject obj = javaInstance();
        switch (jMethod->JNIReturnType()) {
        case void_type:
            callJNIMethodIDA<void>(obj, jMethod->methodID(obj), jArgs);
            break;
        case object_type:
            result.l = callJNIMethodIDA<jobject>(obj, jMethod->methodID(obj), jArgs);
            break;
        case boolean_type:
            result.z = callJNIMethodIDA<jboolean>(obj, jMethod->methodID(obj), jArgs);
            break;
        case byte_type:
            result.b = callJNIMethodIDA<jbyte>(obj, jMethod->methodID(obj), jArgs);
            break;
        case char_type:
            result.c = callJNIMethodIDA<jchar>(obj, jMethod->methodID(obj), jArgs);
            break;
        case short_type:
            result.s = callJNIMethodIDA<jshort>(obj, jMethod->methodID(obj), jArgs);
            break;
        case int_type:
            result.i = callJNIMethodIDA<jint>(obj, jMethod->methodID(obj), jArgs);
            break;

        case long_type:
            result.j = callJNIMethodIDA<jlong>(obj, jMethod->methodID(obj), jArgs);
            break;
        case float_type:
            result.f = callJNIMethodIDA<jfloat>(obj, jMethod->methodID(obj), jArgs);
            break;
        case double_type:
            result.d = callJNIMethodIDA<jdouble>(obj, jMethod->methodID(obj), jArgs);
            break;
        case invalid_type:
        default:
            break;
        }
    }

    convertJValueToNPVariant(result, jMethod->JNIReturnType(), jMethod->returnType(), resultValue);
    free(jArgs);

    return true;
}

JObjectWrapper::JObjectWrapper(jobject instance)
    : m_refCount(0)
{
    assert(instance);
// ANDROID
    if (!instance)
        LOGE("Attempted to create JObjectWrapper for null object");
// END ANDROID

    // Cache the JNIEnv used to get the global ref for this java instanace.
    // It'll be used to delete the reference.
    m_env = getJNIEnv();

    m_instance = m_env->NewGlobalRef(instance);

    LOGV("new global ref %p for %p\n", m_instance, instance);

    if (!m_instance)
// ANDROID
        LOGE("%s:  could not get GlobalRef for %p\n", __PRETTY_FUNCTION__, instance);
// END ANDROID
}

JObjectWrapper::~JObjectWrapper()
{
    LOGV("deleting global ref %p\n", m_instance);
    m_env->DeleteGlobalRef(m_instance);
}
