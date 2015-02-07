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

#ifndef WebNotification_h
#define WebNotification_h

#include "WebCommon.h"

#if WEBKIT_IMPLEMENTATION
namespace WebCore { class Notification; }
namespace WTF { template <typename T> class PassRefPtr; }
#endif

namespace WebKit {

class WebNotificationPrivate;
class WebURL;
class WebString;

// Represents access to a desktop notification.
class WebNotification {
public:
    WebNotification() : m_private(0) { }
    WebNotification(const WebNotification& other) : m_private(0) { assign(other); }

    ~WebNotification() { reset(); }

    WEBKIT_API void reset();
    WEBKIT_API void assign(const WebNotification&);

    WebNotification& operator=(const WebNotification& other)
    {
        assign(other);
        return *this;
    }

    // Operators required to put WebNotification in an ordered map.
    bool equals(const WebNotification& other) const { return m_private == other.m_private; }
    bool lessThan(const WebNotification& other) const;

    // Is the notification HTML vs. icon-title-text?
    WEBKIT_API bool isHTML() const;

    // If HTML, the URL which contains the contents of the notification.
    WEBKIT_API WebURL url() const;

    // If not HTML, the parameters for the icon-title-text notification.
    WEBKIT_API WebString icon() const;
    WEBKIT_API WebString title() const;
    WEBKIT_API WebString body() const;

    // Called to indicate the notification has been displayed.
    WEBKIT_API void dispatchDisplayEvent();

    // Called to indicate an error has occurred with this notification.
    WEBKIT_API void dispatchErrorEvent(const WebString& errorMessage);

    // Called to indicate the notification has been closed.  If it was
    // closed by the user (as opposed to automatically by the system),
    // the byUser parameter will be true.
    WEBKIT_API void dispatchCloseEvent(bool byUser);

#if WEBKIT_IMPLEMENTATION
    WebNotification(const WTF::PassRefPtr<WebCore::Notification>&);
    WebNotification& operator=(const WTF::PassRefPtr<WebCore::Notification>&);
    operator WTF::PassRefPtr<WebCore::Notification>() const;
#endif

private:
    void assign(WebNotificationPrivate*);
    WebNotificationPrivate* m_private;
};

inline bool operator==(const WebNotification& a, const WebNotification& b)
{
    return a.equals(b);
}

inline bool operator!=(const WebNotification& a, const WebNotification& b)
{
    return !a.equals(b);
}

inline bool operator<(const WebNotification& a, const WebNotification& b)
{
    return a.lessThan(b);
}

} // namespace WebKit

#endif
