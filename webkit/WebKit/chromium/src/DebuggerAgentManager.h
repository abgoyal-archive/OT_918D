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
 * Copyright (C) 2010 Google Inc. All rights reserved.
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

#ifndef DebuggerAgentManager_h
#define DebuggerAgentManager_h

#include "WebDevToolsAgent.h"
#include <v8-debug.h>
#include <wtf/HashMap.h>
#include <wtf/Noncopyable.h>

namespace WebCore {
class PageGroupLoadDeferrer;
class String;
}

namespace WebKit {

class DebuggerAgentImpl;
class DictionaryValue;
class WebFrameImpl;
class WebViewImpl;

// There is single v8 instance per render process. Also there may be several
// RenderViews and consequently devtools agents in the process that want to talk
// to the v8 debugger. This class coordinates communication between the debug
// agents and v8 debugger. It will set debug output handler as long as at least
// one debugger agent is attached and remove it when last debugger agent is
// detached. When message is received from debugger it will route it to the
// right debugger agent if there is one otherwise the message will be ignored.
//
// v8 may send a message(e.g. exception event) after which it
// would expect some actions from the handler. If there is no appropriate
// debugger agent to handle such messages the manager will perform the action
// itself, otherwise v8 may hang waiting for the action.
class DebuggerAgentManager : public Noncopyable {
public:
    static void debugAttach(DebuggerAgentImpl* debuggerAgent);
    static void debugDetach(DebuggerAgentImpl* debuggerAgent);
    static void pauseScript();
    static void executeDebuggerCommand(const WebCore::String& command, int callerId);
    static void setMessageLoopDispatchHandler(WebDevToolsAgent::MessageLoopDispatchHandler handler);

    // Sets |hostId| as the frame context data. This id is used to filter scripts
    // related to the inspected page.
    static void setHostId(WebFrameImpl* webframe, int hostId);

    static void onWebViewClosed(WebViewImpl* webview);

    static void onNavigate();

    class UtilityContextScope : public Noncopyable {
    public:
        UtilityContextScope()
        {
            ASSERT(!s_inUtilityContext);
            s_inUtilityContext = true;
        }
        ~UtilityContextScope()
        {
            if (s_debugBreakDelayed) {
              v8::Debug::DebugBreak();
              s_debugBreakDelayed = false;
            }
            s_inUtilityContext = false;
        }
    };

private:
    DebuggerAgentManager();
    ~DebuggerAgentManager();

    static void debugHostDispatchHandler();
    static void onV8DebugMessage(const v8::Debug::Message& message);
    static void sendCommandToV8(const WebCore::String& cmd,
                                v8::Debug::ClientData* data);
    static void sendContinueCommandToV8();

    static DebuggerAgentImpl* findAgentForCurrentV8Context();
    static DebuggerAgentImpl* debuggerAgentForHostId(int hostId);

    typedef HashMap<int, DebuggerAgentImpl*> AttachedAgentsMap;
    static AttachedAgentsMap* s_attachedAgentsMap;

    static WebDevToolsAgent::MessageLoopDispatchHandler s_messageLoopDispatchHandler;
    static bool s_inHostDispatchHandler;
    typedef HashMap<WebViewImpl*, WebCore::PageGroupLoadDeferrer*> DeferrersMap;
    static DeferrersMap s_pageDeferrers;

    static bool s_inUtilityContext;
    static bool s_debugBreakDelayed;
};

} // namespace WebKit

#endif
