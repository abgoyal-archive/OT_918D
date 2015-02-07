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
 * Copyright (c) 2008, Google Inc. All rights reserved.
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
#include "ScriptCallStack.h"

#include <interpreter/CallFrame.h>
#include <interpreter/Interpreter.h>
#include <runtime/InternalFunction.h>
#include <runtime/JSValue.h>
#include <runtime/UString.h>
#include <runtime/JSGlobalData.h>

using namespace JSC;

namespace WebCore {

ScriptCallStack::ScriptCallStack(ExecState* exec, const ArgList& args, unsigned skipArgumentCount)
    : m_initialized(false)
    , m_exec(exec)
    , m_caller(0)
{
    int signedLineNumber;
    intptr_t sourceID;
    UString urlString;
    JSValue function;

    exec->interpreter()->retrieveLastCaller(exec, signedLineNumber, sourceID, urlString, function);

    unsigned lineNumber = signedLineNumber >= 0 ? signedLineNumber : 0;

    if (function) {
        m_caller = asInternalFunction(function);
        m_frames.append(ScriptCallFrame(m_caller->name(m_exec), urlString, lineNumber, args, skipArgumentCount));
    } else {
        // Caller is unknown, but we should still add the frame, because
        // something called us, and gave us arguments.
        m_frames.append(ScriptCallFrame(UString(), urlString, lineNumber, args, skipArgumentCount));
    }
}

ScriptCallStack::~ScriptCallStack()
{
}

const ScriptCallFrame &ScriptCallStack::at(unsigned index)
{
    // First frame is pre-populated in constructor, so don't trigger
    // initialization unless looking beyond the first frame.
    if (index > 0)
        initialize();
    ASSERT(m_frames.size() > index);
    return m_frames[index];
}

unsigned ScriptCallStack::size()
{
    initialize();
    return m_frames.size();
}

void ScriptCallStack::initialize()
{
    if (!m_caller || m_initialized)
        return;

    JSValue func = m_exec->interpreter()->retrieveCaller(m_exec, m_caller);
    while (!func.isNull()) {
        InternalFunction* internalFunction = asInternalFunction(func);
        ArgList emptyArgList;
        m_frames.append(ScriptCallFrame(internalFunction->name(m_exec), UString(), 0, emptyArgList, 0));
        func = m_exec->interpreter()->retrieveCaller(m_exec, internalFunction);
    }
    m_initialized = true;
}

} // namespace WebCore
