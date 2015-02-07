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
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2000 Frederik Holljen (frederik.holljen@hig.no)
 * Copyright (C) 2001 Peter Kelly (pmk@post.com)
 * Copyright (C) 2006 Samuel Weinig (sam.weinig@gmail.com)
 * Copyright (C) 2004, 2008 Apple Inc. All rights reserved.
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

#include "config.h"
#include "TreeWalker.h"

#include "ExceptionCode.h"
#include "ContainerNode.h"
#include "NodeFilter.h"
#include "ScriptState.h"
#include <wtf/PassRefPtr.h>

namespace WebCore {

TreeWalker::TreeWalker(PassRefPtr<Node> rootNode, unsigned whatToShow, PassRefPtr<NodeFilter> filter, bool expandEntityReferences)
    : Traversal(rootNode, whatToShow, filter, expandEntityReferences)
    , m_current(root())
{
}

void TreeWalker::setCurrentNode(PassRefPtr<Node> node, ExceptionCode& ec)
{
    if (!node) {
        ec = NOT_SUPPORTED_ERR;
        return;
    }
    m_current = node;
}

inline Node* TreeWalker::setCurrent(PassRefPtr<Node> node)
{
    m_current = node;
    return m_current.get();
}

Node* TreeWalker::parentNode(ScriptState* state)
{
    RefPtr<Node> node = m_current;
    while (node != root()) {
        node = node->parentNode();
        if (!node)
            return 0;
        short acceptNodeResult = acceptNode(state, node.get());
        if (state && state->hadException())
            return 0;
        if (acceptNodeResult == NodeFilter::FILTER_ACCEPT)
            return setCurrent(node.release());
    }
    return 0;
}

Node* TreeWalker::firstChild(ScriptState* state)
{
    for (RefPtr<Node> node = m_current->firstChild(); node; ) {
        short acceptNodeResult = acceptNode(state, node.get());
        if (state && state->hadException())
            return 0;
        switch (acceptNodeResult) {
            case NodeFilter::FILTER_ACCEPT:
                m_current = node.release();
                return m_current.get();
            case NodeFilter::FILTER_SKIP:
                if (node->firstChild()) {
                    node = node->firstChild();
                    continue;
                }
                break;
            case NodeFilter::FILTER_REJECT:
                break;
        }
        do {
            if (node->nextSibling()) {
                node = node->nextSibling();
                break;
            }
            Node* parent = node->parentNode();
            if (!parent || parent == root() || parent == m_current)
                return 0;
            node = parent;
        } while (node);
    }
    return 0;
}

Node* TreeWalker::lastChild(ScriptState* state)
{
    for (RefPtr<Node> node = m_current->lastChild(); node; ) {
        short acceptNodeResult = acceptNode(state, node.get());
        if (state && state->hadException())
            return 0;
        switch (acceptNodeResult) {
            case NodeFilter::FILTER_ACCEPT:
                m_current = node.release();
                return m_current.get();
            case NodeFilter::FILTER_SKIP:
                if (node->lastChild()) {
                    node = node->lastChild();
                    continue;
                }
                break;
            case NodeFilter::FILTER_REJECT:
                break;
        }
        do {
            if (node->previousSibling()) {
                node = node->previousSibling();
                break;
            }
            Node* parent = node->parentNode();
            if (!parent || parent == root() || parent == m_current)
                return 0;
            node = parent;
        } while (node);
    }
    return 0;
}

Node* TreeWalker::previousSibling(ScriptState* state)
{
    RefPtr<Node> node = m_current;
    if (node == root())
        return 0;
    while (1) {
        for (RefPtr<Node> sibling = node->previousSibling(); sibling; ) {
            short acceptNodeResult = acceptNode(state, sibling.get());
            if (state && state->hadException())
                return 0;
            switch (acceptNodeResult) {
                case NodeFilter::FILTER_ACCEPT:
                    m_current = sibling.release();
                    return m_current.get();
                case NodeFilter::FILTER_SKIP:
                    if (sibling->firstChild()) {
                        sibling = sibling->firstChild();
                        continue;
                    }
                    break;
                case NodeFilter::FILTER_REJECT:
                    break;
            }
            sibling = sibling->previousSibling();
        }
        node = node->parentNode();
        if (!node || node == root())
            return 0;
        short acceptNodeResult = acceptNode(state, node.get());
        if (state && state->hadException())
            return 0;
        if (acceptNodeResult == NodeFilter::FILTER_ACCEPT)
            return 0;
    }
}

Node* TreeWalker::nextSibling(ScriptState* state)
{
    RefPtr<Node> node = m_current;
    if (node == root())
        return 0;
    while (1) {
        for (RefPtr<Node> sibling = node->nextSibling(); sibling; ) {
            short acceptNodeResult = acceptNode(state, sibling.get());
            if (state && state->hadException())
                return 0;
            switch (acceptNodeResult) {
                case NodeFilter::FILTER_ACCEPT:
                    m_current = sibling.release();
                    return m_current.get();
                case NodeFilter::FILTER_SKIP:
                    if (sibling->firstChild()) {
                        sibling = sibling->firstChild();
                        continue;
                    }
                    break;
                case NodeFilter::FILTER_REJECT:
                    break;
            }
            sibling = sibling->nextSibling();
        }
        node = node->parentNode();
        if (!node || node == root())
            return 0;
        short acceptNodeResult = acceptNode(state, node.get());
        if (state && state->hadException())
            return 0;
        if (acceptNodeResult == NodeFilter::FILTER_ACCEPT)
            return 0;
    }
}

Node* TreeWalker::previousNode(ScriptState* state)
{
    RefPtr<Node> node = m_current;
    while (node != root()) {
        while (Node* previousSibling = node->previousSibling()) {
            node = previousSibling;
            short acceptNodeResult = acceptNode(state, node.get());
            if (state && state->hadException())
                return 0;
            if (acceptNodeResult == NodeFilter::FILTER_REJECT)
                continue;
            while (Node* lastChild = node->lastChild()) {
                node = lastChild;
                acceptNodeResult = acceptNode(state, node.get());
                if (state && state->hadException())
                    return 0;
                if (acceptNodeResult == NodeFilter::FILTER_ACCEPT)
                    continue;
            }
            if (acceptNodeResult == NodeFilter::FILTER_ACCEPT) {
                m_current = node.release();
                return m_current.get();
            }
        }
        if (node == root())
            return 0;
        Node* parent = node->parentNode();
        if (!parent)
            return 0;
        node = parent;
        short acceptNodeResult = acceptNode(state, node.get());
        if (state && state->hadException())
            return 0;
        if (acceptNodeResult == NodeFilter::FILTER_ACCEPT)
            return setCurrent(node.release());
    }
    return 0;
}

Node* TreeWalker::nextNode(ScriptState* state)
{
    RefPtr<Node> node = m_current;
Children:
    while (Node* firstChild = node->firstChild()) {
        node = firstChild;
        short acceptNodeResult = acceptNode(state, node.get());
        if (state && state->hadException())
            return 0;
        if (acceptNodeResult == NodeFilter::FILTER_ACCEPT)
            return setCurrent(node.release());
        if (acceptNodeResult == NodeFilter::FILTER_REJECT)
            break;
    }
    while (Node* nextSibling = node->traverseNextSibling(root())) {
        node = nextSibling;
        short acceptNodeResult = acceptNode(state, node.get());
        if (state && state->hadException())
            return 0;
        if (acceptNodeResult == NodeFilter::FILTER_ACCEPT)
            return setCurrent(node.release());
        if (acceptNodeResult == NodeFilter::FILTER_SKIP)
            goto Children;
    }
    return 0;
}

} // namespace WebCore
