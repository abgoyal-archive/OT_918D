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
 * Copyright (C) Research In Motion Limited 2009-2010. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef SurfaceOpenVG_h
#define SurfaceOpenVG_h

#if PLATFORM(EGL)
#include <egl.h>
#endif

#include <wtf/Noncopyable.h>
#include <wtf/Platform.h>

namespace WebCore {

#if PLATFORM(EGL)
class EGLDisplayOpenVG;
#endif
class PainterOpenVG;
class IntSize;

/**
 * SurfaceOpenVG provides the functionality of surfaces and contexts that are
 * underlying the OpenVG implementation. In the vast majority of cases, that
 * underlying technology is EGL, but OpenVG doesn't depend on EGL per se.
 * Wrapping surface/context functionality into a separate class avoids lots
 * of #ifdefs and should make it easy to add different surface/context
 * implementations than EGL.
 */
class SurfaceOpenVG : public Noncopyable {
public:
    enum MakeCurrentMode {
        ApplyPainterStateOnSurfaceSwitch,
        DontApplyPainterState,
    };

    static SurfaceOpenVG* currentSurface();

#if PLATFORM(EGL)
    friend class EGLDisplayOpenVG;

    /**
     * Create a new EGL pbuffer surface with the specified size and config on
     * the given display. If config is not specified, the display's default
     * pbuffer config is used.
     *
     * This constructor will trigger an assertion if creation of the surface
     * fails, unless you pledge to manually process the error code by passing
     * a non-zero pointer as errorCode parameter. The error code returned by
     * eglGetError() will be written to that variable.
     */
    SurfaceOpenVG(const IntSize& size, const EGLDisplay& display, EGLConfig* config = 0, EGLint* errorCode = 0);

    /**
     * Create a new EGL window surface with the specified native window handle
     * and config on the given display. If config is not specified, the
     * display's default window config is used.
     */
    SurfaceOpenVG(EGLNativeWindowType window, const EGLDisplay& display, EGLConfig* config = 0);

    EGLDisplay eglDisplay() const { return m_eglDisplay; }
    EGLSurface eglSurface() const { return m_eglSurface; }
    EGLContext eglContext() const { return m_eglContext; }
#endif

    ~SurfaceOpenVG();

    /**
     * If a surface is invalid (could not be created), all method calls will
     * crash horribly.
     */
    bool isValid() const;

    int width() const;
    int height() const;

    SurfaceOpenVG* sharedSurface() const;

    /**
     * Make the associated GL/EGL context the current one, so that subsequent
     * OpenVG commands apply to it.
     */
    void makeCurrent(MakeCurrentMode mode = ApplyPainterStateOnSurfaceSwitch);

    /**
     * Make a surface/context combination current that is "compatible"
     * (i.e. can access its shared resources) to the given one. If no
     * surface/context is current, the given one is made current.
     *
     * This method is meant to avoid context changes if they're not
     * necessary, particularly tailored for the case where something
     * compatible to the shared surface is requested while actual painting
     * happens on another surface.
     */
    void makeCompatibleCurrent();

    /**
     * Empty the OpenVG pipeline and make sure all the performed paint
     * operations show up on the surface as actual drawn pixels.
     */
    void flush();

    void setActivePainter(PainterOpenVG*);
    PainterOpenVG* activePainter();

private:
    PainterOpenVG* m_activePainter;
    static PainterOpenVG* s_currentPainter; // global currently active painter

#if PLATFORM(EGL)
    SurfaceOpenVG(); // for EGLDisplayOpenVG

    EGLDisplay m_eglDisplay;
    EGLSurface m_eglSurface;
    EGLContext m_eglContext;
#endif
};

}

#endif
