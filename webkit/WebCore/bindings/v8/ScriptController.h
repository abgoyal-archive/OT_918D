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
 * Copyright (C) 2008, 2009 Google Inc. All rights reserved.
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

#ifndef ScriptController_h
#define ScriptController_h

#include "ScriptInstance.h"
#include "ScriptValue.h"

#include "V8Proxy.h"

#include <v8.h>

#include <wtf/HashMap.h>
#include <wtf/RefCounted.h>
#include <wtf/Vector.h>

struct NPObject;

namespace WebCore {

class DOMWrapperWorld;
class Event;
class Frame;
class HTMLPlugInElement;
class ScriptSourceCode;
class String;
class Widget;
class XSSAuditor;

class ScriptController {
public:
    ScriptController(Frame*);
    ~ScriptController();

    // FIXME: V8Proxy should either be folded into ScriptController
    // or this accessor should be made JSProxy*
    V8Proxy* proxy() { return m_proxy.get(); }

    ScriptValue executeScript(const ScriptSourceCode&);
    ScriptValue executeScript(const String& script, bool forceUserGesture = false);

    // Returns true if argument is a JavaScript URL.
    bool executeIfJavaScriptURL(const KURL&, bool userGesture = false, bool replaceDocument = true);

    // This function must be called from the main thread. It is safe to call it repeatedly.
    static void initializeThreading();

    // Evaluate a script file in the environment of this proxy.
    // If succeeded, 'succ' is set to true and result is returned
    // as a string.
    ScriptValue evaluate(const ScriptSourceCode&);

    void evaluateInIsolatedWorld(unsigned worldID, const Vector<ScriptSourceCode>&);

    // Executes JavaScript in an isolated world. The script gets its own global scope,
    // its own prototypes for intrinsic JavaScript objects (String, Array, and so-on),
    // and its own wrappers for all DOM nodes and DOM constructors.
    //
    // If an isolated world with the specified ID already exists, it is reused.
    // Otherwise, a new world is created.
    //
    // If the worldID is 0, a new world is always created.
    //
    // FIXME: Get rid of extensionGroup here.
    void evaluateInIsolatedWorld(unsigned worldID, const Vector<ScriptSourceCode>&, int extensionGroup);

    // Masquerade 'this' as the windowShell.
    // This is a bit of a hack, but provides reasonable compatibility
    // with what JSC does as well.
    ScriptController* windowShell(DOMWrapperWorld*) { return this; }
    ScriptController* existingWindowShell(DOMWrapperWorld*) { return this; }

    XSSAuditor* xssAuditor() { return m_XSSAuditor.get(); }

    void collectGarbage();

    // Notify V8 that the system is running low on memory.
    void lowMemoryNotification();

    // Creates a property of the global object of a frame.
    void bindToWindowObject(Frame*, const String& key, NPObject*);

    PassScriptInstance createScriptInstanceForWidget(Widget*);

    // Check if the javascript engine has been initialized.
    bool haveInterpreter() const;

    bool canExecuteScripts();

    // FIXME: void* is a compile hack.
    void attachDebugger(void*);

    // --- Static methods assume we are running VM in single thread, ---
    // --- and there is only one VM instance.                        ---

    // Returns the frame for the entered context. See comments in
    // V8Proxy::retrieveFrameForEnteredContext() for more information.
    static Frame* retrieveFrameForEnteredContext();

    // Returns the frame for the current context. See comments in
    // V8Proxy::retrieveFrameForEnteredContext() for more information.
    static Frame* retrieveFrameForCurrentContext();

    // Check whether it is safe to access a frame in another domain.
    static bool isSafeScript(Frame*);

    // Pass command-line flags to the JS engine.
    static void setFlags(const char* string, int length);

    // Protect and unprotect the JS wrapper from garbage collected.
    static void gcProtectJSWrapper(void*);
    static void gcUnprotectJSWrapper(void*);

    void finishedWithEvent(Event*);
    void setEventHandlerLineNumber(int lineNumber);

    void setProcessingTimerCallback(bool processingTimerCallback) { m_processingTimerCallback = processingTimerCallback; }
    // FIXME: Currently we don't use the parameter world at all.
    // See http://trac.webkit.org/changeset/54182
    bool processingUserGesture(DOMWrapperWorld* world = 0) const;
    bool anyPageIsProcessingUserGesture() const;

    void setPaused(bool paused) { m_paused = paused; }
    bool isPaused() const { return m_paused; }

    const String* sourceURL() const { return m_sourceURL; } // 0 if we are not evaluating any script.

    void clearWindowShell();
    void updateDocument();

    void updateSecurityOrigin();
    void clearScriptObjects();
    void updatePlatformScriptObjects();
    void cleanupScriptObjectsForPlugin(Widget*);

#if ENABLE(NETSCAPE_PLUGIN_API)
    NPObject* createScriptObjectForPluginElement(HTMLPlugInElement*);
    NPObject* windowScriptNPObject();
#endif

    // Dummy method to avoid a bunch of ifdef's in WebCore.
    void evaluateInWorld(const ScriptSourceCode&, DOMWrapperWorld*);
    static void getAllWorlds(Vector<DOMWrapperWorld*>& worlds);

private:
    Frame* m_frame;
    const String* m_sourceURL;

    bool m_inExecuteScript;

    bool m_processingTimerCallback;
    bool m_paused;

    OwnPtr<V8Proxy> m_proxy;
    typedef HashMap<Widget*, NPObject*> PluginObjectMap;

    // A mapping between Widgets and their corresponding script object.
    // This list is used so that when the plugin dies, we can immediately
    // invalidate all sub-objects which are associated with that plugin.
    // The frame keeps a NPObject reference for each item on the list.
    PluginObjectMap m_pluginObjects;
#if ENABLE(NETSCAPE_PLUGIN_API)
    NPObject* m_windowScriptNPObject;
#endif
    // The XSSAuditor associated with this ScriptController.
    OwnPtr<XSSAuditor> m_XSSAuditor;
};

} // namespace WebCore

#endif // ScriptController_h
