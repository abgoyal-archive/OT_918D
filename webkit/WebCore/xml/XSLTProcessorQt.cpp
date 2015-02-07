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
 * This file is part of the XSL implementation.
 *
 * Copyright (C) 2009 Jakub Wieczorek <faw217@gmail.com>
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

#if ENABLE(XSLT)

#include "XSLTProcessor.h"

#include "Console.h"
#include "DOMWindow.h"
#include "Frame.h"
#include "TransformSource.h"
#include "loader.h"
#include "markup.h"
#include <wtf/Assertions.h>
#include <wtf/Platform.h>
#include <wtf/Vector.h>

#include <qabstractmessagehandler.h>
#include <qabstracturiresolver.h>
#include <qbuffer.h>
#include <qsourcelocation.h>
#include <qxmlquery.h>

namespace WebCore {

class XSLTMessageHandler : public QAbstractMessageHandler {

public:
    XSLTMessageHandler(Document* document = 0);
    virtual void handleMessage(QtMsgType type, const QString& description,
                               const QUrl& identifier, const QSourceLocation& sourceLocation);

private:
    Document* m_document;
};

XSLTMessageHandler::XSLTMessageHandler(Document* document)
    : QAbstractMessageHandler()
    , m_document(document)
{
}

void XSLTMessageHandler::handleMessage(QtMsgType type, const QString& description,
                                       const QUrl&, const QSourceLocation& sourceLocation)
{
    if (!m_document->frame())
        return;

    MessageLevel level;
    switch (type) {
    case QtDebugMsg:
        level = TipMessageLevel;
        break;
    case QtWarningMsg:
        level = WarningMessageLevel;
        break;
    case QtCriticalMsg:
    case QtFatalMsg:
        level = ErrorMessageLevel;
        break;
    default:
        level = LogMessageLevel;
        break;
    }

    Console* console = m_document->frame()->domWindow()->console();
    console->addMessage(XMLMessageSource, LogMessageType, level, description,
                        sourceLocation.line(), sourceLocation.uri().toString());
}

class XSLTUriResolver : public QAbstractUriResolver {

public:
    XSLTUriResolver(Document* document);
    virtual QUrl resolve(const QUrl& relative, const QUrl& baseURI) const;

private:
    Document* m_document;
};

XSLTUriResolver::XSLTUriResolver(Document* document)
    : QAbstractUriResolver()
    , m_document(document)
{
}

QUrl XSLTUriResolver::resolve(const QUrl& relative, const QUrl& baseURI) const
{
    QUrl url = baseURI.resolved(relative);

    if (!m_document->frame() || !m_document->securityOrigin()->canRequest(url))
        return QUrl();
    return url;
}

bool XSLTProcessor::transformToString(Node* sourceNode, String&, String& resultString, String&)
{
    bool success = false;

    RefPtr<XSLStyleSheet> stylesheet = m_stylesheet;
    if (!stylesheet && m_stylesheetRootNode) {
        Node* node = m_stylesheetRootNode.get();
        stylesheet = XSLStyleSheet::create(node->parent() ? node->parent() : node,
            node->document()->url().string(),
            node->document()->url()); // FIXME: Should we use baseURL here?
        stylesheet->parseString(createMarkup(node));
    }

    if (!stylesheet || stylesheet->sheetString().isEmpty())
        return success;

    RefPtr<Document> ownerDocument = sourceNode->document();
    bool sourceIsDocument = (sourceNode == ownerDocument.get());

    QXmlQuery query(QXmlQuery::XSLT20);

    XSLTMessageHandler messageHandler(ownerDocument.get());
    XSLTUriResolver uriResolver(ownerDocument.get());
    query.setMessageHandler(&messageHandler);

    XSLTProcessor::ParameterMap::iterator end = m_parameters.end();
    for (XSLTProcessor::ParameterMap::iterator it = m_parameters.begin(); it != end; ++it)
        query.bindVariable(QString(it->first), QXmlItem(QVariant(it->second)));

    QString source;
    if (sourceIsDocument && ownerDocument->transformSource())
        source = ownerDocument->transformSource()->platformSource();
    if (!sourceIsDocument || source.isEmpty())
        source = createMarkup(sourceNode);

    QBuffer inputBuffer;
    QBuffer styleSheetBuffer;
    QBuffer outputBuffer;

    inputBuffer.setData(source.toUtf8());
    styleSheetBuffer.setData(QString(stylesheet->sheetString()).toUtf8());

    inputBuffer.open(QIODevice::ReadOnly);
    styleSheetBuffer.open(QIODevice::ReadOnly);
    outputBuffer.open(QIODevice::ReadWrite);

    query.setFocus(&inputBuffer);
    query.setQuery(&styleSheetBuffer, QUrl(stylesheet->href()));

    query.setUriResolver(&uriResolver);

    success = query.evaluateTo(&outputBuffer);
    outputBuffer.reset();
    resultString = QString::fromUtf8(outputBuffer.readAll()).trimmed();

    if (m_stylesheet) {
        m_stylesheet->clearDocuments();
        m_stylesheet = 0;
    }

    return success;
}

} // namespace WebCore

#endif // ENABLE(XSLT)
