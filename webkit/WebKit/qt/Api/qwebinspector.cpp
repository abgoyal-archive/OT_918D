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
#include "qwebinspector.h"

#include "Element.h"
#include "InspectorController.h"
#include "qwebelement.h"
#include "qwebinspector_p.h"
#include "qwebpage_p.h"

#include <QResizeEvent>

/*!
    \class QWebInspector
    \since 4.6
    \brief The QWebInspector class allows the placement and control of a
    QWebPage's inspector.
    The inspector allows you to see a page current hierarchy and loading
    statistics.

    The QWebPage to be inspected is determined with the setPage() method.

    A typical use of QWebInspector follows:

    \snippet webkitsnippets/qtwebkit_qwebinspector_snippet.cpp 0

    \note A QWebInspector will display a blank widget if either:
    \list
        \o page() is null
        \o QWebSettings::DeveloperExtrasEnabled is false
    \endlist

    \section1 Resources

    This class acts mostly as a container and a controller for the inspector.
    Most of the resources needed by the inspector are owned by the associated
    QWebPage and are allocated the first time that:
    \list
        \o an element is inspected
        \o the QWebInspector is shown.
    \endlist

    \section1 Inspector configuration persistence

    The inspector allows the user to configure some options through its
    interface (e.g. the resource tracking "Always enable" option).
    These settings are persisted automatically by QtWebKit using QSettings.

    However since the QSettings object is instantiated using the empty
    constructor, QCoreApplication::setOrganizationName() and
    QCoreApplication::setApplicationName() must be called within your
    application to enable the persistence of these options.
*/

/*!
    Constructs an empty QWebInspector with parent \a parent.
*/
QWebInspector::QWebInspector(QWidget* parent)
    : QWidget(parent)
    , d(new QWebInspectorPrivate(this))
{
}

/*!
    Destroys the inspector.
*/
QWebInspector::~QWebInspector()
{
    // Remove association principally to prevent deleting a child frontend
    setPage(0);
}

/*!
    Sets the QWebPage to be inspected.

    There can only be one QWebInspector associated with a QWebPage
    and vices versa.

    Calling with \a page as null will break the current association, if any.

    If \a page is already associated to another QWebInspector, the association
    will be replaced and the previous QWebInspector will have no page
    associated.

    \sa page()
*/
void QWebInspector::setPage(QWebPage* page)
{
    if (d->page) {
        // Break currentPage-->this
        d->page->d->setInspector(0);
    }
    if (page && page->d->inspector && page->d->inspector != this) {
        // Break newPage<->newPageCurrentInspector
        page->d->inspector->setPage(0);
    }

    d->page = page;

    if (page) {
        // Setup the reciprocal association
        page->d->setInspector(this);
    }
}

/*!
    Returns the inspected QWebPage.
    If no web page is currently associated, a null pointer is returned.
*/
QWebPage* QWebInspector::page() const
{
    return d->page;
}

/*! \reimp */
QSize QWebInspector::sizeHint() const
{
    return QSize(450, 300);
}

/*! \reimp */
bool QWebInspector::event(QEvent* ev)
{
    return QWidget::event(ev);
}

/*! \reimp */
void QWebInspector::resizeEvent(QResizeEvent* event)
{
    d->adjustFrontendSize(event->size());
}

/*! \reimp */
void QWebInspector::showEvent(QShowEvent* event)
{
#if ENABLE(INSPECTOR)
    // Allows QWebInspector::show() to init the inspector.
    if (d->page)
        d->page->d->inspectorController()->show();
#endif
}

/*! \reimp */
void QWebInspector::hideEvent(QHideEvent* event)
{
}

/*! \reimp */
void QWebInspector::closeEvent(QCloseEvent* event)
{
#if ENABLE(INSPECTOR)
    if (d->page)
        d->page->d->inspectorController()->setWindowVisible(false);
#endif
}

/*! \internal */
void QWebInspectorPrivate::setFrontend(QWidget* newFrontend)
{
    if (frontend)
        frontend->setParent(0);

    frontend = newFrontend;

    if (frontend) {
        frontend->setParent(q);
        frontend->show();
        adjustFrontendSize(q->size());
    }
}

void QWebInspectorPrivate::adjustFrontendSize(const QSize& size)
{
    if (frontend)
        frontend->resize(size);
}

