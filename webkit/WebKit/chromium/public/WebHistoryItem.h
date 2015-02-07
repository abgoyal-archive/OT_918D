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

#ifndef WebHistoryItem_h
#define WebHistoryItem_h

#include "WebCommon.h"

#if WEBKIT_IMPLEMENTATION
namespace WebCore { class HistoryItem; }
namespace WTF { template <typename T> class PassRefPtr; }
#endif

namespace WebKit {

class WebHistoryItemPrivate;
class WebHTTPBody;
class WebString;
struct WebPoint;
template <typename T> class WebVector;

// Represents a frame-level navigation entry in session history.  A
// WebHistoryItem is a node in a tree.
//
// Copying a WebHistoryItem is cheap.
//
class WebHistoryItem {
public:
    ~WebHistoryItem() { reset(); }

    WebHistoryItem() : m_private(0) { }
    WebHistoryItem(const WebHistoryItem& h) : m_private(0) { assign(h); }
    WebHistoryItem& operator=(const WebHistoryItem& h)
    {
        assign(h);
        return *this;
    }

    WEBKIT_API void initialize();
    WEBKIT_API void reset();
    WEBKIT_API void assign(const WebHistoryItem&);

    bool isNull() const { return !m_private; }

    WEBKIT_API WebString urlString() const;
    WEBKIT_API void setURLString(const WebString&);

    WEBKIT_API WebString originalURLString() const;
    WEBKIT_API void setOriginalURLString(const WebString&);

    WEBKIT_API WebString referrer() const;
    WEBKIT_API void setReferrer(const WebString&);

    WEBKIT_API WebString target() const;
    WEBKIT_API void setTarget(const WebString&);

    WEBKIT_API WebString parent() const;
    WEBKIT_API void setParent(const WebString&);

    WEBKIT_API WebString title() const;
    WEBKIT_API void setTitle(const WebString&);

    WEBKIT_API WebString alternateTitle() const;
    WEBKIT_API void setAlternateTitle(const WebString&);

    WEBKIT_API double lastVisitedTime() const;
    WEBKIT_API void setLastVisitedTime(double);

    WEBKIT_API WebPoint scrollOffset() const;
    WEBKIT_API void setScrollOffset(const WebPoint&);

    WEBKIT_API bool isTargetItem() const;
    WEBKIT_API void setIsTargetItem(bool);

    WEBKIT_API int visitCount() const;
    WEBKIT_API void setVisitCount(int);

    WEBKIT_API WebVector<WebString> documentState() const;
    WEBKIT_API void setDocumentState(const WebVector<WebString>&);

    WEBKIT_API long long documentSequenceNumber() const;
    WEBKIT_API void setDocumentSequenceNumber(long long);

    WEBKIT_API WebString httpContentType() const;
    WEBKIT_API void setHTTPContentType(const WebString&);

    WEBKIT_API WebHTTPBody httpBody() const;
    WEBKIT_API void setHTTPBody(const WebHTTPBody&);

    WEBKIT_API WebVector<WebHistoryItem> children() const;
    WEBKIT_API void setChildren(const WebVector<WebHistoryItem>&);
    WEBKIT_API void appendToChildren(const WebHistoryItem&);

#if WEBKIT_IMPLEMENTATION
    WebHistoryItem(const WTF::PassRefPtr<WebCore::HistoryItem>&);
    WebHistoryItem& operator=(const WTF::PassRefPtr<WebCore::HistoryItem>&);
    operator WTF::PassRefPtr<WebCore::HistoryItem>() const;
#endif

private:
    void assign(WebHistoryItemPrivate*);
    void ensureMutable();
    WebHistoryItemPrivate* m_private;
};

} // namespace WebKit

#endif
