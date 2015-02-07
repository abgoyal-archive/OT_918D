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
 *  Copyright (C) 2007 Alp Toker <alp@atoker.com>
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
#include "Pasteboard.h"

#include "CString.h"
#include "DocumentFragment.h"
#include "Frame.h"
#include "NotImplemented.h"
#include "PlatformString.h"
#include "TextResourceDecoder.h"
#include "Image.h"
#include "RenderImage.h"
#include "KURL.h"
#include "markup.h"

#include <gtk/gtk.h>

namespace WebCore {

class PasteboardSelectionData {
public:
    PasteboardSelectionData(gchar* text, gchar* markup)
        : m_text(text)
        , m_markup(markup) { }

    ~PasteboardSelectionData() {
        g_free(m_text);
        g_free(m_markup);
    }

    const gchar* text() const { return m_text; }
    const gchar* markup() const { return m_markup; }

private:
    gchar* m_text;
    gchar* m_markup;
};

static void clipboard_get_contents_cb(GtkClipboard *clipboard, GtkSelectionData *selection_data,
                                      guint info, gpointer data) {
    PasteboardSelectionData* clipboardData = reinterpret_cast<PasteboardSelectionData*>(data);
    ASSERT(clipboardData);
    if ((gint)info == Pasteboard::generalPasteboard()->m_helper->getWebViewTargetInfoHtml())
        gtk_selection_data_set(selection_data, selection_data->target, 8,
                               reinterpret_cast<const guchar*>(clipboardData->markup()),
                               g_utf8_strlen(clipboardData->markup(), -1));
    else
        gtk_selection_data_set_text(selection_data, clipboardData->text(), -1);
}

static void clipboard_clear_contents_cb(GtkClipboard *clipboard, gpointer data) {
    PasteboardSelectionData* clipboardData = reinterpret_cast<PasteboardSelectionData*>(data);
    ASSERT(clipboardData);
    delete clipboardData;
}


Pasteboard* Pasteboard::generalPasteboard()
{
    static Pasteboard* pasteboard = new Pasteboard();
    return pasteboard;
}

Pasteboard::Pasteboard()
{
    notImplemented();
}

Pasteboard::~Pasteboard()
{
    delete m_helper;
}

void Pasteboard::setHelper(PasteboardHelper* helper)
{
    m_helper = helper;
}

void Pasteboard::writeSelection(Range* selectedRange, bool canSmartCopyOrDelete, Frame* frame)
{
    GtkClipboard* clipboard = m_helper->getClipboard(frame);
    gchar* text = g_strdup(frame->selectedText().utf8().data());
    gchar* markup = g_strdup(createMarkup(selectedRange, 0, AnnotateForInterchange).utf8().data());
    PasteboardSelectionData* data = new PasteboardSelectionData(text, markup);

    gint n_targets;
    GtkTargetEntry* targets = gtk_target_table_new_from_list(m_helper->targetList(), &n_targets);
    gtk_clipboard_set_with_data(clipboard, targets, n_targets,
                                clipboard_get_contents_cb, clipboard_clear_contents_cb, data);
    gtk_target_table_free(targets, n_targets);
}

void Pasteboard::writePlainText(const String& text)
{
    CString utf8 = text.utf8();
    GtkClipboard* clipboard = gtk_clipboard_get_for_display(gdk_display_get_default(), GDK_SELECTION_CLIPBOARD);
    gtk_clipboard_set_text(clipboard, utf8.data(), utf8.length());
}

void Pasteboard::writeURL(const KURL& url, const String&, Frame* frame)
{
    if (url.isEmpty())
        return;

    GtkClipboard* clipboard = m_helper->getClipboard(frame);
    GtkClipboard* primary = m_helper->getPrimary(frame);
    CString utf8 = url.string().utf8();
    gtk_clipboard_set_text(clipboard, utf8.data(), utf8.length());
    gtk_clipboard_set_text(primary, utf8.data(), utf8.length());
}

void Pasteboard::writeImage(Node* node, const KURL&, const String&)
{
    GtkClipboard* clipboard = gtk_clipboard_get_for_display(gdk_display_get_default(), GDK_SELECTION_CLIPBOARD);

    ASSERT(node && node->renderer() && node->renderer()->isImage());
    RenderImage* renderer = toRenderImage(node->renderer());
    CachedImage* cachedImage = renderer->cachedImage();
    ASSERT(cachedImage);
    Image* image = cachedImage->image();
    ASSERT(image);

    GdkPixbuf* pixbuf = image->getGdkPixbuf();
    gtk_clipboard_set_image(clipboard, pixbuf);
    g_object_unref(pixbuf);
}

void Pasteboard::clear()
{
    GtkClipboard* clipboard = gtk_clipboard_get_for_display(gdk_display_get_default(), GDK_SELECTION_CLIPBOARD);

    gtk_clipboard_clear(clipboard);
}

bool Pasteboard::canSmartReplace()
{
    notImplemented();
    return false;
}

PassRefPtr<DocumentFragment> Pasteboard::documentFragment(Frame* frame, PassRefPtr<Range> context,
                                                          bool allowPlainText, bool& chosePlainText)
{
    GdkAtom textHtml = gdk_atom_intern_static_string("text/html");
    GtkClipboard* clipboard = m_helper->getCurrentTarget(frame);
    chosePlainText = false;

    if (GtkSelectionData* data = gtk_clipboard_wait_for_contents(clipboard, textHtml)) {
        ASSERT(data->data);
        RefPtr<TextResourceDecoder> decoder = TextResourceDecoder::create("text/plain", "UTF-8", true);
        String html = decoder->decode(reinterpret_cast<char*>(data->data), data->length);
        html += decoder->flush();
        gtk_selection_data_free(data);

        if (!html.isEmpty()) {
            RefPtr<DocumentFragment> fragment = createFragmentFromMarkup(frame->document(), html, "", FragmentScriptingNotAllowed);
            if (fragment)
                return fragment.release();
        }
    }

    if (!allowPlainText)
        return 0;

    if (gchar* utf8 = gtk_clipboard_wait_for_text(clipboard)) {
        String text = String::fromUTF8(utf8);
        g_free(utf8);

        chosePlainText = true;
        RefPtr<DocumentFragment> fragment = createFragmentFromText(context.get(), text);
        if (fragment)
            return fragment.release();
    }

    return 0;
}

String Pasteboard::plainText(Frame* frame)
{
    GtkClipboard* clipboard = m_helper->getCurrentTarget(frame);

    gchar* utf8 = gtk_clipboard_wait_for_text(clipboard);

    if (!utf8)
        return String();

    String text = String::fromUTF8(utf8);
    g_free(utf8);

    return text;
}

}
