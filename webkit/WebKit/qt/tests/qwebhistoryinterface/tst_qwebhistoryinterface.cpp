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
    Copyright (C) 2008 Holger Hans Peter Freyther

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


#include <QtTest/QtTest>

#include <qwebpage.h>
#include <qwebview.h>
#include <qwebframe.h>
#include <qwebhistoryinterface.h>
#include <QDebug>

class tst_QWebHistoryInterface : public QObject
{
    Q_OBJECT

public:
    tst_QWebHistoryInterface();
    virtual ~tst_QWebHistoryInterface();

public slots:
    void init();
    void cleanup();

private slots:
    void visitedLinks();

private:


private:
    QWebView* m_view;
    QWebPage* m_page;
};

tst_QWebHistoryInterface::tst_QWebHistoryInterface()
{
}

tst_QWebHistoryInterface::~tst_QWebHistoryInterface()
{
}

void tst_QWebHistoryInterface::init()
{
    m_view = new QWebView();
    m_page = m_view->page();
}

void tst_QWebHistoryInterface::cleanup()
{
    delete m_view;
}

class FakeHistoryImplementation : public QWebHistoryInterface {
public:
    void addHistoryEntry(const QString&) {}
    bool historyContains(const QString& url) const {
        return url == QLatin1String("http://www.trolltech.com/");
    }
};


/*
 * Test that visited links are properly colored. http://www.trolltech.com is marked
 * as visited, so the below website should have exactly one element in the a:visited
 * state.
 */
void tst_QWebHistoryInterface::visitedLinks()
{
    QWebHistoryInterface::setDefaultInterface(new FakeHistoryImplementation);
    m_view->setHtml("<html><body><a href='http://www.trolltech.com'>Trolltech</a></body></html>");
    QCOMPARE(m_page->mainFrame()->evaluateJavaScript("document.querySelectorAll(':visited').length;").toString(),
             QString::fromLatin1("1"));
}

QTEST_MAIN(tst_QWebHistoryInterface)
#include "tst_qwebhistoryinterface.moc"
