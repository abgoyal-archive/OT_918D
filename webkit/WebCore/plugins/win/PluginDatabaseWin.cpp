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
 * Copyright (C) 2008-2009 Torch Mobile, Inc.  All rights reserved.
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

#include "config.h"
#include "PluginDatabase.h"

#include "Frame.h"
#include "KURL.h"
#include "PluginPackage.h"
#include <windows.h>
#include <shlwapi.h>

#if COMPILER(MINGW)
#define _countof(x) (sizeof(x)/sizeof(x[0]))
#endif

#if OS(WINCE)
// WINCE doesn't support Registry Key Access Rights. The parameter should always be 0
#define KEY_ENUMERATE_SUB_KEYS 0

DWORD SHGetValue(HKEY hkey, LPCWSTR pszSubKey, LPCWSTR pszValue, LPDWORD pdwType, LPVOID pvData, LPDWORD pcbData)
{
    HKEY key;
    if (RegOpenKeyEx(hkey, pszSubKey, 0, 0, &key) == ERROR_SUCCESS) {
        DWORD result = RegQueryValueEx(key, pszValue, 0, pdwType, (LPBYTE)pvData, pcbData);
        RegCloseKey(key);
        return result;
    }
    return ERROR_INVALID_NAME;
}

BOOL PathRemoveFileSpec(LPWSTR moduleFileNameStr)
{
    if (!*moduleFileNameStr)
        return FALSE;

    LPWSTR lastPos = 0;
    LPWSTR curPos = moduleFileNameStr;
    do {
        if (*curPos == L'/' || *curPos == L'\\')
            lastPos = curPos;
    } while (*++curPos);

    if (lastPos == curPos - 1)
        return FALSE;

    if (lastPos)
        *lastPos = 0;
    else {
        moduleFileNameStr[0] = L'\\';
        moduleFileNameStr[1] = 0;
    }

    return TRUE;
}
#endif

namespace WebCore {

static inline void addPluginPathsFromRegistry(HKEY rootKey, HashSet<String>& paths)
{
    HKEY key;
    HRESULT result = RegOpenKeyExW(rootKey, L"Software\\MozillaPlugins", 0, KEY_ENUMERATE_SUB_KEYS, &key);

    if (result != ERROR_SUCCESS)
        return;

    wchar_t name[128];
    FILETIME lastModified;

    // Enumerate subkeys
    for (int i = 0;; i++) {
        DWORD nameLen = _countof(name);
        result = RegEnumKeyExW(key, i, name, &nameLen, 0, 0, 0, &lastModified);

        if (result != ERROR_SUCCESS)
            break;

        WCHAR pathStr[_MAX_PATH];
        DWORD pathStrSize = sizeof(pathStr);
        DWORD type;

        result = SHGetValue(key, name, TEXT("Path"), &type, (LPBYTE)pathStr, &pathStrSize);
        if (result != ERROR_SUCCESS || type != REG_SZ)
            continue;

        paths.add(String(pathStr, pathStrSize / sizeof(WCHAR) - 1));
    }

    RegCloseKey(key);
}

void PluginDatabase::getPluginPathsInDirectories(HashSet<String>& paths) const
{
    // FIXME: This should be a case insensitive set.
    HashSet<String> uniqueFilenames;

    HANDLE hFind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATAW findFileData;

    String oldWMPPluginPath;
    String newWMPPluginPath;

    Vector<String>::const_iterator end = m_pluginDirectories.end();
    for (Vector<String>::const_iterator it = m_pluginDirectories.begin(); it != end; ++it) {
        String pattern = *it + "\\*";

        hFind = FindFirstFileW(pattern.charactersWithNullTermination(), &findFileData);

        if (hFind == INVALID_HANDLE_VALUE)
            continue;

        do {
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                continue;

            String filename = String(findFileData.cFileName, wcslen(findFileData.cFileName));
            if ((!filename.startsWith("np", false) || !filename.endsWith("dll", false)) &&
                (!equalIgnoringCase(filename, "Plugin.dll") || !it->endsWith("Shockwave 10", false)))
                continue;

            String fullPath = *it + "\\" + filename;
            if (!uniqueFilenames.add(fullPath).second)
                continue;

            paths.add(fullPath);

            if (equalIgnoringCase(filename, "npdsplay.dll"))
                oldWMPPluginPath = fullPath;
            else if (equalIgnoringCase(filename, "np-mswmp.dll"))
                newWMPPluginPath = fullPath;

        } while (FindNextFileW(hFind, &findFileData) != 0);

        FindClose(hFind);
    }

    addPluginPathsFromRegistry(HKEY_LOCAL_MACHINE, paths);
    addPluginPathsFromRegistry(HKEY_CURRENT_USER, paths);

    // If both the old and new WMP plugin are present in the plugins set, 
    // we remove the old one so we don't end up choosing the old one.
    if (!oldWMPPluginPath.isEmpty() && !newWMPPluginPath.isEmpty())
        paths.remove(oldWMPPluginPath);
}

static inline Vector<int> parseVersionString(const String& versionString)
{
    Vector<int> version;

    unsigned startPos = 0;
    unsigned endPos;
    
    while (startPos < versionString.length()) {
        for (endPos = startPos; endPos < versionString.length(); ++endPos)
            if (versionString[endPos] == '.' || versionString[endPos] == '_')
                break;

        int versionComponent = versionString.substring(startPos, endPos - startPos).toInt();
        version.append(versionComponent);

        startPos = endPos + 1;
    }

    return version;
}

// This returns whether versionA is higher than versionB
static inline bool compareVersions(const Vector<int>& versionA, const Vector<int>& versionB)
{
    for (unsigned i = 0; i < versionA.size(); i++) {
        if (i >= versionB.size())
            return true;

        if (versionA[i] > versionB[i])
            return true;
        else if (versionA[i] < versionB[i])
            return false;
    }

    // If we come here, the versions are either the same or versionB has an extra component, just return false
    return false;
}

static inline void addMozillaPluginDirectories(Vector<String>& directories)
{
    // Enumerate all Mozilla plugin directories in the registry
    HKEY key;
    LONG result;
    
    result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Mozilla"), 0, KEY_READ, &key);
    if (result == ERROR_SUCCESS) {
        WCHAR name[128];
        FILETIME lastModified;

        // Enumerate subkeys
        for (int i = 0;; i++) {
            DWORD nameLen = sizeof(name) / sizeof(WCHAR);
            result = RegEnumKeyExW(key, i, name, &nameLen, 0, 0, 0, &lastModified);

            if (result != ERROR_SUCCESS)
                break;

            String extensionsPath = String(name, nameLen) + "\\Extensions";
            HKEY extensionsKey;

            // Try opening the key
            result = RegOpenKeyEx(key, extensionsPath.charactersWithNullTermination(), 0, KEY_READ, &extensionsKey);

            if (result == ERROR_SUCCESS) {
                // Now get the plugins directory
                WCHAR pluginsDirectoryStr[_MAX_PATH];
                DWORD pluginsDirectorySize = sizeof(pluginsDirectoryStr);
                DWORD type;

                result = RegQueryValueEx(extensionsKey, TEXT("Plugins"), 0, &type, (LPBYTE)&pluginsDirectoryStr, &pluginsDirectorySize);

                if (result == ERROR_SUCCESS && type == REG_SZ)
                    directories.append(String(pluginsDirectoryStr, pluginsDirectorySize / sizeof(WCHAR) - 1));

                RegCloseKey(extensionsKey);
            }
        }
        
        RegCloseKey(key);
    }
}

static inline void addWindowsMediaPlayerPluginDirectory(Vector<String>& directories)
{
#if !OS(WINCE)
    // The new WMP Firefox plugin is installed in \PFiles\Plugins if it can't find any Firefox installs
    WCHAR pluginDirectoryStr[_MAX_PATH + 1];
    DWORD pluginDirectorySize = ::ExpandEnvironmentStringsW(TEXT("%SYSTEMDRIVE%\\PFiles\\Plugins"), pluginDirectoryStr, _countof(pluginDirectoryStr));

    if (pluginDirectorySize > 0 && pluginDirectorySize <= _countof(pluginDirectoryStr))
        directories.append(String(pluginDirectoryStr, pluginDirectorySize - 1));
#endif

    DWORD type;
    WCHAR installationDirectoryStr[_MAX_PATH];
    DWORD installationDirectorySize = sizeof(installationDirectoryStr);

    HRESULT result = SHGetValue(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\MediaPlayer"), TEXT("Installation Directory"), &type, (LPBYTE)&installationDirectoryStr, &installationDirectorySize);

    if (result == ERROR_SUCCESS && type == REG_SZ)
        directories.append(String(installationDirectoryStr, installationDirectorySize / sizeof(WCHAR) - 1));
}

static inline void addQuickTimePluginDirectory(Vector<String>& directories)
{
    DWORD type;
    WCHAR installationDirectoryStr[_MAX_PATH];
    DWORD installationDirectorySize = sizeof(installationDirectoryStr);

    HRESULT result = SHGetValue(HKEY_LOCAL_MACHINE, TEXT("Software\\Apple Computer, Inc.\\QuickTime"), TEXT("InstallDir"), &type, (LPBYTE)&installationDirectoryStr, &installationDirectorySize);

    if (result == ERROR_SUCCESS && type == REG_SZ) {
        String pluginDir = String(installationDirectoryStr, installationDirectorySize / sizeof(WCHAR) - 1) + "\\plugins";
        directories.append(pluginDir);
    }
}

static inline void addAdobeAcrobatPluginDirectory(Vector<String>& directories)
{
    HKEY key;
    HRESULT result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Adobe\\Acrobat Reader"), 0, KEY_READ, &key);
    if (result != ERROR_SUCCESS)
        return;

    WCHAR name[128];
    FILETIME lastModified;

    Vector<int> latestAcrobatVersion;
    String latestAcrobatVersionString;

    // Enumerate subkeys
    for (int i = 0;; i++) {
        DWORD nameLen = sizeof(name) / sizeof(WCHAR);
        result = RegEnumKeyExW(key, i, name, &nameLen, 0, 0, 0, &lastModified);

        if (result != ERROR_SUCCESS)
            break;

        Vector<int> acrobatVersion = parseVersionString(String(name, nameLen));
        if (compareVersions(acrobatVersion, latestAcrobatVersion)) {
            latestAcrobatVersion = acrobatVersion;
            latestAcrobatVersionString = String(name, nameLen);
        }
    }

    if (!latestAcrobatVersionString.isNull()) {
        DWORD type;
        WCHAR acrobatInstallPathStr[_MAX_PATH];
        DWORD acrobatInstallPathSize = sizeof(acrobatInstallPathStr);

        String acrobatPluginKeyPath = "Software\\Adobe\\Acrobat Reader\\" + latestAcrobatVersionString + "\\InstallPath";
        result = SHGetValue(HKEY_LOCAL_MACHINE, acrobatPluginKeyPath.charactersWithNullTermination(), 0, &type, (LPBYTE)acrobatInstallPathStr, &acrobatInstallPathSize);

        if (result == ERROR_SUCCESS) {
            String acrobatPluginDirectory = String(acrobatInstallPathStr, acrobatInstallPathSize / sizeof(WCHAR) - 1) + "\\browser";
            directories.append(acrobatPluginDirectory);
        }
    }

    RegCloseKey(key);
}

static inline String safariPluginsDirectory()
{
    WCHAR moduleFileNameStr[_MAX_PATH];
    static String pluginsDirectory;
    static bool cachedPluginDirectory = false;

    if (!cachedPluginDirectory) {
        cachedPluginDirectory = true;

        int moduleFileNameLen = GetModuleFileName(0, moduleFileNameStr, _MAX_PATH);

        if (!moduleFileNameLen || moduleFileNameLen == _MAX_PATH)
            goto exit;

        if (!PathRemoveFileSpec(moduleFileNameStr))
            goto exit;

        pluginsDirectory = String(moduleFileNameStr) + "\\Plugins";
    }
exit:
    return pluginsDirectory;
}

static inline void addMacromediaPluginDirectories(Vector<String>& directories)
{
#if !OS(WINCE)
    WCHAR systemDirectoryStr[MAX_PATH];

    if (GetSystemDirectory(systemDirectoryStr, _countof(systemDirectoryStr)) == 0)
        return;

    WCHAR macromediaDirectoryStr[MAX_PATH];

    PathCombine(macromediaDirectoryStr, systemDirectoryStr, TEXT("macromed\\Flash"));
    directories.append(macromediaDirectoryStr);

    PathCombine(macromediaDirectoryStr, systemDirectoryStr, TEXT("macromed\\Shockwave 10"));
    directories.append(macromediaDirectoryStr);
#endif
}

Vector<String> PluginDatabase::defaultPluginDirectories()
{
    Vector<String> directories;
    String ourDirectory = safariPluginsDirectory();

    if (!ourDirectory.isNull())
        directories.append(ourDirectory);
    addQuickTimePluginDirectory(directories);
    addAdobeAcrobatPluginDirectory(directories);
    addMozillaPluginDirectories(directories);
    addWindowsMediaPlayerPluginDirectory(directories);
    addMacromediaPluginDirectories(directories);

    return directories;
}

bool PluginDatabase::isPreferredPluginDirectory(const String& directory)
{
    String ourDirectory = safariPluginsDirectory();

    if (!ourDirectory.isNull() && !directory.isNull())
        return ourDirectory == directory;

    return false;
}

}
