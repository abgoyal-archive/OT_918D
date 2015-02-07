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
 * This file is part of the WebKit project.
 *
 * Copyright (C) 2006 Apple Computer, Inc.
 * Copyright (C) 2006 Michael Emmel mike.emmel@gmail.com
 * Copyright (C) 2007 Holger Hans Peter Freyther
 * Copyright (C) 2007 Alp Toker <alp@atoker.com>
 * Copyright (C) 2008, 2009 Google, Inc.
 * All rights reserved.
 * Copyright (C) 2009 Kenneth Rohde Christiansen
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
 *
 */

#ifndef RenderThemeChromiumLinux_h
#define RenderThemeChromiumLinux_h

#include "RenderThemeChromiumSkia.h"

namespace WebCore {

    class RenderThemeChromiumLinux : public RenderThemeChromiumSkia {
    public:
        static PassRefPtr<RenderTheme> create();
        virtual String extraDefaultStyleSheet();

        virtual Color systemColor(int cssValidId) const;

        // A method asking if the control changes its tint when the window has focus or not.
        virtual bool controlSupportsTints(const RenderObject*) const;

        // List Box selection color
        virtual Color activeListBoxSelectionBackgroundColor() const;
        virtual Color activeListBoxSelectionForegroundColor() const;
        virtual Color inactiveListBoxSelectionBackgroundColor() const;
        virtual Color inactiveListBoxSelectionForegroundColor() const;

        virtual Color platformActiveSelectionBackgroundColor() const;
        virtual Color platformInactiveSelectionBackgroundColor() const;
        virtual Color platformActiveSelectionForegroundColor() const;
        virtual Color platformInactiveSelectionForegroundColor() const;

        virtual void adjustSliderThumbSize(RenderObject*) const;

        static void setCaretBlinkInterval(double interval);
        virtual double caretBlinkIntervalInternal() const;

        static void setSelectionColors(unsigned activeBackgroundColor,
                                       unsigned activeForegroundColor,
                                       unsigned inactiveBackgroundColor,
                                       unsigned inactiveForegroundColor);

        static void setScrollbarColors(unsigned inactive_color,
                                       unsigned active_color,
                                       unsigned track_color);
        static unsigned thumbInactiveColor() { return m_thumbInactiveColor; }
        static unsigned thumbActiveColor() { return m_thumbActiveColor; }
        static unsigned trackColor() { return m_trackColor; }

    private:
        RenderThemeChromiumLinux();
        virtual ~RenderThemeChromiumLinux();

        // A general method asking if any control tinting is supported at all.
        virtual bool supportsControlTints() const;

        static double m_caretBlinkInterval;

        static unsigned m_activeSelectionBackgroundColor;
        static unsigned m_activeSelectionForegroundColor;
        static unsigned m_inactiveSelectionBackgroundColor;
        static unsigned m_inactiveSelectionForegroundColor;

        static unsigned m_thumbInactiveColor;
        static unsigned m_thumbActiveColor;
        static unsigned m_trackColor;
    };

} // namespace WebCore

#endif // RenderThemeChromiumLinux_h
