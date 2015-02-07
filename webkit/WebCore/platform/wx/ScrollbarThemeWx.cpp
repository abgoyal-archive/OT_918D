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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"
#include "ScrollbarThemeWx.h"

#include "HostWindow.h"
#include "NotImplemented.h"
#include "PlatformMouseEvent.h"
#include "Scrollbar.h"
#include "ScrollbarClient.h"
#include "scrollbar_render.h"
#include "ScrollbarThemeComposite.h"
#include "ScrollView.h"

#include <wx/defs.h>
#include <wx/dcgraph.h>
#include <wx/settings.h>

const int cMacButtonOverlap = 4;

namespace WebCore {

ScrollbarTheme* ScrollbarTheme::nativeTheme()
{
    static ScrollbarThemeWx theme;
    return &theme;
}

ScrollbarThemeWx::~ScrollbarThemeWx()
{
}

int ScrollbarThemeWx::scrollbarThickness(ScrollbarControlSize size)
{
    int thickness = wxSystemSettings::GetMetric(wxSYS_VSCROLL_X);
    
    // fallback for when a platform doesn't define this metric
    if (thickness <= 0)
        thickness = 20;
    
    return thickness;
}

bool ScrollbarThemeWx::hasThumb(Scrollbar* scrollbar)
{
    // This method is just called as a paint-time optimization to see if
    // painting the thumb can be skipped.  We don't have to be exact here.
    return thumbLength(scrollbar) > 0;
}

int ScrollbarThemeWx::minimumThumbLength(Scrollbar* scrollbar)
{
    return 20;
}

IntSize ScrollbarThemeWx::buttonSize(Scrollbar*) 
{
#ifdef __WXMAC__
    return IntSize(20,20);
#else
    return IntSize(16,16);
#endif
}

void ScrollbarThemeWx::splitTrack(Scrollbar* scrollbar, const IntRect& unconstrainedTrackRect, IntRect& beforeThumbRect, IntRect& thumbRect, IntRect& afterThumbRect)
{
    ScrollbarThemeComposite::splitTrack(scrollbar, unconstrainedTrackRect, beforeThumbRect, thumbRect, afterThumbRect);
#ifdef __WXMAC__
    // on Mac, there are a few pixels drawn above the actual track and so adjust
    // the hit testing rects accordingly
    int trackStart = 10; 
    if (scrollbar->orientation() == HorizontalScrollbar) {
        thumbRect.setX(thumbRect.x() + trackStart);
        afterThumbRect.setX(afterThumbRect.x() - trackStart);
    } else {
        thumbRect.setY(thumbRect.y() + trackStart);
        afterThumbRect.setY(afterThumbRect.y() - trackStart);
    }
#endif
}

IntRect ScrollbarThemeWx::backButtonRect(Scrollbar* scrollbar, ScrollbarPart part, bool)
{
    // FIXME: Handling this case is needed when there are two sets of arrow buttons
    // on Mac, one at the top and one at the bottom.
    if (part == BackButtonEndPart)
        return IntRect();

    IntSize size = buttonSize(scrollbar);
    int x = scrollbar->x();
    int y = scrollbar->y();
    
#if __WXMAC__
    if (scrollbar->orientation() == HorizontalScrollbar)
        x += scrollbar->width() - (size.width() * 2) + cMacButtonOverlap;
    else
        y += scrollbar->height() - (size.height() * 2) + cMacButtonOverlap;
#endif
    
    return IntRect(x, y, size.width(), size.height());
}

IntRect ScrollbarThemeWx::forwardButtonRect(Scrollbar* scrollbar, ScrollbarPart part, bool)
{
    // FIXME: Handling this case is needed when there are two sets of arrow buttons
    // on Mac, one at the top and one at the bottom.
    if (part == ForwardButtonStartPart)
        return IntRect();

    IntSize size = buttonSize(scrollbar);
    int x, y;
    if (scrollbar->orientation() == HorizontalScrollbar) {
#ifdef __WXMAC__
        size.setWidth(size.width() + cMacButtonOverlap);
#endif
        x = scrollbar->x() + scrollbar->width() - size.width();
        y = scrollbar->y();
    } else {
        x = scrollbar->x();
#ifdef __WXMAC__
        size.setHeight(size.height() + cMacButtonOverlap);
#endif
        y = scrollbar->y() + scrollbar->height() - size.height();
    }
    return IntRect(x, y, size.width(), size.height());
}

IntRect ScrollbarThemeWx::trackRect(Scrollbar* scrollbar, bool)
{
    IntSize bs = buttonSize(scrollbar);
    int trackStart = 0;
    if (scrollbar->orientation() == HorizontalScrollbar)
        trackStart = bs.width();
    else
        trackStart = bs.height();
        
#if __WXMAC__
    trackStart = 0;
#endif
    
    int thickness = scrollbarThickness(scrollbar->controlSize());
    if (scrollbar->orientation() == HorizontalScrollbar) {
        if (scrollbar->width() < 2 * thickness)
            return IntRect();
        return IntRect(scrollbar->x() + trackStart, scrollbar->y(), scrollbar->width() - 2 * bs.width(), thickness);
    }
    if (scrollbar->height() < 2 * thickness)
        return IntRect();
    return IntRect(scrollbar->x(), scrollbar->y() + trackStart, thickness, scrollbar->height() - 2 * bs.height());
}

void ScrollbarThemeWx::paintScrollCorner(ScrollView* view, GraphicsContext* context, const IntRect& cornerRect)
{
    // ScrollbarThemeComposite::paintScrollCorner incorrectly assumes that the
    // ScrollView is a FrameView (see FramelessScrollView), so we cannot let
    // that code run.  For FrameView's this is correct since we don't do custom
    // scrollbar corner rendering, which ScrollbarThemeComposite supports.
    ScrollbarTheme::paintScrollCorner(view, context, cornerRect);
}

bool ScrollbarThemeWx::paint(Scrollbar* scrollbar, GraphicsContext* context, const IntRect& rect)
{
    wxOrientation orientation = (scrollbar->orientation() == HorizontalScrollbar) ? wxHORIZONTAL : wxVERTICAL;
    int flags = 0;
    if (scrollbar->client()->isActive())
        flags |= wxCONTROL_FOCUSED;
    
    if (!scrollbar->enabled())
        flags |= wxCONTROL_DISABLED;
    
    wxDC* dc = static_cast<wxDC*>(context->platformContext());
    
    context->save();
    ScrollView* root = scrollbar->root();
    ASSERT(root);
    if (!root)
        return false;
    
    wxWindow* webview = root->hostWindow()->platformPageClient(); 
    
    wxRenderer_DrawScrollbar(webview, *dc, scrollbar->frameRect(), orientation, scrollbar->currentPos(), static_cast<wxScrollbarPart>(scrollbar->pressedPart()),    
                     static_cast<wxScrollbarPart>(scrollbar->hoveredPart()), scrollbar->maximum(), scrollbar->pageStep(), flags);

    context->restore();
    return true;
}

}
