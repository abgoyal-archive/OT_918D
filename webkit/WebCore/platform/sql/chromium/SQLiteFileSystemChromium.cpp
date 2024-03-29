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

#include "config.h"
#include "SQLiteFileSystem.h"

#include "ChromiumBridge.h"
#include "CString.h"
#include "SQLiteDatabase.h"
#include <sqlite3.h>

#ifndef SQLITE_OPEN_FULLMUTEX
#define SQLITE_OPEN_FULLMUTEX 0x00010000
#endif

// SQLiteFileSystem::registerSQLiteVFS() is implemented in the
// platform-specific files SQLiteFileSystemChromium{Win|Posix}.cpp
namespace WebCore {

SQLiteFileSystem::SQLiteFileSystem()
{
}

int SQLiteFileSystem::openDatabase(const String& fileName, sqlite3** database)
{
    if (!ChromiumBridge::sandboxEnabled()) {
        String path = fileName;
        return sqlite3_open16(path.charactersWithNullTermination(), database);
    }

    // open databases using the default VFS
    // in renderers, it should be Chromium's VFS; in the browser process it should be SQLite's default VFS
    return sqlite3_open_v2(fileName.utf8().data(), database,
                           SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX,
                           "chromium_vfs");
}

String SQLiteFileSystem::getFileNameForNewDatabase(
  const String&, const String& dbName, const String &originIdentifier, SQLiteDatabase*)
{
    // Chromium names DB files based on origin and DB name only
    return originIdentifier + "_" + dbName + ".db";
}

String SQLiteFileSystem::appendDatabaseFileNameToPath(const String&, const String& fileName)
{
    // Chromium saves all DB files in the same directory (known by
    // the browser process only); as far as the renderer processes
    // are concerned, all DB files are saved in the "current" directory
    return fileName;
}

bool SQLiteFileSystem::ensureDatabaseDirectoryExists(const String&)
{
    // if the directory where Chromium stores the databases does not exist,
    // it will be automatically created by the browser process;
    // so as far as the WebKit code is concerned, this directory always exists
    return true;
}

bool SQLiteFileSystem::ensureDatabaseFileExists(const String&, bool)
{
    // all database directories will be created as needed by the browser process
    return true;
}

bool SQLiteFileSystem::deleteEmptyDatabaseDirectory(const String&)
{
    // Chromium does not use a separate directory for each database,
    // so there's nothing to do here
    return true;
}

bool SQLiteFileSystem::deleteDatabaseFile(const String& fileName)
{
    return (ChromiumBridge::databaseDeleteFile(fileName) == SQLITE_OK);
}

long long SQLiteFileSystem::getDatabaseFileSize(const String& fileName)
{
    return ChromiumBridge::databaseGetFileSize(fileName);
}

} // namespace WebCore
