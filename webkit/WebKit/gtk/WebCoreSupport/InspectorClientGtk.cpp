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
 * Copyright (C) 2008 Gustavo Noronha Silva
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "config.h"
#include "InspectorClientGtk.h"

#include "webkitwebview.h"
#include "webkitwebinspector.h"
#include "webkitprivate.h"
#include "CString.h"
#include "InspectorController.h"
#include "NotImplemented.h"
#include "PlatformString.h"

using namespace WebCore;

namespace WebKit {

static void notifyWebViewDestroyed(WebKitWebView* webView, InspectorClient* inspectorClient)
{
    inspectorClient->webViewDestroyed();
}

InspectorClient::InspectorClient(WebKitWebView* webView)
    : m_webView(0)
    , m_inspectedWebView(webView)
    , m_webInspector(0)
{}

void InspectorClient::inspectorDestroyed()
{
    if (m_webInspector)
        g_object_unref(m_webInspector);

    delete this;
}

void InspectorClient::webViewDestroyed()
{
    m_webView = 0;
    core(m_inspectedWebView)->inspectorController()->pageDestroyed();

    // createPage will be called again, if the user chooses to inspect
    // something else, and the inspector will be referenced again,
    // there.
    g_object_unref(m_webInspector);
    m_webInspector = 0;
}

Page* InspectorClient::createPage()
{
    if (m_webView) {
        gboolean handled = FALSE;
        g_signal_emit_by_name(m_webInspector, "destroy", &handled);

        /* we can now dispose our own reference */
        g_object_unref(m_webInspector);
    }

    // This g_object_get will ref the inspector. We're not doing an
    // unref if this method succeeds because the inspector object must
    // be alive even after the inspected WebView is destroyed - the
    // close-window and destroy signals still need to be
    // emitted.
    WebKitWebInspector* webInspector;
    g_object_get(m_inspectedWebView, "web-inspector", &webInspector, NULL);
    m_webInspector = webInspector;

    g_signal_emit_by_name(m_webInspector, "inspect-web-view", m_inspectedWebView, &m_webView);

    if (!m_webView) {
        g_object_unref(m_webInspector);
        return 0;
    }

    webkit_web_inspector_set_web_view(m_webInspector, m_webView);

    g_signal_connect(m_webView, "destroy",
                     G_CALLBACK(notifyWebViewDestroyed), (gpointer)this);

    GOwnPtr<gchar> inspectorURI;

    // Make the Web Inspector work when running tests
    if (g_file_test("WebCore/inspector/front-end/inspector.html", G_FILE_TEST_EXISTS)) {
        GOwnPtr<gchar> currentDirectory(g_get_current_dir());
        GOwnPtr<gchar> fullPath(g_strdup_printf("%s/WebCore/inspector/front-end/inspector.html", currentDirectory.get()));
        inspectorURI.set(g_filename_to_uri(fullPath.get(), NULL, NULL));
    } else
        inspectorURI.set(g_filename_to_uri(DATA_DIR"/webkit-1.0/webinspector/inspector.html", NULL, NULL));

    webkit_web_view_load_uri(m_webView, inspectorURI.get());

    gtk_widget_show(GTK_WIDGET(m_webView));

    return core(m_webView);
}

String InspectorClient::localizedStringsURL()
{
    GOwnPtr<gchar> URL;

    // Make the Web Inspector work when running tests
    if (g_file_test("WebCore/English.lproj/localizedStrings.js", G_FILE_TEST_EXISTS)) {
        GOwnPtr<gchar> currentDirectory(g_get_current_dir());
        GOwnPtr<gchar> fullPath(g_strdup_printf("%s/WebCore/English.lproj/localizedStrings.js", currentDirectory.get()));
        URL.set(g_filename_to_uri(fullPath.get(), NULL, NULL));
    } else
        URL.set(g_filename_to_uri(DATA_DIR"/webkit-1.0/webinspector/localizedStrings.js", NULL, NULL));

    // FIXME: support l10n of localizedStrings.js
    return String::fromUTF8(URL.get());
}

String InspectorClient::hiddenPanels()
{
    notImplemented();
    return String();
}

void InspectorClient::showWindow()
{
    if (!m_webView)
        return;

    gboolean handled = FALSE;
    g_signal_emit_by_name(m_webInspector, "show-window", &handled);

    core(m_inspectedWebView)->inspectorController()->setWindowVisible(true);
}

void InspectorClient::closeWindow()
{
    if (!m_webView)
        return;

    gboolean handled = FALSE;
    g_signal_emit_by_name(m_webInspector, "close-window", &handled);

    core(m_inspectedWebView)->inspectorController()->setWindowVisible(false);
}

void InspectorClient::attachWindow()
{
    if (!m_webView)
        return;

    gboolean handled = FALSE;
    g_signal_emit_by_name(m_webInspector, "attach-window", &handled);
}

void InspectorClient::detachWindow()
{
    if (!m_webView)
        return;

    gboolean handled = FALSE;
    g_signal_emit_by_name(m_webInspector, "detach-window", &handled);
}

void InspectorClient::setAttachedWindowHeight(unsigned height)
{
    notImplemented();
}

void InspectorClient::highlight(Node* node)
{
    notImplemented();
}

void InspectorClient::hideHighlight()
{
    notImplemented();
}

void InspectorClient::inspectedURLChanged(const String& newURL)
{
    if (!m_webView)
        return;

    webkit_web_inspector_set_inspected_uri(m_webInspector, newURL.utf8().data());
}

void InspectorClient::inspectorWindowObjectCleared()
{
    notImplemented();
}

void InspectorClient::populateSetting(const String& key, String* value)
{
    notImplemented();
}

void InspectorClient::storeSetting(const String& key, const String& value)
{
    notImplemented();
}

}

