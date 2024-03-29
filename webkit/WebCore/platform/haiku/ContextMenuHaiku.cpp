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
 * Copyright (C) 2006 Zack Rusin <zack@kde.org>
 * Copyright (C) 2007 Ryan Leavengood <leavengood@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"
#include "ContextMenu.h"

#include "ContextMenuController.h"
#include "ContextMenuItem.h"
#include "Document.h"
#include "Frame.h"
#include "FrameView.h"
#include <Looper.h>
#include <Menu.h>
#include <Message.h>
#include <wtf/Assertions.h>


namespace WebCore {

// FIXME: This class isn't used yet
class ContextMenuReceiver : public BLooper {
public:
    ContextMenuReceiver(ContextMenu* menu)
        : BLooper("context_menu_receiver")
        , m_menu(menu)
        , m_result(-1)
    {
    }

    void HandleMessage(BMessage* msg)
    {
        m_result = msg->what;
        if (m_result != -1) {
            BMenuItem* item = m_menu->platformDescription()->FindItem(m_result);
            if (!item) {
                printf("Error: Context menu item with code %i not found!\n", m_result);
                return;
            }
            ContextMenuItem cmItem(item);
            m_menu->controller()->contextMenuItemSelected(&cmItem);
            cmItem.releasePlatformDescription();
        }
    }

    int Result()
    {
        return m_result;
    }

private:
    ContextMenu* m_menu;
    int m_result;
};

ContextMenu::ContextMenu(const HitTestResult& result)
    : m_hitTestResult(result)
    , m_platformDescription(new BMenu("context_menu"))
{
}

ContextMenu::~ContextMenu()
{
    delete m_platformDescription;
}

void ContextMenu::appendItem(ContextMenuItem& item)
{
    checkOrEnableIfNeeded(item);

    BMenuItem* menuItem = item.releasePlatformDescription();
    if (menuItem)
        m_platformDescription->AddItem(menuItem);
}

unsigned ContextMenu::itemCount() const
{
    return m_platformDescription->CountItems();
}

void ContextMenu::insertItem(unsigned position, ContextMenuItem& item)
{
    checkOrEnableIfNeeded(item);

    BMenuItem* menuItem = item.releasePlatformDescription();
    if (menuItem)
        m_platformDescription->AddItem(menuItem, position);
}

PlatformMenuDescription ContextMenu::platformDescription() const
{
    return m_platformDescription;
}

void ContextMenu::setPlatformDescription(PlatformMenuDescription menu)
{
    if (static_cast<BMenu*>(menu) == m_platformDescription)
        return;

    delete m_platformDescription;
    m_platformDescription = static_cast<BMenu*>(menu);
}

} // namespace WebCore

