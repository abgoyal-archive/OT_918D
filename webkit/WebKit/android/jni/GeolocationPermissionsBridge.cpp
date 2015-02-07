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
 * Copyright 2009, The Android Open Source Project
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

#include <JNIHelp.h>  // For jniRegisterNativeMethods
#include "GeolocationPermissions.h"
#include "WebCoreJni.h"  // For to_string


/**
 * This file provides a set of functions to bridge between the Java and C++
 * GeolocationPermissions classes. The java GeolocationPermissions object calls
 * the functions provided here, which in turn call static methods on the C++
 * GeolocationPermissions class.
 */

namespace android {

static jobject getOrigins(JNIEnv* env, jobject obj)
{
    GeolocationPermissions::OriginSet origins = GeolocationPermissions::getOrigins();

    jclass setClass = env->FindClass("java/util/HashSet");
    jmethodID constructor = env->GetMethodID(setClass, "<init>", "()V");
    jmethodID addMethod = env->GetMethodID(setClass, "add", "(Ljava/lang/Object;)Z");
    jobject set = env->NewObject(setClass, constructor);

    GeolocationPermissions::OriginSet::const_iterator end = origins.end();
    for (GeolocationPermissions::OriginSet::const_iterator iter = origins.begin(); iter != end; ++iter) {
        jstring originString = env->NewString(iter->characters(), iter->length());
        env->CallBooleanMethod(set, addMethod, originString);
        env->DeleteLocalRef(originString);
    }
    return set;
}

static bool getAllowed(JNIEnv* env, jobject obj, jstring origin)
{
    WebCore::String originString = to_string(env, origin);
    return GeolocationPermissions::getAllowed(originString);
}

static void clear(JNIEnv* env, jobject obj, jstring origin)
{
    WebCore::String originString = to_string(env, origin);
    GeolocationPermissions::clear(originString);
}

static void allow(JNIEnv* env, jobject obj, jstring origin)
{
    WebCore::String originString = to_string(env, origin);
    GeolocationPermissions::allow(originString);
}

static void clearAll(JNIEnv* env, jobject obj)
{
    GeolocationPermissions::clearAll();
}

/*
 * JNI registration
 */
static JNINativeMethod gGeolocationPermissionsMethods[] = {
    { "nativeGetOrigins", "()Ljava/util/Set;",
        (void*) getOrigins },
    { "nativeGetAllowed", "(Ljava/lang/String;)Z",
        (void*) getAllowed },
    { "nativeClear", "(Ljava/lang/String;)V",
        (void*) clear },
    { "nativeAllow", "(Ljava/lang/String;)V",
        (void*) allow },
    { "nativeClearAll", "()V",
        (void*) clearAll }
};

int register_geolocation_permissions(JNIEnv* env)
{
    const char* kGeolocationPermissionsClass = "android/webkit/GeolocationPermissions";
    jclass geolocationPermissions = env->FindClass(kGeolocationPermissionsClass);
    LOG_ASSERT(geolocationPermissions, "Unable to find class");

    return jniRegisterNativeMethods(env, kGeolocationPermissionsClass,
            gGeolocationPermissionsMethods, NELEM(gGeolocationPermissionsMethods));
}

}
