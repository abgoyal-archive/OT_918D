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

#ifndef InspectorTimelineAgent_h
#define InspectorTimelineAgent_h

#if ENABLE(INSPECTOR)

#include "Document.h"
#include "ScriptExecutionContext.h"
#include "ScriptObject.h"
#include "ScriptArray.h"
#include <wtf/Vector.h>

namespace WebCore {
    class Event;
    class InspectorFrontend;
    class IntRect;
    class ResourceRequest;
    class ResourceResponse;

    // Must be kept in sync with TimelineAgent.js
    enum TimelineRecordType {
        EventDispatchTimelineRecordType = 0,
        LayoutTimelineRecordType = 1,
        RecalculateStylesTimelineRecordType = 2,
        PaintTimelineRecordType = 3,
        ParseHTMLTimelineRecordType = 4,
        TimerInstallTimelineRecordType = 5,
        TimerRemoveTimelineRecordType = 6,
        TimerFireTimelineRecordType = 7,
        XHRReadyStateChangeRecordType = 8,
        XHRLoadRecordType = 9,
        EvaluateScriptTimelineRecordType = 10,
        MarkTimelineRecordType = 11,
        ResourceSendRequestTimelineRecordType = 12,
        ResourceReceiveResponseTimelineRecordType = 13,
        ResourceFinishTimelineRecordType = 14,
    };

    class InspectorTimelineAgent : public Noncopyable {
    public:
        InspectorTimelineAgent(InspectorFrontend* frontend);
        ~InspectorTimelineAgent();

        void reset();
        void resetFrontendProxyObject(InspectorFrontend*);

        // Methods called from WebCore.
        void willDispatchEvent(const Event&);
        void didDispatchEvent();

        void willLayout();
        void didLayout();

        void willRecalculateStyle();
        void didRecalculateStyle();

        void willPaint(const IntRect&);
        void didPaint();

        void willWriteHTML(unsigned int length, unsigned int startLine);
        void didWriteHTML(unsigned int endLine);
        
        void didInstallTimer(int timerId, int timeout, bool singleShot);
        void didRemoveTimer(int timerId);
        void willFireTimer(int timerId);
        void didFireTimer();

        void willChangeXHRReadyState(const String&, int);
        void didChangeXHRReadyState();
        void willLoadXHR(const String&);
        void didLoadXHR();

        void willEvaluateScript(const String&, int);
        void didEvaluateScript();

        void didMarkTimeline(const String&);

        void willSendResourceRequest(unsigned long, bool isMainResource, const ResourceRequest&);
        void didReceiveResourceResponse(unsigned long, const ResourceResponse&);
        void didFinishLoadingResource(unsigned long, bool didFail);

        static InspectorTimelineAgent* retrieve(ScriptExecutionContext*);
    private:
        struct TimelineRecordEntry {
            TimelineRecordEntry(ScriptObject record, ScriptObject data, ScriptArray children, TimelineRecordType type) : record(record), data(data), children(children), type(type) { }
            ScriptObject record;
            ScriptObject data;
            ScriptArray children;
            TimelineRecordType type;
        };
        
        void pushCurrentRecord(ScriptObject, TimelineRecordType);
        
        static double currentTimeInMilliseconds();

        void didCompleteCurrentRecord(TimelineRecordType);

        void addRecordToTimeline(ScriptObject, TimelineRecordType);

        InspectorFrontend* m_frontend;
        
        Vector< TimelineRecordEntry > m_recordStack;
    };

inline InspectorTimelineAgent* InspectorTimelineAgent::retrieve(ScriptExecutionContext* context)
{
    if (context && context->isDocument())
        return static_cast<Document*>(context)->inspectorTimelineAgent();
    return 0;
}

} // namespace WebCore

#endif // !ENABLE(INSPECTOR)
#endif // !defined(InspectorTimelineAgent_h)
