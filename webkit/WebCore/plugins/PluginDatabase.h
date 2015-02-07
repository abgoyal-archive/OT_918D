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
 * Copyright (C) 2006, 2007 Apple Inc.  All rights reserved.
 * Copyright (C) 2008 Collabora, Ltd.  All rights reserved.
 * Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies)
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
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
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

#ifndef PluginDatabase_H
#define PluginDatabase_H

#include "PlatformString.h"
#include "PluginPackage.h"
#include "StringHash.h"

#include <wtf/HashSet.h>
#include <wtf/Vector.h>

#if defined(ANDROID_PLUGINS)
namespace android {
    class WebSettings;
}
#endif

namespace WebCore {
    class Element;
    class Frame;
    class IntSize;
    class KURL;
    class PluginDatabaseClient;
    class PluginPackage;

    typedef HashSet<RefPtr<PluginPackage>, PluginPackageHash> PluginSet;

    class PluginDatabase : public Noncopyable {
    public:
        PluginDatabase();

        // The first call to installedPlugins creates the plugin database
        // and by default populates it with the plugins installed on the system.
        // For testing purposes, it is possible to not populate the database
        // automatically, as the plugins might affect the DRT results by
        // writing to a.o. stderr.
        static PluginDatabase* installedPlugins(bool populate = true);

        bool refresh();
        void clear();
        Vector<PluginPackage*> plugins() const;
        bool isMIMETypeRegistered(const String& mimeType);
        void addExtraPluginDirectory(const String&);

        static bool isPreferredPluginDirectory(const String& directory);
        static int preferredPluginCompare(const void*, const void*);

        PluginPackage* findPlugin(const KURL&, String& mimeType);
        PluginPackage* pluginForMIMEType(const String& mimeType);
        void setPreferredPluginForMIMEType(const String& mimeType, PluginPackage* plugin);

        void setPluginDirectories(const Vector<String>& directories)
        {
            clear();
            m_pluginDirectories = directories;
        }

        void setClient(PluginDatabaseClient* client)
        {
            m_client = client;
        }

        static Vector<String> defaultPluginDirectories();
        Vector<String> pluginDirectories() const { return m_pluginDirectories; }

        String MIMETypeForExtension(const String& extension) const;

    private:
        void getPluginPathsInDirectories(HashSet<String>&) const;
        void getDeletedPlugins(PluginSet&) const;

        // Returns whether the plugin was actually added or not (it won't be added if it's a duplicate of an existing plugin).
        bool add(PassRefPtr<PluginPackage>);
        void remove(PluginPackage*);

        Vector<String> m_pluginDirectories;
        HashSet<String> m_registeredMIMETypes;
        PluginSet m_plugins;
        HashMap<String, RefPtr<PluginPackage> > m_pluginsByPath;
        HashMap<String, time_t> m_pluginPathsWithTimes;

#if defined(ANDROID_PLUGINS)
        // Need access to setPluginDirectories() to change the default
        // path after startup.
        friend class ::android::WebSettings;
#endif
        HashMap<String, RefPtr<PluginPackage> > m_preferredPlugins;
        PluginDatabaseClient* m_client;
    };

} // namespace WebCore

#endif
