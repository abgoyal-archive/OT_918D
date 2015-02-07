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
 *  Copyright (C) 2000 Harri Porten (porten@kde.org)
 *  Copyright (c) 2000 Daniel Molkentin (molkentin@kde.org)
 *  Copyright (c) 2000 Stefan Schimanski (schimmi@kde.org)
 *  Copyright (C) 2003, 2004, 2005, 2006 Apple Computer, Inc.
 *  Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
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
#include "Navigator.h"

#include "Chrome.h"
#include "CookieJar.h"
#include "ExceptionCode.h"
#include "Frame.h"
#include "FrameLoader.h"
#include "FrameLoaderClient.h"
#include "Geolocation.h"
#include "KURL.h"
#include "Language.h"
#include "MimeTypeArray.h"
#include "Page.h"
#include "PageGroup.h"
#include "PlatformString.h"
#include "PluginArray.h"
#include "PluginData.h"
#include "ScriptController.h"
#include "Settings.h"
#include "StorageNamespace.h"

#if PLATFORM(ANDROID)
#include "ApplicationInstalledCallback.h"
#include "Connection.h"
#include "PackageNotifier.h"
#endif

namespace WebCore {

Navigator::Navigator(Frame* frame)
    : m_frame(frame)
{
}

Navigator::~Navigator()
{
    disconnectFrame();
}

void Navigator::disconnectFrame()
{
    if (m_plugins) {
        m_plugins->disconnectFrame();
        m_plugins = 0;
    }
    if (m_mimeTypes) {
        m_mimeTypes->disconnectFrame();
        m_mimeTypes = 0;
    }
    if (m_geolocation) {
        m_geolocation->disconnectFrame();
        m_geolocation = 0;
    }
    m_frame = 0;
}

// If this function returns true, we need to hide the substring "4." that would otherwise
// appear in the appVersion string. This is to avoid problems with old versions of a
// library called OpenCube QuickMenu, which as of this writing is still being used on
// sites such as nwa.com -- the library thinks Safari is Netscape 4 if we don't do this!
static bool shouldHideFourDot(Frame* frame)
{
    const String* sourceURL = frame->script()->sourceURL();
    if (!sourceURL)
        return false;
    if (!(sourceURL->endsWith("/dqm_script.js") || sourceURL->endsWith("/dqm_loader.js") || sourceURL->endsWith("/tdqm_loader.js")))
        return false;
    Settings* settings = frame->settings();
    if (!settings)
        return false;
    return settings->needsSiteSpecificQuirks();
}

String Navigator::appVersion() const
{
    if (!m_frame)
        return String();
    String appVersion = NavigatorBase::appVersion();
    if (shouldHideFourDot(m_frame))
        appVersion.replace("4.", "4_");
    return appVersion;
}

String Navigator::language() const
{
    return defaultLanguage();
}

String Navigator::userAgent() const
{
    if (!m_frame)
        return String();
        
    // If the frame is already detached, FrameLoader::userAgent may malfunction, because it calls a client method
    // that uses frame's WebView (at least, in Mac WebKit).
    if (!m_frame->page())
        return String();
        
    return m_frame->loader()->userAgent(m_frame->document()->url());
}

PluginArray* Navigator::plugins() const
{
    if (!m_plugins)
        m_plugins = PluginArray::create(m_frame);
    return m_plugins.get();
}

MimeTypeArray* Navigator::mimeTypes() const
{
    if (!m_mimeTypes)
        m_mimeTypes = MimeTypeArray::create(m_frame);
    return m_mimeTypes.get();
}

bool Navigator::cookieEnabled() const
{
    if (!m_frame)
        return false;
        
    if (m_frame->page() && !m_frame->page()->cookieEnabled())
        return false;

    return cookiesEnabled(m_frame->document());
}

bool Navigator::javaEnabled() const
{
    if (!m_frame || !m_frame->settings())
        return false;

    return m_frame->settings()->isJavaEnabled();
}

Geolocation* Navigator::geolocation() const
{
    if (!m_geolocation)
        m_geolocation = Geolocation::create(m_frame);
    return m_geolocation.get();
}

#if PLATFORM(ANDROID)
Connection* Navigator::connection() const
{
    if (!m_connection)
        m_connection = Connection::create();
    return m_connection.get();
}
#endif

#if PLATFORM(ANDROID) && ENABLE(APPLICATION_INSTALLED)

bool Navigator::isApplicationInstalled(const String& name, PassRefPtr<ApplicationInstalledCallback> callback)
{
    if (m_applicationInstalledCallback)
        return false;

    m_applicationInstalledCallback = callback;
    m_applicationNameQuery = name;

    packageNotifier().requestPackageResult();

    return true;
}

void Navigator::onPackageResult()
{
    if (m_applicationInstalledCallback) {
        m_applicationInstalledCallback->handleEvent(packageNotifier().isPackageInstalled(m_applicationNameQuery));
        m_applicationInstalledCallback = 0;
    }
}
#endif

#if ENABLE(DOM_STORAGE)
void Navigator::getStorageUpdates()
{
    if (!m_frame)
        return;

    Page* page = m_frame->page();
    if (!page)
        return;

    StorageNamespace* localStorage = page->group().localStorage();
    if (localStorage)
        localStorage->unlock();
}
#endif

static bool verifyCustomHandlerURL(const String& baseURL, const String& url, ExceptionCode& ec)
{
    // The specification requires that it is a SYNTAX_ERR if the the "%s" token is not present.
    static const char token[] = "%s";
    int index = url.find(token);
    if (-1 == index) {
        ec = SYNTAX_ERR;
        return false;
    }

    // It is also a SYNTAX_ERR if the custom handler URL, as created by removing
    // the "%s" token and prepending the base url, does not resolve.
    String newURL = url;
    newURL.remove(index, sizeof(token) / sizeof(token[0]));

    KURL base(ParsedURLString, baseURL);
    KURL kurl(base, newURL);

    if (kurl.isEmpty() || !kurl.isValid()) {
        ec = SYNTAX_ERR;
        return false;
    }

    return true;
}

static bool verifyProtocolHandlerScheme(const String& scheme, ExceptionCode& ec)
{
    // It is a SECURITY_ERR for these schemes to be handled by a custom handler.
    if (equalIgnoringCase(scheme, "http") || equalIgnoringCase(scheme, "https") || equalIgnoringCase(scheme, "file")) {
        ec = SECURITY_ERR;
        return false;
    }
    return true;
}

void Navigator::registerProtocolHandler(const String& scheme, const String& url, const String& title, ExceptionCode& ec)
{
    if (!verifyProtocolHandlerScheme(scheme, ec))
        return;

    if (!m_frame)
        return;

    Document* document = m_frame->document();
    if (!document)
        return;

    String baseURL = document->baseURL().baseAsString();

    if (!verifyCustomHandlerURL(baseURL, url, ec))
        return;

    if (Page* page = m_frame->page())
        page->chrome()->registerProtocolHandler(scheme, baseURL, url, m_frame->displayStringModifiedByEncoding(title));
}

static bool verifyProtocolHandlerMimeType(const String& type, ExceptionCode& ec)
{
    // It is a SECURITY_ERR for these mime types to be assigned to a custom
    // handler.
    if (equalIgnoringCase(type, "text/html") || equalIgnoringCase(type, "text/css") || equalIgnoringCase(type, "application/x-javascript")) {
        ec = SECURITY_ERR;
        return false;
    }
    return true;
}

void Navigator::registerContentHandler(const String& mimeType, const String& url, const String& title, ExceptionCode& ec)
{
    if (!verifyProtocolHandlerMimeType(mimeType, ec))
        return;

    if (!m_frame)
        return;

    Document* document = m_frame->document();
    if (!document)
        return;

    String baseURL = document->baseURL().baseAsString();

    if (!verifyCustomHandlerURL(baseURL, url, ec))
        return;

    if (Page* page = m_frame->page())
        page->chrome()->registerContentHandler(mimeType, baseURL, url, m_frame->displayStringModifiedByEncoding(title));
}

} // namespace WebCore
