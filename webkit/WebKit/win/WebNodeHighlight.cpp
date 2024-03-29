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
 * Copyright (C) 2007 Apple Inc.  All rights reserved.
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

#include "config.h"
#include "WebNodeHighlight.h"

#include "WebView.h"
#pragma warning(push, 0)
#include <WebCore/BitmapInfo.h>
#include <WebCore/Color.h>
#include <WebCore/GraphicsContext.h>
#include <WebCore/InspectorController.h>
#include <WebCore/Page.h>
#include <WebCore/WindowMessageBroadcaster.h>
#pragma warning(pop)
#include <wtf/OwnPtr.h>
#include <wtf/HashSet.h>

using namespace WebCore;

static LPCTSTR kOverlayWindowClassName = TEXT("WebNodeHighlightWindowClass");
static ATOM registerOverlayClass();
static LPCTSTR kWebNodeHighlightPointerProp = TEXT("WebNodeHighlightPointer");

WebNodeHighlight::WebNodeHighlight(WebView* webView)
    : m_inspectedWebView(webView)
    , m_overlay(0)
    , m_observedWindow(0)
    , m_showsWhileWebViewIsVisible(false)
{
    m_inspectedWebView->viewWindow(reinterpret_cast<OLE_HANDLE*>(&m_inspectedWebViewWindow));
}

WebNodeHighlight::~WebNodeHighlight()
{
    if (m_observedWindow)
        WindowMessageBroadcaster::removeListener(m_observedWindow, this);
    if (m_inspectedWebViewWindow)
        WindowMessageBroadcaster::removeListener(m_inspectedWebViewWindow, this);

    if (m_overlay)
        ::DestroyWindow(m_overlay);
}

void WebNodeHighlight::setShowsWhileWebViewIsVisible(bool shows)
{
    if (m_showsWhileWebViewIsVisible == shows)
        return;
    m_showsWhileWebViewIsVisible = shows;

    if (!m_showsWhileWebViewIsVisible) {
        hide();
        return;
    }

    bool webViewVisible = isWebViewVisible();

    if (isShowing() == webViewVisible)
        return;

    if (webViewVisible)
        show();
    else
        hide();
}

bool WebNodeHighlight::isWebViewVisible() const
{
    if (!m_inspectedWebViewWindow)
        return false;

    return IsWindowVisible(m_inspectedWebViewWindow);
}

void WebNodeHighlight::show()
{
    if (!m_overlay) {
        registerOverlayClass();

        m_overlay = ::CreateWindowEx(WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT, kOverlayWindowClassName, 0, WS_POPUP,
                                     0, 0, 0, 0,
                                     m_inspectedWebViewWindow, 0, 0, 0);
        if (!m_overlay)
            return;

        ::SetProp(m_overlay, kWebNodeHighlightPointerProp, reinterpret_cast<HANDLE>(this));

        m_observedWindow = GetAncestor(m_inspectedWebViewWindow, GA_ROOT);
        WindowMessageBroadcaster::addListener(m_observedWindow, this);
        WindowMessageBroadcaster::addListener(m_inspectedWebViewWindow, this);
    }

    ASSERT(m_showsWhileWebViewIsVisible);

    update();
    SetWindowPos(m_overlay, 0, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void WebNodeHighlight::hide()
{
    if (m_overlay)
        ::ShowWindow(m_overlay, SW_HIDE);
}

bool WebNodeHighlight::isShowing() const
{
    return m_overlay && ::IsWindowVisible(m_overlay);
}

void WebNodeHighlight::update()
{
    ASSERT(m_overlay);

    HDC hdc = ::CreateCompatibleDC(::GetDC(m_overlay));
    if (!hdc)
        return;

    RECT webViewRect;
    ::GetWindowRect(m_inspectedWebViewWindow, &webViewRect);

    SIZE size;
    size.cx = webViewRect.right - webViewRect.left;
    size.cy = webViewRect.bottom - webViewRect.top;

    BitmapInfo bitmapInfo = BitmapInfo::createBottomUp(IntSize(size));

    void* pixels = 0;
    OwnPtr<HBITMAP> hbmp(::CreateDIBSection(hdc, &bitmapInfo, DIB_RGB_COLORS, &pixels, 0, 0));

    ::SelectObject(hdc, hbmp.get());

    GraphicsContext context(hdc);

    m_inspectedWebView->page()->inspectorController()->drawNodeHighlight(context);

    BLENDFUNCTION bf;
    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.SourceConstantAlpha = 255;
    bf.AlphaFormat = AC_SRC_ALPHA;

    POINT srcPoint;
    srcPoint.x = 0;
    srcPoint.y = 0;

    POINT dstPoint;
    dstPoint.x = webViewRect.left;
    dstPoint.y = webViewRect.top;

    ::UpdateLayeredWindow(m_overlay, ::GetDC(0), &dstPoint, &size, hdc, &srcPoint, 0, &bf, ULW_ALPHA);

    ::DeleteDC(hdc);
}

void WebNodeHighlight::placeBehindWindow(HWND window)
{
    ASSERT(m_overlay);
    SetWindowPos(m_overlay, window, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

static ATOM registerOverlayClass()
{
    static bool haveRegisteredWindowClass = false;

    if (haveRegisteredWindowClass)
        return true;

    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = 0;
    wcex.lpfnWndProc    = OverlayWndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = 0;
    wcex.hIcon          = 0;
    wcex.hCursor        = LoadCursor(0, IDC_ARROW);
    wcex.hbrBackground  = 0;
    wcex.lpszMenuName   = 0;
    wcex.lpszClassName  = kOverlayWindowClassName;
    wcex.hIconSm        = 0;

    haveRegisteredWindowClass = true;

    return ::RegisterClassEx(&wcex);
}

LRESULT CALLBACK OverlayWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    WebNodeHighlight* highlight = reinterpret_cast<WebNodeHighlight*>(::GetProp(hwnd, kWebNodeHighlightPointerProp));
    if (!highlight)
        return ::DefWindowProc(hwnd, msg, wParam, lParam);

    return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

void WebNodeHighlight::onWebViewShowWindow(bool showing)
{
    if (!m_showsWhileWebViewIsVisible)
        return;

    if (isShowing() == showing)
        return;

    if (showing)
        show();
    else
        hide();
}

void WebNodeHighlight::onWebViewWindowPosChanged(WINDOWPOS* windowPos)
{
    bool sizing = !(windowPos->flags & SWP_NOSIZE);

    if (!sizing)
        return;

    if (!isShowing())
        return;

    update();
}

void WebNodeHighlight::onRootWindowPosChanged(WINDOWPOS* windowPos)
{
    bool moving = !(windowPos->flags & SWP_NOMOVE);
    bool sizing = !(windowPos->flags & SWP_NOSIZE);

    if (!moving)
        return;

    // Size changes are handled by onWebViewWindowPosChanged.
    if (sizing)
        return;

    if (!isShowing())
        return;

    update();
}

void WebNodeHighlight::windowReceivedMessage(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (window == m_inspectedWebViewWindow) {
        switch (msg) {
            case WM_SHOWWINDOW:
                onWebViewShowWindow(wParam);
                break;
            case WM_WINDOWPOSCHANGED:
                onWebViewWindowPosChanged(reinterpret_cast<WINDOWPOS*>(lParam));
                break;
            default:
                break;
        }

        return;
    }

    ASSERT(window == m_observedWindow);
    switch (msg) {
        case WM_WINDOWPOSCHANGED:
            onRootWindowPosChanged(reinterpret_cast<WINDOWPOS*>(lParam));
            break;
        default:
            break;
    }
}
