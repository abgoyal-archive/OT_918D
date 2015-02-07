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
 * Copyright (C) 2005, 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer. 
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution. 
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// This header contains WebView declarations that can be used anywhere in WebKit, but are neither SPI nor API.

#import "WebTypesInternal.h"

@class WebView;

struct WebResourceDelegateImplementationCache {
    IMP didCancelAuthenticationChallengeFunc;
    IMP didReceiveAuthenticationChallengeFunc;
    IMP identifierForRequestFunc;
    IMP willSendRequestFunc;
    IMP didReceiveResponseFunc;
    IMP didReceiveContentLengthFunc;
    IMP didFinishLoadingFromDataSourceFunc;
    IMP didFailLoadingWithErrorFromDataSourceFunc;
    IMP didLoadResourceFromMemoryCacheFunc;
    IMP willCacheResponseFunc;
    IMP plugInFailedWithErrorFunc;
    IMP shouldUseCredentialStorageFunc;
};

struct WebFrameLoadDelegateImplementationCache {
    IMP didClearWindowObjectForFrameFunc;
    IMP didClearWindowObjectForFrameInScriptWorldFunc;
    IMP didClearInspectorWindowObjectForFrameFunc;
    IMP windowScriptObjectAvailableFunc;
    IMP didHandleOnloadEventsForFrameFunc;
    IMP didReceiveServerRedirectForProvisionalLoadForFrameFunc;
    IMP didCancelClientRedirectForFrameFunc;
    IMP willPerformClientRedirectToURLDelayFireDateForFrameFunc;
    IMP didChangeLocationWithinPageForFrameFunc;
    IMP didPushStateWithinPageForFrameFunc;
    IMP didReplaceStateWithinPageForFrameFunc;
    IMP didPopStateWithinPageForFrameFunc;
    IMP willCloseFrameFunc;
    IMP didStartProvisionalLoadForFrameFunc;
    IMP didReceiveTitleForFrameFunc;
    IMP didCommitLoadForFrameFunc;
    IMP didFailProvisionalLoadWithErrorForFrameFunc;
    IMP didFailLoadWithErrorForFrameFunc;
    IMP didFinishLoadForFrameFunc;
    IMP didFirstLayoutInFrameFunc;
    IMP didFirstVisuallyNonEmptyLayoutInFrameFunc;
    IMP didReceiveIconForFrameFunc;
    IMP didFinishDocumentLoadForFrameFunc;
    IMP didDisplayInsecureContentFunc;
    IMP didRunInsecureContentFunc;
};

struct WebScriptDebugDelegateImplementationCache {
    BOOL didParseSourceExpectsBaseLineNumber;
    IMP didParseSourceFunc;
    IMP failedToParseSourceFunc;
    IMP didEnterCallFrameFunc;
    IMP willExecuteStatementFunc;
    IMP willLeaveCallFrameFunc;
    IMP exceptionWasRaisedFunc;
};

struct WebHistoryDelegateImplementationCache {
    IMP navigatedFunc;
    IMP clientRedirectFunc;
    IMP serverRedirectFunc;
    IMP setTitleFunc;
    IMP populateVisitedLinksFunc;
};

WebResourceDelegateImplementationCache* WebViewGetResourceLoadDelegateImplementations(WebView *);
WebFrameLoadDelegateImplementationCache* WebViewGetFrameLoadDelegateImplementations(WebView *);
WebScriptDebugDelegateImplementationCache* WebViewGetScriptDebugDelegateImplementations(WebView *);
WebHistoryDelegateImplementationCache* WebViewGetHistoryDelegateImplementations(WebView *webView);

id CallFormDelegate(WebView *, SEL, id, id);
id CallFormDelegate(WebView *self, SEL selector, id object1, id object2, id object3, id object4, id object5);
BOOL CallFormDelegateReturningBoolean(BOOL, WebView *, SEL, id, SEL, id);

id CallUIDelegate(WebView *, SEL);
id CallUIDelegate(WebView *, SEL, id);
id CallUIDelegate(WebView *, SEL, NSRect);
id CallUIDelegate(WebView *, SEL, id, id);
id CallUIDelegate(WebView *, SEL, id, BOOL);
id CallUIDelegate(WebView *, SEL, id, id, id);
id CallUIDelegate(WebView *, SEL, id, NSUInteger);
float CallUIDelegateReturningFloat(WebView *, SEL);
BOOL CallUIDelegateReturningBoolean(BOOL, WebView *, SEL);
BOOL CallUIDelegateReturningBoolean(BOOL, WebView *, SEL, id);
BOOL CallUIDelegateReturningBoolean(BOOL, WebView *, SEL, id, id);
BOOL CallUIDelegateReturningBoolean(BOOL, WebView *, SEL, id, BOOL);
BOOL CallUIDelegateReturningBoolean(BOOL, WebView *, SEL, id, BOOL, id);

id CallFrameLoadDelegate(IMP, WebView *, SEL);
id CallFrameLoadDelegate(IMP, WebView *, SEL, id);
id CallFrameLoadDelegate(IMP, WebView *, SEL, id, id);
id CallFrameLoadDelegate(IMP, WebView *, SEL, id, id, id);
id CallFrameLoadDelegate(IMP, WebView *, SEL, id, id, id, id);
id CallFrameLoadDelegate(IMP, WebView *, SEL, id, NSTimeInterval, id, id);

id CallResourceLoadDelegate(IMP, WebView *, SEL, id, id);
id CallResourceLoadDelegate(IMP, WebView *, SEL, id, id, id);
id CallResourceLoadDelegate(IMP, WebView *, SEL, id, id, id, id);
id CallResourceLoadDelegate(IMP, WebView *, SEL, id, NSInteger, id);
id CallResourceLoadDelegate(IMP, WebView *, SEL, id, id, NSInteger, id);

BOOL CallResourceLoadDelegateReturningBoolean(BOOL, IMP, WebView *, SEL, id, id);

id CallScriptDebugDelegate(IMP, WebView *, SEL, id, id, NSInteger, id);
id CallScriptDebugDelegate(IMP, WebView *, SEL, id, NSInteger, id, NSInteger, id);
id CallScriptDebugDelegate(IMP, WebView *, SEL, id, NSInteger, id, id, id);
id CallScriptDebugDelegate(IMP, WebView *, SEL, id, NSInteger, NSInteger, id);

id CallHistoryDelegate(IMP, WebView *, SEL);
id CallHistoryDelegate(IMP, WebView *, SEL, id, id);
id CallHistoryDelegate(IMP, WebView *, SEL, id, id, id);
