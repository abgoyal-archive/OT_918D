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
 * Copyright (C) 2007 Alp Toker <alp@atoker.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "config.h"
#include "WorkQueueItem.h"

#include "DumpRenderTree.h"

#include <JavaScriptCore/JSStringRef.h>
#include <webkit/webkit.h>
#include <string.h>

// Returns a newly allocated UTF-8 character buffer which must be freed with g_free()
gchar* JSStringCopyUTF8CString(JSStringRef jsString)
{
    size_t dataSize = JSStringGetMaximumUTF8CStringSize(jsString);
    gchar* utf8 = (gchar*)g_malloc(dataSize);
    JSStringGetUTF8CString(jsString, utf8, dataSize);

    return utf8;
}

bool LoadItem::invoke() const
{
    gchar* targetString = JSStringCopyUTF8CString(m_target.get());

    WebKitWebFrame* targetFrame;
    if (!strlen(targetString))
        targetFrame = mainFrame;
    else
        targetFrame = webkit_web_frame_find_frame(mainFrame, targetString);
    g_free(targetString);

    gchar* urlString = JSStringCopyUTF8CString(m_url.get());
    WebKitNetworkRequest* request = webkit_network_request_new(urlString);
    g_free(urlString);
    webkit_web_frame_load_request(targetFrame, request);
    g_object_unref(request);

    return true;
}

bool ReloadItem::invoke() const
{
    webkit_web_frame_reload(mainFrame);
    return true;
}

bool ScriptItem::invoke() const
{
    WebKitWebView* webView = webkit_web_frame_get_web_view(mainFrame);
    gchar* scriptString = JSStringCopyUTF8CString(m_script.get());
    webkit_web_view_execute_script(webView, scriptString);
    g_free(scriptString);
    return true;
}

bool BackForwardItem::invoke() const
{
    WebKitWebView* webView = webkit_web_frame_get_web_view(mainFrame);
    if (m_howFar == 1)
        webkit_web_view_go_forward(webView);
    else if (m_howFar == -1)
        webkit_web_view_go_back(webView);
    else {
        WebKitWebBackForwardList* webBackForwardList = webkit_web_view_get_back_forward_list(webView);
        WebKitWebHistoryItem* item = webkit_web_back_forward_list_get_nth_item(webBackForwardList, m_howFar);
        webkit_web_view_go_to_back_forward_item(webView, item);
    }
    return true;
}
