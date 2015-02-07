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

// CLWrapper.cpp : Calls the perl script parallelcl to perform parallel compilation

#define WIN32_LEAN_AND_MEAN    // Exclude rarely-used stuff from Windows headers
#include <process.h>
#include <stdio.h>
#include <string>
#include <windows.h>

using namespace std;

int wmain(int argc, wchar_t* argv[])
{
    const int numArgs = 3;

#ifndef NDEBUG
    fwprintf(stderr, L"######### im in ur IDE, compiling ur c0des ########\n");
#endif

    wstring** args = new wstring*[numArgs];

    args[0] = new wstring(L"sh");
    args[1] = new wstring(L"-c");

    args[2] = new wstring(L"\"parallelcl");
    for (int i = 1; i < argc; ++i) {
        args[2]->append(L" '");
        args[2]->append(argv[i]);
        if (i < argc - 1)
            args[2]->append(L"' ");
        else
            args[2]->append(L"'");
    }
    args[2]->append(L"\"");

    for (unsigned i = 0; i < args[2]->length(); i++) {
       if (args[2]->at(i) == '\\')
            args[2]->at(i) = '/';
    }

    wchar_t** newArgv = new wchar_t*[numArgs + 1];
    for (int i = 0; i < numArgs; i++)
        newArgv[i] = (wchar_t*)args[i]->c_str();

    newArgv[numArgs] = 0;

#ifndef NDEBUG
    fwprintf(stderr, L"exec(\"%s\", \"%s\", \"%s\", \"%s\")\n", L"sh", newArgv[0], newArgv[1], newArgv[2]);
#endif

    return _wspawnvp(_P_WAIT, L"sh", newArgv);
}

