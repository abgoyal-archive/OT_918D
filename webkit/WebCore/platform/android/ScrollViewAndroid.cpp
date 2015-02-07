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
 * Copyright 2007, The Android Open Source Project
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
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
 */
#define LOG_TAG "WebCore"

#include "config.h"
#include "ScrollView.h"

#include "FloatRect.h"
#include "FrameView.h"
#include "IntRect.h"
#include "SkRegion.h"
#include "WebCoreFrameBridge.h"
#include "WebCoreViewBridge.h"
#include "WebViewCore.h"

/*
    This class implementation does NOT actually emulate the Qt ScrollView.
    It does provide an implementation that khtml will use to interact with
    WebKit's WebFrameView documentView and our NSScrollView subclass.

    ScrollView's view is a NSScrollView (or subclass of NSScrollView)
    in most cases. That scrollview is a subview of an
    WebCoreFrameView. The WebCoreFrameView's documentView will also be
    the scroll view's documentView.
    
    The WebCoreFrameView's size is the frame size.  The WebCoreFrameView's documentView
    corresponds to the frame content size.  The scrollview itself is autosized to the
    WebCoreFrameView's size (see Widget::resize).
*/

namespace WebCore {

IntRect ScrollView::platformVisibleContentRect(bool includeScrollbars) const
{
    IntRect rect = platformWidget()->getBounds();
    // This makes subframes draw correctly, since subframes cannot scroll.
    if (parent())
        return IntRect(0, 0, rect.width(), rect.height());
    return rect;
}

IntSize ScrollView::platformContentsSize() const
{
    return m_contentsSize;
}

void ScrollView::platformSetScrollPosition(const WebCore::IntPoint& pt)
{
    if (parent()) // don't attempt to scroll subframes; they're fully visible
        return;
    android::WebViewCore::getWebViewCore(this)->scrollTo(pt.x(), pt.y());
}

void ScrollView::platformSetScrollbarModes()
{
    if (parent()) // no scrollbar for the subframes
        return;
    android::WebViewCore::getWebViewCore(this)->setScrollbarModes(m_horizontalScrollbarMode, m_verticalScrollbarMode);
}

void ScrollView::platformScrollbarModes(ScrollbarMode& h, ScrollbarMode& v) const
{
    // m_horizontalScrollbarMode and m_verticalScrollbarMode are set in ScrollView::setScrollbarModes()
    h = m_horizontalScrollbarMode;
    v = m_verticalScrollbarMode;
}

bool ScrollView::platformProhibitsScrolling()
{
    if (!isFrameView())
        return false;
    FrameView* view = static_cast<FrameView*>(this);
    // We want to ignore requests to scroll that were not initiated by the user.  An
    // example of this is when text is inserted into a textfield/area, which results in
    // a scroll.  We ignore this because we now how to do this ourselves in the UI thread.
    // An example of it being initiated by the user is if the user clicks an anchor
    // element which simply scrolls the page.
    return !android::WebFrame::getWebFrame(view->frame())->userInitiatedClick();
}

void ScrollView::platformRepaintContentRectangle(const IntRect &rect, bool now)
{
    android::WebViewCore::getWebViewCore(this)->contentInvalidate(rect);
}

#ifdef ANDROID_CAPTURE_OFFSCREEN_PAINTS
//  Compute the offscreen parts of the drawn rectangle by subtracting
//  vis from rect. This can compute up to four rectangular slices.
void ScrollView::platformOffscreenContentRectangle(const IntRect& vis, const IntRect& rect)
{
    SkRegion rectRgn = SkRegion(rect);
    rectRgn.op(vis, SkRegion::kDifference_Op);
    SkRegion::Iterator iter(rectRgn);
    for (; !iter.done(); iter.next()) {
        const SkIRect& diff = iter.rect();
        android::WebViewCore::getWebViewCore(this)->offInvalidate(diff);
    }
}
#endif

} // namespace WebCore