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
 *  Copyright (C) 2007 Holger Hans Peter Freyther
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
 */

#include "config.h"
#include "ContextMenu.h"
#include "ContextMenuItem.h"
#include "CString.h"
#include "NotImplemented.h"

#include <gtk/gtk.h>

#define WEBKIT_CONTEXT_MENU_ACTION "webkit-context-menu"

namespace WebCore {

static const char* gtkStockIDFromContextMenuAction(const ContextMenuAction& action)
{
    switch (action) {
    case ContextMenuItemTagCopyLinkToClipboard:
    case ContextMenuItemTagCopyImageToClipboard:
    case ContextMenuItemTagCopy:
        return GTK_STOCK_COPY;
    case ContextMenuItemTagOpenLinkInNewWindow:
    case ContextMenuItemTagOpenImageInNewWindow:
    case ContextMenuItemTagOpenFrameInNewWindow:
        return GTK_STOCK_OPEN;
    case ContextMenuItemTagDownloadLinkToDisk:
    case ContextMenuItemTagDownloadImageToDisk:
        return GTK_STOCK_SAVE;
    case ContextMenuItemTagGoBack:
        return GTK_STOCK_GO_BACK;
    case ContextMenuItemTagGoForward:
        return GTK_STOCK_GO_FORWARD;
    case ContextMenuItemTagStop:
        return GTK_STOCK_STOP;
    case ContextMenuItemTagReload:
        return GTK_STOCK_REFRESH;
    case ContextMenuItemTagCut:
        return GTK_STOCK_CUT;
    case ContextMenuItemTagPaste:
        return GTK_STOCK_PASTE;
    case ContextMenuItemTagDelete:
        return GTK_STOCK_DELETE;
    case ContextMenuItemTagSelectAll:
        return GTK_STOCK_SELECT_ALL;
    case ContextMenuItemTagSpellingGuess:
        return NULL;
    case ContextMenuItemTagIgnoreSpelling:
        return GTK_STOCK_NO;
    case ContextMenuItemTagLearnSpelling:
        return GTK_STOCK_OK;
    case ContextMenuItemTagOther:
        return GTK_STOCK_MISSING_IMAGE;
    case ContextMenuItemTagSearchInSpotlight:
        return GTK_STOCK_FIND;
    case ContextMenuItemTagSearchWeb:
        return GTK_STOCK_FIND;
    case ContextMenuItemTagOpenWithDefaultApplication:
        return GTK_STOCK_OPEN;
    case ContextMenuItemPDFZoomIn:
        return GTK_STOCK_ZOOM_IN;
    case ContextMenuItemPDFZoomOut:
        return GTK_STOCK_ZOOM_OUT;
    case ContextMenuItemPDFAutoSize:
        return GTK_STOCK_ZOOM_FIT;
    case ContextMenuItemPDFNextPage:
        return GTK_STOCK_GO_FORWARD;
    case ContextMenuItemPDFPreviousPage:
        return GTK_STOCK_GO_BACK;
    // New tags, not part of API
    case ContextMenuItemTagOpenLink:
        return GTK_STOCK_OPEN;
    case ContextMenuItemTagCheckSpelling:
        return GTK_STOCK_SPELL_CHECK;
    case ContextMenuItemTagFontMenu:
        return GTK_STOCK_SELECT_FONT;
    case ContextMenuItemTagShowFonts:
        return GTK_STOCK_SELECT_FONT;
    case ContextMenuItemTagBold:
        return GTK_STOCK_BOLD;
    case ContextMenuItemTagItalic:
        return GTK_STOCK_ITALIC;
    case ContextMenuItemTagUnderline:
        return GTK_STOCK_UNDERLINE;
    case ContextMenuItemTagShowColors:
        return GTK_STOCK_SELECT_COLOR;
    default:
        return NULL;
    }
}

// Extract the ActionType from the menu item
ContextMenuItem::ContextMenuItem(GtkMenuItem* item)
    : m_platformDescription()
{
    if (GTK_IS_SEPARATOR_MENU_ITEM(item))
        m_platformDescription.type = SeparatorType;
    else if (gtk_menu_item_get_submenu(item))
        m_platformDescription.type = SubmenuType;
    else if (GTK_IS_CHECK_MENU_ITEM(item)) {
        m_platformDescription.type = CheckableActionType;
        m_platformDescription.checked = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(item));
    } else
        m_platformDescription.type = ActionType;
#if GTK_CHECK_VERSION (2, 16, 0)
    m_platformDescription.title = String::fromUTF8(gtk_menu_item_get_label(GTK_MENU_ITEM(item)));
#else
    GtkWidget* label = gtk_bin_get_child(GTK_BIN(item));
    m_platformDescription.title = String::fromUTF8(gtk_label_get_label(GTK_LABEL(label)));
#endif

    m_platformDescription.action = *static_cast<ContextMenuAction*>(g_object_get_data(G_OBJECT(item), WEBKIT_CONTEXT_MENU_ACTION));

    m_platformDescription.subMenu = GTK_MENU(gtk_menu_item_get_submenu(item));
    if (m_platformDescription.subMenu)
        g_object_ref(m_platformDescription.subMenu);
}

ContextMenuItem::ContextMenuItem(ContextMenu*)
{
    notImplemented();
}

ContextMenuItem::ContextMenuItem(ContextMenuItemType type, ContextMenuAction action, const String& title, ContextMenu* subMenu)
{
    m_platformDescription.type = type;
    m_platformDescription.action = action;
    m_platformDescription.title = title;

    setSubMenu(subMenu);
}

ContextMenuItem::~ContextMenuItem()
{
    if (m_platformDescription.subMenu)
        g_object_unref(m_platformDescription.subMenu);
}

GtkMenuItem* ContextMenuItem::createNativeMenuItem(const PlatformMenuItemDescription& menu)
{
    GtkMenuItem* item = 0;
    if (menu.type == SeparatorType)
        item = GTK_MENU_ITEM(gtk_separator_menu_item_new());
    else {
        if (menu.type == CheckableActionType) {
            item = GTK_MENU_ITEM(gtk_check_menu_item_new_with_mnemonic(menu.title.utf8().data()));
            gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item), menu.checked);
        } else {
            if (const gchar* stockID = gtkStockIDFromContextMenuAction(menu.action)) {
                item = GTK_MENU_ITEM(gtk_image_menu_item_new_with_mnemonic(menu.title.utf8().data()));
                GtkWidget* image = gtk_image_new_from_stock(stockID, GTK_ICON_SIZE_MENU);
                gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(item), image);
            } else
                item = GTK_MENU_ITEM(gtk_menu_item_new_with_mnemonic(menu.title.utf8().data()));
        }

        ContextMenuAction* menuAction = static_cast<ContextMenuAction*>(malloc(sizeof(ContextMenuAction*)));
        *menuAction = menu.action;
        g_object_set_data(G_OBJECT(item), WEBKIT_CONTEXT_MENU_ACTION, menuAction);

        gtk_widget_set_sensitive(GTK_WIDGET(item), menu.enabled);

        if (menu.subMenu)
            gtk_menu_item_set_submenu(item, GTK_WIDGET(menu.subMenu));
    }

    return item;
}

PlatformMenuItemDescription ContextMenuItem::releasePlatformDescription()
{
    PlatformMenuItemDescription description = m_platformDescription;
    m_platformDescription = PlatformMenuItemDescription();
    return description;
}

ContextMenuItemType ContextMenuItem::type() const
{
    return m_platformDescription.type;
}

void ContextMenuItem::setType(ContextMenuItemType type)
{
    m_platformDescription.type = type;
}

ContextMenuAction ContextMenuItem::action() const
{
    return m_platformDescription.action;
}

void ContextMenuItem::setAction(ContextMenuAction action)
{
    m_platformDescription.action = action;
}

String ContextMenuItem::title() const
{
    return m_platformDescription.title;
}

void ContextMenuItem::setTitle(const String& title)
{
    m_platformDescription.title = title;
}

PlatformMenuDescription ContextMenuItem::platformSubMenu() const
{
    return m_platformDescription.subMenu;
}

void ContextMenuItem::setSubMenu(ContextMenu* menu)
{
    if (m_platformDescription.subMenu)
        g_object_unref(m_platformDescription.subMenu);

    if (!menu)
        return;

    m_platformDescription.subMenu = menu->releasePlatformDescription();
    m_platformDescription.type = SubmenuType;

    g_object_ref_sink(G_OBJECT(m_platformDescription.subMenu));
}

void ContextMenuItem::setChecked(bool shouldCheck)
{
    m_platformDescription.checked = shouldCheck;
}

void ContextMenuItem::setEnabled(bool shouldEnable)
{
    m_platformDescription.enabled = shouldEnable;
}

}
