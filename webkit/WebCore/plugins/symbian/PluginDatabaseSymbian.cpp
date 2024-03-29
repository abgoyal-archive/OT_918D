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
    Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies)

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
#include "PluginDatabase.h"

#include <QFileInfo>
#include <f32file.h>

static const char QTPLUGIN_FILTER[] = "*.qtplugin";
static const char QT_PLUGIN_FOLDER[] = ":\\resource\\qt\\plugins\\npqtplugins\\";

namespace WebCore {

Vector<String> PluginDatabase::defaultPluginDirectories()
{
    Vector<String> directories;
    //find the installation drive
    TDriveList drivelist;
    TChar driveLetter;
    RFs fsSession;
    
    if (fsSession.Connect() == KErrNone && fsSession.DriveList(drivelist) == KErrNone) {
        for (TInt driveNumber = EDriveA; driveNumber <= EDriveZ; driveNumber++) {
            if (drivelist[driveNumber] && fsSession.DriveToChar(driveNumber, driveLetter) == KErrNone) {
                QString driveStringValue(QChar((uint)driveLetter.GetUpperCase()));
                QString stubDirPath;
                stubDirPath.append(driveStringValue);
                stubDirPath.append(QT_PLUGIN_FOLDER);
                if (QFileInfo(stubDirPath).exists())
                    directories.append(stubDirPath);
            }
        }
    }

    fsSession.Close();
    return directories;
}

bool PluginDatabase::isPreferredPluginDirectory(const String& path)
{
    return true;
}

void PluginDatabase::getPluginPathsInDirectories(HashSet<String>& paths) const
{
    // FIXME: This should be a case insensitive set.
    HashSet<String> uniqueFilenames;

    String fileNameFilter(QTPLUGIN_FILTER);

    Vector<String>::const_iterator dirsEnd = m_pluginDirectories.end();
    for (Vector<String>::const_iterator dIt = m_pluginDirectories.begin(); dIt != dirsEnd; ++dIt) {
        Vector<String> pluginPaths = listDirectory(*dIt, fileNameFilter);
        Vector<String>::const_iterator pluginsEnd = pluginPaths.end();
        for (Vector<String>::const_iterator pIt = pluginPaths.begin(); pIt != pluginsEnd; ++pIt) {
            if (!fileExists(*pIt))
                continue;
            paths.add(*pIt);
        }
    }
}

}
