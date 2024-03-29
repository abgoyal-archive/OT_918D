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
 * Copyright (C) 2007, 2008 Apple Inc. All rights reserved.
 * Copyright (C) 2008 Matt Lilek <webkit@mattlilek.com>
 * Copyright (C) 2009 Google Inc. All rights reserved.
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

#include "config.h"
#include "JSInjectedScriptHost.h"

#if ENABLE(INSPECTOR)

#include "Console.h"
#if ENABLE(DATABASE)
#include "Database.h"
#include "JSDatabase.h"
#endif
#include "ExceptionCode.h"
#include "Frame.h"
#include "FrameLoader.h"
#include "InjectedScript.h"
#include "InjectedScriptHost.h"
#include "InspectorController.h"
#include "InspectorResource.h"
#include "JSDOMWindow.h"
#include "JSNode.h"
#include "JSRange.h"
#include "Node.h"
#include "Page.h"
#if ENABLE(DOM_STORAGE)
#include "SerializedScriptValue.h"
#include "Storage.h"
#include "JSStorage.h"
#endif
#include "TextIterator.h"
#include "VisiblePosition.h"
#include <parser/SourceCode.h>
#include <runtime/JSArray.h>
#include <runtime/JSLock.h>
#include <wtf/RefPtr.h>
#include <wtf/Vector.h>

#if ENABLE(JAVASCRIPT_DEBUGGER)
#include "JavaScriptCallFrame.h"
#include "JavaScriptDebugServer.h"
#include "JSJavaScriptCallFrame.h"
#endif

using namespace JSC;

namespace WebCore {

static ScriptObject createInjectedScript(const String& source, InjectedScriptHost* injectedScriptHost, ScriptState* scriptState, long id)
{
    SourceCode sourceCode = makeSource(source);
    JSLock lock(SilenceAssertionsOnly);
    JSDOMGlobalObject* globalObject = static_cast<JSDOMGlobalObject*>(scriptState->lexicalGlobalObject());
    JSValue globalThisValue = scriptState->globalThisValue();
    Completion comp = JSC::evaluate(scriptState, globalObject->globalScopeChain(), sourceCode, globalThisValue);
    if (comp.complType() != JSC::Normal && comp.complType() != JSC::ReturnValue)
        return ScriptObject();
    JSValue functionValue = comp.value();
    CallData callData;
    CallType callType = functionValue.getCallData(callData);
    if (callType == CallTypeNone)
        return ScriptObject();

    MarkedArgumentBuffer args;
    args.append(toJS(scriptState, globalObject, injectedScriptHost));
    args.append(globalThisValue);
    args.append(jsNumber(scriptState, id));
    JSValue result = JSC::call(scriptState, functionValue, callType, callData, globalThisValue, args);
    if (result.isObject())
        return ScriptObject(scriptState, result.getObject());
    return ScriptObject();
}

#if ENABLE(DATABASE)
JSValue JSInjectedScriptHost::databaseForId(ExecState* exec, const ArgList& args)
{
    if (args.size() < 1)
        return jsUndefined();

    InspectorController* ic = impl()->inspectorController();
    if (!ic)
        return jsUndefined();

    Database* database = impl()->databaseForId(args.at(0).toInt32(exec));
    if (!database)
        return jsUndefined();
    return toJS(exec, database);
}
#endif

#if ENABLE(JAVASCRIPT_DEBUGGER)

JSValue JSInjectedScriptHost::currentCallFrame(ExecState* exec, const ArgList&)
{
    JavaScriptCallFrame* callFrame = impl()->currentCallFrame();
    if (!callFrame || !callFrame->isValid())
        return jsUndefined();

    JSLock lock(SilenceAssertionsOnly);
    return toJS(exec, callFrame);
}

JSValue JSInjectedScriptHost::isActivation(ExecState*, const ArgList& args)
{
    JSObject* object = args.at(0).getObject();
    return jsBoolean(object && object->isActivationObject());
}

#endif

JSValue JSInjectedScriptHost::nodeForId(ExecState* exec, const ArgList& args)
{
    if (args.size() < 1)
        return jsUndefined();

    Node* node = impl()->nodeForId(args.at(0).toInt32(exec));
    if (!node)
        return jsUndefined();

    InspectorController* ic = impl()->inspectorController();
    if (!ic)
        return jsUndefined();

    JSLock lock(SilenceAssertionsOnly);
    return toJS(exec, node);
}

JSValue JSInjectedScriptHost::pushNodePathToFrontend(ExecState* exec, const ArgList& args)
{
    if (args.size() < 3)
        return jsUndefined();

    Node* node = toNode(args.at(0));
    if (!node)
        return jsUndefined();

    bool withChildren = args.at(1).toBoolean(exec);
    bool selectInUI = args.at(2).toBoolean(exec);
    return jsNumber(exec, impl()->pushNodePathToFrontend(node, withChildren, selectInUI));
}

#if ENABLE(DATABASE)
JSValue JSInjectedScriptHost::selectDatabase(ExecState*, const ArgList& args)
{
    if (args.size() < 1)
        return jsUndefined();

    Database* database = toDatabase(args.at(0));
    if (database)
        impl()->selectDatabase(database);
    return jsUndefined();
}
#endif

#if ENABLE(DOM_STORAGE)
JSValue JSInjectedScriptHost::selectDOMStorage(ExecState*, const ArgList& args)
{
    if (args.size() < 1)
        return jsUndefined();
    InspectorController* ic = impl()->inspectorController();
    if (!ic)
        return jsUndefined();

    Storage* storage = toStorage(args.at(0));
    if (storage)
        impl()->selectDOMStorage(storage);
    return jsUndefined();
}
#endif

JSValue JSInjectedScriptHost::reportDidDispatchOnInjectedScript(ExecState* exec, const ArgList& args)
{
    if (args.size() < 3)
        return jsUndefined();
    
    if (!args.at(0).isInt32())
        return jsUndefined();
    int callId = args.at(0).asInt32();
    
    RefPtr<SerializedScriptValue> result(SerializedScriptValue::create(exec, args.at(1)));
    
    bool isException;
    if (!args.at(2).getBoolean(isException))
        return jsUndefined();
    impl()->reportDidDispatchOnInjectedScript(callId, result.get(), isException);
    return jsUndefined();
}

InjectedScript InjectedScriptHost::injectedScriptFor(ScriptState* scriptState)
{
    JSLock lock(SilenceAssertionsOnly);
    JSDOMGlobalObject* globalObject = static_cast<JSDOMGlobalObject*>(scriptState->lexicalGlobalObject());
    JSObject* injectedScript = globalObject->injectedScript();
    if (injectedScript)
        return InjectedScript(ScriptObject(scriptState, injectedScript));

    ASSERT(!m_injectedScriptSource.isEmpty());
    ScriptObject injectedScriptObject = createInjectedScript(m_injectedScriptSource, this, scriptState, m_nextInjectedScriptId);
    globalObject->setInjectedScript(injectedScriptObject.jsObject());
    InjectedScript result(injectedScriptObject);
    m_idToInjectedScript.set(m_nextInjectedScriptId, result);
    m_nextInjectedScriptId++;
    return result;
}

} // namespace WebCore

#endif // ENABLE(INSPECTOR)
