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
    Copyright (C) 2007 Staikos Computing Services Inc.  <info@staikos.net>

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

    This class provides all functionality needed for tracking global history.
*/

#include "config.h"
#include "qwebhistoryinterface.h"

#include <QCoreApplication>

#include "PageGroup.h"
#include "PlatformString.h"


static QWebHistoryInterface* default_interface;

static bool gRoutineAdded;

static void gCleanupInterface()
{
    if (default_interface && !default_interface->parent())
        delete default_interface;
    default_interface = 0;
}

/*!
  Sets a new default interface, \a defaultInterface, that will be used by all of WebKit
  for managing history.

  If an interface without a parent has already been set, the old interface will be deleted.
  When the application exists QWebHistoryInterface will automatically delete the
  \a defaultInterface if it does not have a parent.
*/
void QWebHistoryInterface::setDefaultInterface(QWebHistoryInterface* defaultInterface)
{
    if (default_interface == defaultInterface)
        return;

    if (default_interface && !default_interface->parent())
        delete default_interface;

    default_interface = defaultInterface;
    WebCore::PageGroup::removeAllVisitedLinks();

    //### enable after the introduction of a version
    //WebCore::PageGroup::setShouldTrackVisitedLinks(true);

    if (!gRoutineAdded) {
        qAddPostRoutine(gCleanupInterface);
        gRoutineAdded = true;
    }
}

/*!
  Returns the default interface that will be used by WebKit. If no
  default interface has been set, QtWebkit will not track history.
*/
QWebHistoryInterface* QWebHistoryInterface::defaultInterface()
{
    return default_interface;
}

/*!
  \class QWebHistoryInterface
  \since 4.4
  \brief The QWebHistoryInterface class provides an interface to implement link history.

  \inmodule QtWebKit

  The QWebHistoryInterface is an interface that can be used to
  implement link history. It contains two pure virtual methods that
  are called by the WebKit engine.  addHistoryEntry() is used to add
  pages that have been visited to the interface, while
  historyContains() is used to query whether this page has been
  visited by the user.
*/

/*!
    Constructs a new QWebHistoryInterface with parent \a parent.
*/
QWebHistoryInterface::QWebHistoryInterface(QObject* parent)
    : QObject(parent)
{
}

/*!
    Destructor.  If this is currently the default interface it will be unset.
*/
QWebHistoryInterface::~QWebHistoryInterface()
{
    if (default_interface == this)
        default_interface = 0;
}

/*!
  \fn bool QWebHistoryInterface::historyContains(const QString &url) const = 0

  Called by the WebKit engine to query whether a certain \a url has been visited by the user already.
  Returns true if the \a url is part of the history of visited links; otherwise returns false.
*/

/*!
  \fn void QWebHistoryInterface::addHistoryEntry(const QString &url) = 0

  Called by WebKit to add another \a url to the list of visited pages.
*/
