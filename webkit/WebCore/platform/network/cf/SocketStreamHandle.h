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
 * Copyright (C) 2009 Apple Inc. All rights reserved.
 * Copyright (C) 2009 Google Inc.  All rights reserved.
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

#ifndef SocketStreamHandle_h
#define SocketStreamHandle_h

#include "AuthenticationClient.h"
#include "SocketStreamHandleBase.h"
#include <wtf/RetainPtr.h>

namespace WebCore {

    class AuthenticationChallenge;
    class Credential;
    class SocketStreamHandleClient;

    class SocketStreamHandle : public RefCounted<SocketStreamHandle>, public SocketStreamHandleBase, public AuthenticationClient {
    public:
        static PassRefPtr<SocketStreamHandle> create(const KURL& url, SocketStreamHandleClient* client) { return adoptRef(new SocketStreamHandle(url, client)); }

        virtual ~SocketStreamHandle();

        using RefCounted<SocketStreamHandle>::ref;
        using RefCounted<SocketStreamHandle>::deref;

    private:
        virtual int platformSend(const char* data, int length);
        virtual void platformClose();

        SocketStreamHandle(const KURL&, SocketStreamHandleClient*);
        void createStreams();
        void scheduleStreams();
        void chooseProxy();
#ifndef BUILDING_ON_TIGER
        void chooseProxyFromArray(CFArrayRef);
        void executePACFileURL(CFURLRef);
        void removePACRunLoopSource();
        RetainPtr<CFRunLoopSourceRef> m_pacRunLoopSource;
        static void pacExecutionCallback(void* client, CFArrayRef proxyList, CFErrorRef error);
        static void pacExecutionCallbackMainThread(void*);
        static CFStringRef copyPACExecutionDescription(void*);
#endif

        bool shouldUseSSL() const { return m_url.protocolIs("wss"); }

        void addCONNECTCredentials(CFHTTPMessageRef response);

        static CFStringRef copyCFStreamDescription(void* );
        static void readStreamCallback(CFReadStreamRef, CFStreamEventType, void*);
        static void writeStreamCallback(CFWriteStreamRef, CFStreamEventType, void*);
#if PLATFORM(WIN)
        static void readStreamCallbackMainThread(void*);
        static void writeStreamCallbackMainThread(void*);
#endif
        void readStreamCallback(CFStreamEventType);
        void writeStreamCallback(CFStreamEventType);

        // No authentication for streams per se, but proxy may ask for credentials.
        virtual void receivedCredential(const AuthenticationChallenge&, const Credential&);
        virtual void receivedRequestToContinueWithoutCredential(const AuthenticationChallenge&);
        virtual void receivedCancellation(const AuthenticationChallenge&);

        virtual void refAuthenticationClient() { ref(); }
        virtual void derefAuthenticationClient() { deref(); }

        enum ConnectingSubstate { New, ExecutingPACFile, WaitingForCredentials, WaitingForConnect, Connected };
        ConnectingSubstate m_connectingSubstate;

        enum ConnectionType { Unknown, Direct, SOCKSProxy, CONNECTProxy };
        ConnectionType m_connectionType;
        RetainPtr<CFStringRef> m_proxyHost;
        RetainPtr<CFNumberRef> m_proxyPort;

        RetainPtr<CFHTTPMessageRef> m_proxyResponseMessage;
        bool m_sentStoredCredentials;
        RetainPtr<CFReadStreamRef> m_readStream;
        RetainPtr<CFWriteStreamRef> m_writeStream;

        RetainPtr<CFURLRef> m_httpsURL; // ws(s): replaced with https:
    };

}  // namespace WebCore

#endif  // SocketStreamHandle_h
