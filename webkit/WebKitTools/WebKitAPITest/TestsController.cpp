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
 * Copyright (C) 2010 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "TestsController.h"

#include "Test.h"
#include <wtf/PassOwnPtr.h>

using namespace std;

namespace WebKitAPITest {

static const LPCWSTR testsControllerWindowClassName = L"TestsControllerWindowClass";

enum { runNextTestTimerID = 1 };

inline TestsController::TestsController()
    : m_testFailed(false)
    , m_anyTestFailed(false)
{
    registerWindowClass();
    m_window = CreateWindowExW(0, testsControllerWindowClassName, 0, WS_CHILD, 0, 0, 0, 0, HWND_MESSAGE, 0, GetModuleHandle(0), 0);
}

TestsController& TestsController::shared()
{
    static TestsController& shared = *new TestsController;
    return shared;
}

bool TestsController::runAllTests()
{
    if (m_tests.isEmpty())
        return true;

    MSG msg;
    BOOL result;
    while ((result = GetMessage(&msg, 0, 0, 0))) {
        if (result == -1)
            break;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (msg.message != WM_QUIT)
        return false;

    return !m_anyTestFailed;
}

void TestsController::addTest(PassOwnPtr<Test> test)
{
    m_tests.append(test.release());
    runNextTestSoon();
}

void TestsController::testFailed(const char* file, int line, const char* message)
{
    ASSERT(!m_tests.isEmpty());

    m_testFailed = true;
    m_anyTestFailed = true;

    printf("FAIL: %s\n\t%s (%s:%d)\n", m_tests.first()->name(), message, file, line);
    fflush(stdout);
}

void TestsController::runNextTest()
{
    if (m_tests.isEmpty()) {
        PostQuitMessage(0);
        return;
    }

    Test* test = m_tests.first();

    m_testFailed = false;
    printf("RUN: %s\n", test->name());
    fflush(stdout);
    test->run();

    if (!m_testFailed) {
        printf("PASS: %s\n", test->name());
        fflush(stdout);
    }

    m_tests.removeFirst();
    delete test;

    runNextTestSoon();
}

void TestsController::runNextTestSoon()
{
    SetTimer(m_window, runNextTestTimerID, 0, 0);
}

void TestsController::registerWindowClass()
{
    static bool initialized;
    if (initialized)
        return;
    initialized = true;

    WNDCLASSEXW wndClass = {0};
    wndClass.cbSize = sizeof(wndClass);
    wndClass.lpfnWndProc = wndProc;
    wndClass.hCursor = LoadCursor(0, IDC_ARROW);
    wndClass.hInstance = GetModuleHandle(0);
    wndClass.lpszClassName = testsControllerWindowClassName;

    RegisterClassExW(&wndClass);
}

LRESULT TestsController::wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_TIMER && wParam == runNextTestTimerID) {
        KillTimer(hWnd, runNextTestTimerID);
        TestsController::shared().runNextTest();
        return 0;
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

} // namespace WebKitAPITest
