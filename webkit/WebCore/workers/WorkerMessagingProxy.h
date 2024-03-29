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
 * Copyright (C) 2008 Apple Inc. All Rights Reserved.
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
 *
 */

#ifndef WorkerMessagingProxy_h
#define WorkerMessagingProxy_h

#if ENABLE(WORKERS)

#include "ScriptExecutionContext.h"
#include "WorkerContextProxy.h"
#include "WorkerLoaderProxy.h"
#include "WorkerObjectProxy.h"
#include <wtf/Noncopyable.h>
#include <wtf/PassOwnPtr.h>
#include <wtf/PassRefPtr.h>
#include <wtf/RefPtr.h>
#include <wtf/Vector.h>

namespace WebCore {

    class DedicatedWorkerThread;
    class ScriptExecutionContext;
    class String;
    class Worker;

    class WorkerMessagingProxy : public WorkerContextProxy, public WorkerObjectProxy, public WorkerLoaderProxy, public Noncopyable {
    public:
        WorkerMessagingProxy(Worker*);

        // Implementations of WorkerContextProxy.
        // (Only use these methods in the worker object thread.)
        virtual void startWorkerContext(const KURL& scriptURL, const String& userAgent, const String& sourceCode);
        virtual void terminateWorkerContext();
        virtual void postMessageToWorkerContext(PassRefPtr<SerializedScriptValue>, PassOwnPtr<MessagePortChannelArray>);
        virtual bool hasPendingActivity() const;
        virtual void workerObjectDestroyed();

        // Implementations of WorkerObjectProxy.
        // (Only use these methods in the worker context thread.)
        virtual void postMessageToWorkerObject(PassRefPtr<SerializedScriptValue>, PassOwnPtr<MessagePortChannelArray>);
        virtual void postExceptionToWorkerObject(const String& errorMessage, int lineNumber, const String& sourceURL);
        virtual void postConsoleMessageToWorkerObject(MessageDestination, MessageSource, MessageType, MessageLevel, const String& message, int lineNumber, const String& sourceURL);
        virtual void confirmMessageFromWorkerObject(bool hasPendingActivity);
        virtual void reportPendingActivity(bool hasPendingActivity);
        virtual void workerContextClosed();
        virtual void workerContextDestroyed();

        // Implementation of WorkerLoaderProxy.
        // These methods are called on different threads to schedule loading
        // requests and to send callbacks back to WorkerContext.
        virtual void postTaskToLoader(PassOwnPtr<ScriptExecutionContext::Task>);
        virtual void postTaskForModeToWorkerContext(PassOwnPtr<ScriptExecutionContext::Task>, const String& mode);

        void workerThreadCreated(PassRefPtr<DedicatedWorkerThread>);

        // Only use this method on the worker object thread.
        bool askedToTerminate() const { return m_askedToTerminate; }

    private:
        friend class MessageWorkerTask;
        friend class WorkerContextDestroyedTask;
        friend class WorkerExceptionTask;
        friend class WorkerThreadActivityReportTask;

        virtual ~WorkerMessagingProxy();

        void workerContextDestroyedInternal();
        void reportPendingActivityInternal(bool confirmingMessage, bool hasPendingActivity);
        Worker* workerObject() const { return m_workerObject; }

        RefPtr<ScriptExecutionContext> m_scriptExecutionContext;
        Worker* m_workerObject;
        RefPtr<DedicatedWorkerThread> m_workerThread;

        unsigned m_unconfirmedMessageCount; // Unconfirmed messages from worker object to worker thread.
        bool m_workerThreadHadPendingActivity; // The latest confirmation from worker thread reported that it was still active.

        bool m_askedToTerminate;

        Vector<OwnPtr<ScriptExecutionContext::Task> > m_queuedEarlyTasks; // Tasks are queued here until there's a thread object created.
    };

} // namespace WebCore

#endif // ENABLE(WORKERS)

#endif // WorkerMessagingProxy_h
