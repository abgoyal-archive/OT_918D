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
 * Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies)
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "widget.h"

#include "qwebelement.h"
#include "qwebframe.h"
#include "ui_widget.h"
#include <QPainter>
#include <QtTest/QtTest>

Widget::Widget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

void Widget::refreshJS()
{
    ui->webView->page()->mainFrame()->addToJavaScriptWindowObject("myWidget", this);
}
void Widget::start()
{
    ui->webView->load(QUrl("qrc:///test.html"));
    connect(ui->webView->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(refreshJS()));
    ui->webView->page()->mainFrame()->addToJavaScriptWindowObject("myWidget", this);
}

void Widget::completeTest()
{
    QCOMPARE(ui->lbl1->pixmap()->size(), ui->lbl2->size());
    QCOMPARE(ui->lbl3->size(), ui->lbl4->pixmap()->size());
    QCOMPARE(ui->lbl2->size().width(), ui->webView->page()->mainFrame()->findFirstElement("#img1").evaluateJavaScript("this.width").toInt());
    QCOMPARE(ui->lbl3->size().width(), ui->webView->page()->mainFrame()->findFirstElement("#img2").evaluateJavaScript("this.width").toInt());
    emit testComplete();
}

void Widget::setPixmap(const QPixmap& p)
{
    ui->lbl1->setPixmap(p);
}
QPixmap Widget::pixmap() const
{
    QPixmap px(ui->lbl3->size());
    {
        QPainter p(&px);
        ui->lbl3->render(&p);
    }
    return px;
}
void Widget::setImage(const QImage& img)
{
    ui->lbl4->setPixmap(QPixmap::fromImage(img));
}

QImage Widget::image() const
{
    QImage img(ui->lbl2->size(), QImage::Format_ARGB32);
    {
        QPainter p(&img);
        ui->lbl2->render(&p);
    }
    return img;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::changeEvent(QEvent* e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
void Widget::compare(const QVariant& a, const QVariant& b)
{
    QCOMPARE(a, b);
}

void Widget::imageSlot(const QImage& img)
{
    QCOMPARE(img.size(), ui->lbl3->size());
    emit pixmapSignal(QPixmap::fromImage(img));
}

void Widget::pixmapSlot(const QPixmap& pxm)
{
    QCOMPARE(pxm.size(), ui->lbl2->size());
    emit imageSignal(ui->lbl4->pixmap()->toImage());
}

void Widget::randomSlot(const QPixmap& pxm)
{
    QVERIFY(pxm.isNull());
}
