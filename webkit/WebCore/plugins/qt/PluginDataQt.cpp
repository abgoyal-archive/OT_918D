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
    Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
    Copyright (C) 2008 Collabora Ltd. All rights reserved.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"
#include "PluginData.h"

#include "PluginDatabase.h"
#include "PluginPackage.h"

#include "Chrome.h"
#include "ChromeClientQt.h"
#include "Page.h"
#include <qwebpage.h>
#include <qwebpluginfactory.h>

namespace WebCore {

void PluginData::initPlugins()
{
    QWebPage* webPage = static_cast<ChromeClientQt*>(m_page->chrome()->client())->m_webPage;
    QWebPluginFactory* factory = webPage->pluginFactory();
    if (factory) {

        QList<QWebPluginFactory::Plugin> qplugins = factory->plugins();
        for (int i = 0; i < qplugins.count(); ++i) {
            const QWebPluginFactory::Plugin& qplugin = qplugins.at(i);

            PluginInfo* info = new PluginInfo;
            info->name = qplugin.name;
            info->desc = qplugin.description;

            for (int j = 0; j < qplugin.mimeTypes.count(); ++j) {
                const QWebPluginFactory::MimeType& mimeType = qplugin.mimeTypes.at(j);

                MimeClassInfo* mimeInfo = new MimeClassInfo;
                mimeInfo->type = mimeType.name;
                mimeInfo->desc = mimeType.description;
                mimeInfo->suffixes = mimeType.fileExtensions.join(QLatin1String("; "));

                info->mimes.append(mimeInfo);
            }

            m_plugins.append(info);
        }
    }

    PluginDatabase *db = PluginDatabase::installedPlugins();
    const Vector<PluginPackage*> &plugins = db->plugins();

    for (unsigned int i = 0; i < plugins.size(); ++i) {
        PluginInfo* info = new PluginInfo;
        PluginPackage* package = plugins[i];

        info->name = package->name();
        info->file = package->fileName();
        info->desc = package->description();

        const MIMEToDescriptionsMap& mimeToDescriptions = package->mimeToDescriptions();
        MIMEToDescriptionsMap::const_iterator end = mimeToDescriptions.end();
        for (MIMEToDescriptionsMap::const_iterator it = mimeToDescriptions.begin(); it != end; ++it) {
            MimeClassInfo* mime = new MimeClassInfo;
            info->mimes.append(mime);

            mime->type = it->first;
            mime->desc = it->second;
            mime->plugin = info;

            Vector<String> extensions = package->mimeToExtensions().get(mime->type);

            for (unsigned i = 0; i < extensions.size(); i++) {
                if (i > 0)
                    mime->suffixes += ",";

                mime->suffixes += extensions[i];
            }
        }

        m_plugins.append(info);
    }
}

void PluginData::refresh()
{
    PluginDatabase *db = PluginDatabase::installedPlugins();
    db->refresh();
}

};
