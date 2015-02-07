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
 * Copyright (C) 2006, 2007, 2008, 2009 Google Inc. All rights reserved.
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

#ifndef V8Utilities_h
#define V8Utilities_h

#include <v8.h>

namespace WebCore {

    class EventListener;
    class Frame;
    class KURL;
    class ScriptExecutionContext;
    class ScriptState;
    class String;

    // Use an array to hold dependents. It works like a ref-counted scheme. A value can be added more than once to the DOM object.
    void createHiddenDependency(v8::Handle<v8::Object>, v8::Local<v8::Value>, int cacheIndex);
    void removeHiddenDependency(v8::Handle<v8::Object>, v8::Local<v8::Value>, int cacheIndex);
    
    // Combo create/remove, for generated event-handler-setter bindings:
    void transferHiddenDependency(v8::Handle<v8::Object>, EventListener* oldValue, v8::Local<v8::Value> newValue, int cacheIndex);
    
    bool processingUserGesture();
    bool shouldAllowNavigation(Frame*);
    KURL completeURL(const String& relativeURL);
    void navigateIfAllowed(Frame*, const KURL&, bool lockHistory, bool lockBackForwardList);

    ScriptExecutionContext* getScriptExecutionContext(ScriptState*);
    inline ScriptExecutionContext* getScriptExecutionContext() {
        return getScriptExecutionContext(0);
    }

    void reportException(ScriptState*, v8::TryCatch&);

    class AllowAllocation {
    public:
        inline AllowAllocation()
        {
            m_previous = m_current;
            m_current = true;
        }

        inline ~AllowAllocation()
        {
            m_current = m_previous;
        }

        static bool m_current;

    private:
        bool m_previous;
    };

    class SafeAllocation {
     public:
      static inline v8::Local<v8::Object> newInstance(v8::Handle<v8::Function>);
      static inline v8::Local<v8::Object> newInstance(v8::Handle<v8::ObjectTemplate>);
      static inline v8::Local<v8::Object> newInstance(v8::Handle<v8::Function>, int argc, v8::Handle<v8::Value> argv[]);
    };

    v8::Local<v8::Object> SafeAllocation::newInstance(v8::Handle<v8::Function> function)
    {
        if (function.IsEmpty())
            return v8::Local<v8::Object>();
        AllowAllocation allow;
        return function->NewInstance();
    }

    v8::Local<v8::Object> SafeAllocation::newInstance(v8::Handle<v8::ObjectTemplate> objectTemplate)
    {
        if (objectTemplate.IsEmpty())
            return v8::Local<v8::Object>();
        AllowAllocation allow;
        return objectTemplate->NewInstance();
    }

    v8::Local<v8::Object> SafeAllocation::newInstance(v8::Handle<v8::Function> function, int argc, v8::Handle<v8::Value> argv[])
    {
        if (function.IsEmpty())
            return v8::Local<v8::Object>();
        AllowAllocation allow;
        return function->NewInstance(argc, argv);
    }

} // namespace WebCore

#endif // V8Utilities_h
