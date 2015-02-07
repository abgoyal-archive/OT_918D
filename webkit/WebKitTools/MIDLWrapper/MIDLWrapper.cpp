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

// MIDLWrapper.cpp : Just calls the built-in midl.exe with the given arguments.

#define WIN32_LEAN_AND_MEAN    // Exclude rarely-used stuff from Windows headers
#include <process.h>
#include <stdio.h>
#include <string>
#include <windows.h>

using namespace std;

int wmain(int argc, wchar_t* argv[], wchar_t* envp[])
{
#ifndef NDEBUG
    fwprintf(stderr, L"######### im in ur IDE, compiling ur c0des ########\n");
#endif

    int pathIndex = -1;
    for (int i = 0; envp[i]; ++i)
        if (!wcsncmp(envp[i], L"PATH=", 5)) {
            pathIndex = i;
            break;
        }

    if (pathIndex == -1) {
        fwprintf(stderr, L"Couldn't find PATH environment variable!\n");
        return -1;
    }

    wchar_t* vcbin = wcsstr(envp[pathIndex], L"WebKitTools\\vcbin");
    if (!vcbin) {
        fwprintf(stderr, L"Couldn't find WebKitTools\\vcbin in PATH!\n");
        return -1;
    }

    wchar_t saved = *vcbin;
    *vcbin = 0;
    
    wchar_t* afterLeadingSemiColon = wcsrchr(envp[pathIndex], ';');
    if (!afterLeadingSemiColon)
        afterLeadingSemiColon = envp[pathIndex] + 5; // +5 for the length of "PATH="
    else
        afterLeadingSemiColon++;

    *vcbin = saved;

    size_t pathLength = wcslen(envp[pathIndex]);

    wchar_t* trailingSemiColon = wcschr(vcbin, ';');
    if (!trailingSemiColon)
        trailingSemiColon = envp[pathIndex] + pathLength;

    int vcbinLength = trailingSemiColon - afterLeadingSemiColon;

    size_t newPathLength = pathLength - vcbinLength;

    wchar_t* newPath = new wchar_t[newPathLength + 1];

    // Copy everything before the vcbin path...
    wchar_t* d = newPath;
    wchar_t* s = envp[pathIndex];
    while (s < afterLeadingSemiColon)
        *d++ = *s++;

    // Copy everything after the vcbin path...
    s = trailingSemiColon;
    while (*d++ = *s++);

    envp[pathIndex] = newPath;

#ifndef NDEBUG
    fwprintf(stderr, L"New path: %s\n", envp[pathIndex]);
#endif

    wchar_t** newArgv = new wchar_t*[argc + 1];
    for (int i = 0; i < argc; ++i) {
        size_t length = wcslen(argv[i]);
        newArgv[i] = new wchar_t[length + 3];
        *newArgv[i] = '\"';
        wcscpy_s(newArgv[i] + 1, length + 2, argv[i]);
        *(newArgv[i] + 1 + length) = '\"';
        *(newArgv[i] + 2 + length) = 0;
    }
    newArgv[argc] = 0;

    return _wspawnvpe(_P_WAIT, L"midl", newArgv, envp);
}
