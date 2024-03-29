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
 * Copyright (C) 2007, 2008, 2009, 2010 Apple, Inc.  All rights reserved.
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

#ifndef QTMovieWin_h
#define QTMovieWin_h

#include <Unicode.h>
#include <windows.h>

#ifdef QTMOVIEWIN_EXPORTS
#define QTMOVIEWIN_API __declspec(dllexport)
#else
#define QTMOVIEWIN_API __declspec(dllimport)
#endif

class QTMovieWin;
class QTMovieWinPrivate;

class QTMovieWinClient {
public:
    virtual void movieEnded(QTMovieWin*) = 0;
    virtual void movieLoadStateChanged(QTMovieWin*) = 0;
    virtual void movieTimeChanged(QTMovieWin*) = 0;
    virtual void movieNewImageAvailable(QTMovieWin*) = 0;
};

class QTMovieWinFullscreenClient {
public:
    virtual LRESULT fullscreenClientWndProc(HWND, UINT message, WPARAM, LPARAM) = 0;
};

enum {
    QTMovieLoadStateError = -1L,
    QTMovieLoadStateLoaded  = 2000L,
    QTMovieLoadStatePlayable = 10000L,
    QTMovieLoadStatePlaythroughOK = 20000L,
    QTMovieLoadStateComplete = 100000L
};

typedef const struct __CFURL * CFURLRef;

class QTMOVIEWIN_API QTMovieWin {
public:
    static bool initializeQuickTime();

    typedef void (*SetTaskTimerDelayFunc)(double);
    typedef void (*StopTaskTimerFunc)();
    static void setTaskTimerFuncs(SetTaskTimerDelayFunc, StopTaskTimerFunc);
    static void taskTimerFired();

    QTMovieWin(QTMovieWinClient*);
    ~QTMovieWin();

    void load(const UChar* url, int len, bool preservesPitch);
    long loadState() const;
    float maxTimeLoaded() const;

    void play();
    void pause();

    float rate() const;
    void setRate(float);

    float duration() const;
    float currentTime() const;
    void setCurrentTime(float) const;

    void setVolume(float);
    void setPreservesPitch(bool);

    unsigned dataSize() const;

    void getNaturalSize(int& width, int& height);
    void setSize(int width, int height);

    void setVisible(bool);
    void paint(HDC, int x, int y);
    void getCurrentFrameInfo(void*& buffer, unsigned& bitsPerPixel, unsigned& rowBytes, unsigned& width, unsigned& height);

    void disableUnsupportedTracks(unsigned& enabledTrackCount, unsigned& totalTrackCount);
    void setDisabled(bool);

    bool hasVideo() const;
    bool hasAudio() const;

    bool hasClosedCaptions() const;
    void setClosedCaptionsVisible(bool);

    static unsigned countSupportedTypes();
    static void getSupportedType(unsigned index, const UChar*& str, unsigned& len);

    // Returns the full-screen window created
    HWND enterFullscreen(QTMovieWinFullscreenClient*);
    void exitFullscreen();

private:
    void load(CFURLRef, bool preservesPitch);
    static LRESULT fullscreenWndProc(HWND, UINT message, WPARAM, LPARAM);

    QTMovieWinPrivate* m_private;
    bool m_disabled;
    friend class QTMovieWinPrivate;
};

#endif
