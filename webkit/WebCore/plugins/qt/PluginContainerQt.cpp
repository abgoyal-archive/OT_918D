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
#include "PluginContainerQt.h"

#include "FocusController.h"
#include "Frame.h"
#include "FrameView.h"
#include "Page.h"
#include "PlatformKeyboardEvent.h"
#include "PlatformWheelEvent.h"
#include "PluginView.h"
#include <QApplication>
#include <QX11Info>

using namespace WebCore;

PluginClientWrapper::PluginClientWrapper(QWidget* parent, WId client)
    : QWidget(0, Qt::Popup)
{
    // create a QWidget that adopts the plugin window id, do not give it
    // a parent so that we don't end up handling events supposed to be
    // handled by the QX11EmbedContainer.

    // without the parent this will be considered a toplevel widget,
    // and thus make Qt not quit the event loop after the last window
    // has been closed. In order to work around this, we set the window
    // type to Qt::Popup.

    create(client, false, true);
    m_parent = parent;
}

PluginClientWrapper::~PluginClientWrapper()
{
    destroy(false, false);
}

bool PluginClientWrapper::x11Event(XEvent* event)
{
    // modify the event window id and insert it into the Qt event system.
    event->xany.window = m_parent->parentWidget()->winId();
    static_cast<QApplication*>(QApplication::instance())->x11ProcessEvent(event);
    return true;
}

PluginContainerQt::PluginContainerQt(PluginView* view, QWidget* parent)
    : QX11EmbedContainer(parent)
    , m_pluginView(view)
    , m_clientWrapper(0)
{
    connect(this, SIGNAL(clientClosed()), this, SLOT(on_clientClosed()));
    connect(this, SIGNAL(clientIsEmbedded()), this, SLOT(on_clientIsEmbedded()));
}

PluginContainerQt::~PluginContainerQt()
{
    delete m_clientWrapper;
    m_pluginView->setPlatformPluginWidget(0);
}

void PluginContainerQt::on_clientClosed()
{
    delete m_clientWrapper;
    m_clientWrapper = 0;
}

void PluginContainerQt::on_clientIsEmbedded()
{
    delete m_clientWrapper;
    m_clientWrapper = 0;

    // Only create a QWidget wrapper for the plugin in the case it isn't in the
    // Qt window mapper, and thus receiving events from the Qt event system.
    // This way the PluginClientWrapper receives the scroll events and passes
    // them to the parent. NOTICE: Native Qt based plugins running in process,
    // will already be in the window mapper, and thus creating a wrapper, stops
    // them from getting events from Qt, as they are redirected to the wrapper.
    if (!QWidget::find(clientWinId()))
        m_clientWrapper = new PluginClientWrapper(this, clientWinId());
}

void PluginContainerQt::redirectWheelEventsToParent(bool enable)
{
    // steal wheel events from the plugin as we want to handle it. When doing this
    // all button 4, 5, 6, and 7, ButtonPress and ButtonRelease events are passed
    // to the x11Event handler of the PluginClientWrapper, which then changes the
    // window id of the event to the parent of PluginContainer and puts the event
    // back into the Qt event loop, so that we will actually scroll the parent
    // frame.
    for (int buttonNo = 4; buttonNo < 8; buttonNo++) {
        if (enable)
            XGrabButton(x11Info().display(), buttonNo, AnyModifier, clientWinId(),
                false, ButtonPressMask, GrabModeAsync, GrabModeAsync, 0L, 0L);
        else
            XUngrabButton(x11Info().display(), buttonNo, AnyModifier, clientWinId());
    }
}

bool PluginContainerQt::x11Event(XEvent* event)
{
    switch (event->type) {
    case EnterNotify:
        // if the plugin window doesn't have focus we do not want to send wheel
        // events to it, but to the parent frame, so let's redirect here.
        redirectWheelEventsToParent(!hasFocus());
        break;
    case LeaveNotify:
        // it is always safe to ungrab wheel events when the mouse leaves the
        // plugin window.
        redirectWheelEventsToParent(false);
        break;
    }

    return QX11EmbedContainer::x11Event(event);
}

void PluginContainerQt::focusInEvent(QFocusEvent* event)
{
    // we got focus, stop redirecting the wheel events
    redirectWheelEventsToParent(false);

    if (Page* page = m_pluginView->parentFrame()->page())
        page->focusController()->setActive(true);

    m_pluginView->focusPluginElement();
}

void PluginContainerQt::focusOutEvent(QFocusEvent*)
{
    if (Page* page = m_pluginView->parentFrame()->page())
        page->focusController()->setActive(false);
}
