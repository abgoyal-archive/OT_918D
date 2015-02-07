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
 *  Copyright (C) 2001 Peter Kelly (pmk@post.com)
 *  Copyright (C) 2003, 2008, 2009 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef JSEventListener_h
#define JSEventListener_h

#include "EventListener.h"
#include "JSDOMWindow.h"
#include <runtime/WeakGCPtr.h>

namespace WebCore {

    class JSDOMGlobalObject;

    class JSEventListener : public EventListener {
    public:
        static PassRefPtr<JSEventListener> create(JSC::JSObject* listener, JSC::JSObject* wrapper, bool isAttribute, DOMWrapperWorld* isolatedWorld)
        {
            return adoptRef(new JSEventListener(listener, wrapper, isAttribute, isolatedWorld));
        }

        static const JSEventListener* cast(const EventListener* listener)
        {
            return listener->type() == JSEventListenerType
                ? static_cast<const JSEventListener*>(listener)
                : 0;
        }

        virtual ~JSEventListener();

        virtual bool operator==(const EventListener& other);

        // Returns true if this event listener was created for an event handler attribute, like "onload" or "onclick".
        bool isAttribute() const { return m_isAttribute; }

        JSC::JSObject* jsFunction(ScriptExecutionContext*) const;
        DOMWrapperWorld* isolatedWorld() const { return m_isolatedWorld.get(); }

        JSC::JSObject* wrapper() const { return m_wrapper.get(); }
        void setWrapper(JSC::JSObject* wrapper) const { m_wrapper = wrapper; }

    private:
        virtual JSC::JSObject* initializeJSFunction(ScriptExecutionContext*) const;
        virtual void markJSFunction(JSC::MarkStack&);
        virtual void invalidateJSFunction(JSC::JSObject*);
        virtual void handleEvent(ScriptExecutionContext*, Event*);
        virtual bool reportError(ScriptExecutionContext*, const String& message, const String& url, int lineNumber);
        virtual bool virtualisAttribute() const;

    protected:
        JSEventListener(JSC::JSObject* function, JSC::JSObject* wrapper, bool isAttribute, DOMWrapperWorld* isolatedWorld);

    private:
        mutable JSC::JSObject* m_jsFunction;
        mutable JSC::WeakGCPtr<JSC::JSObject> m_wrapper;

        bool m_isAttribute;
        RefPtr<DOMWrapperWorld> m_isolatedWorld;
    };

    inline JSC::JSObject* JSEventListener::jsFunction(ScriptExecutionContext* scriptExecutionContext) const
    {
        if (!m_jsFunction)
            m_jsFunction = initializeJSFunction(scriptExecutionContext);

        // Verify that we have a valid wrapper protecting our function from
        // garbage collection.
        ASSERT(m_wrapper || !m_jsFunction);
        if (!m_wrapper)
            return 0;

        // Try to verify that m_jsFunction wasn't recycled. (Not exact, since an
        // event listener can be almost anything, but this makes test-writing easier).
        ASSERT(!m_jsFunction || static_cast<JSC::JSCell*>(m_jsFunction)->isObject());

        return m_jsFunction;
    }

    inline void JSEventListener::invalidateJSFunction(JSC::JSObject* wrapper)
    {
        m_wrapper.clear(wrapper);
    }

    // Creates a JS EventListener for an "onXXX" event attribute.
    inline PassRefPtr<JSEventListener> createJSAttributeEventListener(JSC::ExecState* exec, JSC::JSValue listener, JSC::JSObject* wrapper)
    {
        if (!listener.isObject())
            return 0;

        return JSEventListener::create(asObject(listener), wrapper, true, currentWorld(exec));
    }


} // namespace WebCore

#endif // JSEventListener_h
