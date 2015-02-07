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
 * Copyright (C) 2008-2009 Torch Mobile Inc.
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

#include "config.h"
#include "Cursor.h"

namespace WebCore {

struct AllCursors {
    AllCursors()
    {
        for (int i = 0; i < NumCursorTypes; ++i)
            m_cursors[i] = (CursorType) i;
    }
    Cursor m_cursors[NumCursorTypes];
};

static const Cursor& getCursor(CursorType type)
{
    static AllCursors allCursors;
    return allCursors.m_cursors[type];
}

Cursor::Cursor(const Cursor& other)
: m_impl(other.m_impl)
{
}

Cursor::Cursor(Image* img, const IntPoint& hotspot)
: m_impl(CursorNone)
{
}

Cursor::~Cursor()
{
}

Cursor& Cursor::operator=(const Cursor& other)
{
    m_impl = other.m_impl;
    return *this;
}

Cursor::Cursor(PlatformCursor c)
: m_impl(c)
{
}

const Cursor& noneCursor() { return getCursor(CursorNone); }
const Cursor& pointerCursor() { return getCursor(CursorPointer); }
const Cursor& crossCursor()  { return getCursor(CursorCross); }
const Cursor& handCursor() { return getCursor(CursorHand); }
const Cursor& iBeamCursor() { return getCursor(CursorBeam); }
const Cursor& waitCursor() { return getCursor(CursorWait); }
const Cursor& helpCursor() { return getCursor(CursorHelp); }
const Cursor& moveCursor()  { return getCursor(CursorMove); }
const Cursor& eastResizeCursor() { return getCursor(CursorEastResize); }
const Cursor& northResizeCursor() { return getCursor(CursorNorthResize); }
const Cursor& northEastResizeCursor() { return getCursor(CursorNorthEastResize); }
const Cursor& northWestResizeCursor() { return getCursor(CursorNorthWestResize); }
const Cursor& southResizeCursor() { return getCursor(CursorSouthResize); }
const Cursor& southEastResizeCursor() { return getCursor(CursorSouthEastResize); }
const Cursor& southWestResizeCursor() { return getCursor(CursorSouthWestResize); }
const Cursor& westResizeCursor() { return getCursor(CursorWestResize); }
const Cursor& northSouthResizeCursor() { return getCursor(CursorNorthSouthResize); }
const Cursor& eastWestResizeCursor() { return getCursor(CursorEastWestResize); }
const Cursor& northEastSouthWestResizeCursor() { return getCursor(CursorNorthEastSouthWestResize); }
const Cursor& northWestSouthEastResizeCursor() { return getCursor(CursorNorthWestSouthEastResize); }
const Cursor& columnResizeCursor() { return getCursor(CursorColumnResize); }
const Cursor& rowResizeCursor() { return getCursor(CursorRowResize); }
const Cursor& verticalTextCursor() { return getCursor(CursorVerticalText); }
const Cursor& cellCursor() { return getCursor(CursorCell); }
const Cursor& contextMenuCursor() { return getCursor(CursorContextMenu); }
const Cursor& noDropCursor() { return getCursor(CursorNoDrop); }
const Cursor& notAllowedCursor() { return getCursor(CursorNotAllowed); }
const Cursor& progressCursor() { return getCursor(CursorProgress); }
const Cursor& aliasCursor() { return getCursor(CursorAlias); }
const Cursor& zoomInCursor() { return getCursor(CursorZoomIn); }
const Cursor& zoomOutCursor() { return getCursor(CursorZoomOut); }
const Cursor& copyCursor() { return getCursor(CursorCopy); }
const Cursor& middlePanningCursor() { return crossCursor(); }
const Cursor& eastPanningCursor() { return crossCursor(); }
const Cursor& northPanningCursor() { return crossCursor(); }
const Cursor& northEastPanningCursor() { return crossCursor(); }
const Cursor& northWestPanningCursor() { return crossCursor(); }
const Cursor& southPanningCursor() { return crossCursor(); }
const Cursor& southEastPanningCursor() { return crossCursor(); }
const Cursor& southWestPanningCursor() { return crossCursor(); }
const Cursor& westPanningCursor() { return crossCursor(); }
const Cursor& grabbingCursor() { return moveCursor(); }
const Cursor& grabCursor() { return moveCursor(); }

}
