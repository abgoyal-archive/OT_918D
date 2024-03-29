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
 * Copyright (C) 2006, 2007 Apple Inc.  All rights reserved.
 * Copyright (C) 2008 Collabora Ltd. All rights reserved.
 * Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) 2009 Girish Ramakrishnan <girish@forwardbias.in>
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

#ifndef __LP64__

#include "config.h"
#include "PluginView.h"

#include "Bridge.h"
#include "Document.h"
#include "DocumentLoader.h"
#include "Element.h"
#include "EventNames.h"
#include "FocusController.h"
#include "FrameLoader.h"
#include "FrameLoadRequest.h"
#include "FrameTree.h"
#include "Frame.h"
#include "FrameView.h"
#include "GraphicsContext.h"
#include "HostWindow.h"
#include "HTMLNames.h"
#include "HTMLPlugInElement.h"
#include "Image.h"
#include "JSDOMBinding.h"
#include "KeyboardEvent.h"
#include "MouseEvent.h"
#include "NotImplemented.h"
#include "Page.h"
#include "PlatformMouseEvent.h"
#include "PlatformKeyboardEvent.h"
#include "PluginDebug.h"
#include "PluginPackage.h"
#include "PluginMainThreadScheduler.h"
#include "RenderLayer.h"
#include "ScriptController.h"
#include "Settings.h"
#include "npruntime_impl.h"
#include "runtime_root.h"
#include <runtime/JSLock.h>
#include <runtime/JSValue.h>
#include <wtf/RetainPtr.h>


using JSC::ExecState;
using JSC::Interpreter;
using JSC::JSLock;
using JSC::JSObject;
using JSC::JSValue;
using JSC::UString;

#if PLATFORM(QT)
#include <QWidget>
#include <QKeyEvent>
#include <QPainter>
#include "QWebPageClient.h"
QT_BEGIN_NAMESPACE
#if QT_VERSION < 0x040500
extern Q_GUI_EXPORT WindowPtr qt_mac_window_for(const QWidget* w);
#else
extern Q_GUI_EXPORT OSWindowRef qt_mac_window_for(const QWidget* w);
#endif
QT_END_NAMESPACE
#endif

#if PLATFORM(WX)
#include <wx/defs.h>
#include <wx/wx.h>
#endif

using std::min;

using namespace WTF;

namespace WebCore {

using namespace HTMLNames;

static int modifiersForEvent(UIEventWithKeyState *event);

static inline WindowRef nativeWindowFor(PlatformWidget widget)
{
#if PLATFORM(QT)
    if (widget)
        return static_cast<WindowRef>(qt_mac_window_for(widget));
#endif
#if PLATFORM(WX)
    if (widget)
        return (WindowRef)widget->MacGetTopLevelWindowRef();
#endif
    return 0;
}

static inline CGContextRef cgHandleFor(PlatformWidget widget)
{
#if PLATFORM(QT)
    if (widget)
        return (CGContextRef)widget->macCGHandle();
#endif
#if PLATFORM(WX)
    if (widget)
        return (CGContextRef)widget->MacGetCGContextRef();
#endif
    return 0;
}

static inline IntPoint topLevelOffsetFor(PlatformWidget widget)
{
#if PLATFORM(QT)
    if (widget) {
        PlatformWidget topLevel = widget->window();
        return widget->mapTo(topLevel, QPoint(0, 0)) + topLevel->geometry().topLeft() - topLevel->pos();
    }
#endif
#if PLATFORM(WX)
    if (widget) {
        PlatformWidget toplevel = wxGetTopLevelParent(widget);
        return toplevel->ScreenToClient(widget->GetScreenPosition());
    }
#endif
    return IntPoint();
}

// --------------- Lifetime management -----------------

bool PluginView::platformStart()
{
    ASSERT(m_isStarted);
    ASSERT(m_status == PluginStatusLoadedSuccessfully);

    if (m_drawingModel == NPDrawingModel(-1)) {
        // We default to QuickDraw, even though we don't support it,
        // since that's what Safari does, and some plugins expect this
        // behavior and never set the drawing model explicitly.
#ifndef NP_NO_QUICKDRAW
        m_drawingModel = NPDrawingModelQuickDraw;
#else
        // QuickDraw not available, so we have to default to CoreGraphics
        m_drawingModel = NPDrawingModelCoreGraphics;
#endif
    }

    if (m_eventModel == NPEventModel(-1)) {
        // If the plug-in did not specify an event model
        // we default to Carbon, when it is available.
#ifndef NP_NO_CARBON
        m_eventModel = NPEventModelCarbon;
#else
        m_eventModel = NPEventModelCocoa;
#endif
    }

    // Gracefully handle unsupported drawing or event models. We can do this
    // now since the drawing and event model can only be set during NPP_New.
    NPBool eventModelSupported, drawingModelSupported;
    if (getValueStatic(NPNVariable(NPNVsupportsCarbonBool + m_eventModel), &eventModelSupported) != NPERR_NO_ERROR
            || !eventModelSupported) {
        m_status = PluginStatusCanNotLoadPlugin;
        LOG(Plugins, "Plug-in '%s' uses unsupported event model %s",
                m_plugin->name().utf8().data(), prettyNameForEventModel(m_eventModel));
        return false;
    }

    if (getValueStatic(NPNVariable(NPNVsupportsQuickDrawBool + m_drawingModel), &drawingModelSupported) != NPERR_NO_ERROR
            || !drawingModelSupported) {
        m_status = PluginStatusCanNotLoadPlugin;
        LOG(Plugins, "Plug-in '%s' uses unsupported drawing model %s",
                m_plugin->name().utf8().data(), prettyNameForDrawingModel(m_drawingModel));
        return false;
    }

#if PLATFORM(QT)
    // Set the platformPluginWidget only in the case of QWebView so that the context menu appears in the right place.
    // In all other cases, we use off-screen rendering
    if (QWebPageClient* client = m_parentFrame->view()->hostWindow()->platformPageClient()) {
        if (QWidget* widget = qobject_cast<QWidget*>(client->pluginParent()))
            setPlatformPluginWidget(widget);
    }
#endif
#if PLATFORM(WX)
    if (wxWindow* widget = m_parentFrame->view()->hostWindow()->platformPageClient())
        setPlatformPluginWidget(widget);
#endif

    // Create a fake window relative to which all events will be sent when using offscreen rendering
    if (!platformPluginWidget()) {
        // Make the default size really big. It is unclear why this is required but with a smaller size, mouse move
        // events don't get processed. Resizing the fake window to flash's size doesn't help.
        ::Rect windowBounds = { 0, 0, 1000, 1000 };
        CreateNewWindow(kDocumentWindowClass, kWindowStandardDocumentAttributes, &windowBounds, &m_fakeWindow);
        // Flash requires the window to be hilited to process mouse move events.
        HiliteWindow(m_fakeWindow, true); 
    }

    show();

    return true;
}

void PluginView::platformDestroy()
{
    if (platformPluginWidget())
        setPlatformPluginWidget(0);
    else {
        CGContextRelease(m_contextRef);
        if (m_fakeWindow)
            DisposeWindow(m_fakeWindow);
    }
}

// Used before the plugin view has been initialized properly, and as a
// fallback for variables that do not require a view to resolve.
NPError PluginView::getValueStatic(NPNVariable variable, void* value)
{
    LOG(Plugins, "PluginView::getValueStatic(%s)", prettyNameForNPNVariable(variable).data());

    switch (variable) {
    case NPNVToolkit:
        *static_cast<uint32*>(value) = 0;
        return NPERR_NO_ERROR;

    case NPNVjavascriptEnabledBool:
        *static_cast<NPBool*>(value) = true;
        return NPERR_NO_ERROR;

#ifndef NP_NO_CARBON
    case NPNVsupportsCarbonBool:
        *static_cast<NPBool*>(value) = true;
        return NPERR_NO_ERROR;

#endif
    case NPNVsupportsCocoaBool:
        *static_cast<NPBool*>(value) = false;
        return NPERR_NO_ERROR;

    // CoreGraphics is the only drawing model we support
    case NPNVsupportsCoreGraphicsBool:
        *static_cast<NPBool*>(value) = true;
        return NPERR_NO_ERROR;

#ifndef NP_NO_QUICKDRAW
    // QuickDraw is deprecated in 10.5 and not supported on 64-bit
    case NPNVsupportsQuickDrawBool:
#endif
    case NPNVsupportsOpenGLBool:
    case NPNVsupportsCoreAnimationBool:
        *static_cast<NPBool*>(value) = false;
        return NPERR_NO_ERROR;

    default:
        return NPERR_GENERIC_ERROR;
    }
}

// Used only for variables that need a view to resolve
NPError PluginView::getValue(NPNVariable variable, void* value)
{
    LOG(Plugins, "PluginView::getValue(%s)", prettyNameForNPNVariable(variable).data());

    switch (variable) {
    case NPNVWindowNPObject: {
        if (m_isJavaScriptPaused)
            return NPERR_GENERIC_ERROR;

        NPObject* windowScriptObject = m_parentFrame->script()->windowScriptNPObject();

        // Return value is expected to be retained, as described in
        // <http://www.mozilla.org/projects/plugin/npruntime.html>
        if (windowScriptObject)
            _NPN_RetainObject(windowScriptObject);

        void** v = (void**)value;
        *v = windowScriptObject;

        return NPERR_NO_ERROR;
    }

    case NPNVPluginElementNPObject: {
        if (m_isJavaScriptPaused)
            return NPERR_GENERIC_ERROR;

        NPObject* pluginScriptObject = 0;

        if (m_element->hasTagName(appletTag) || m_element->hasTagName(embedTag) || m_element->hasTagName(objectTag))
            pluginScriptObject = static_cast<HTMLPlugInElement*>(m_element)->getNPObject();

        // Return value is expected to be retained, as described in
        // <http://www.mozilla.org/projects/plugin/npruntime.html>
        if (pluginScriptObject)
            _NPN_RetainObject(pluginScriptObject);

        void** v = (void**)value;
        *v = pluginScriptObject;

        return NPERR_NO_ERROR;
    }

    default:
        return getValueStatic(variable, value);
    }

}
void PluginView::setParent(ScrollView* parent)
{
    LOG(Plugins, "PluginView::setParent(%p)", parent);

    Widget::setParent(parent);

    if (parent)
        init();
}

// -------------- Geometry and painting ----------------

void PluginView::show()
{
    LOG(Plugins, "PluginView::show()");

    setSelfVisible(true);

    Widget::show();
}

void PluginView::hide()
{
    LOG(Plugins, "PluginView::hide()");

    setSelfVisible(false);

    Widget::hide();
}

void PluginView::setFocus()
{
    LOG(Plugins, "PluginView::setFocus()");

    if (platformPluginWidget())
#if PLATFORM(QT)
       platformPluginWidget()->setFocus(Qt::OtherFocusReason);
#else
        platformPluginWidget()->SetFocus();
#endif
   else
       Widget::setFocus();

    // TODO: Also handle and pass on blur events (focus lost)

    EventRecord record;
    record.what = getFocusEvent;
    record.message = 0;
    record.when = TickCount();
    record.where = globalMousePosForPlugin();
    record.modifiers = GetCurrentKeyModifiers();

    if (!dispatchNPEvent(record))
        LOG(Events, "PluginView::setFocus(): Get-focus event not accepted");
}

void PluginView::setParentVisible(bool visible)
{
    if (isParentVisible() == visible)
        return;

    Widget::setParentVisible(visible);
}

void PluginView::setNPWindowRect(const IntRect&)
{
    setNPWindowIfNeeded();
}

void PluginView::setNPWindowIfNeeded()
{
    if (!m_isStarted || !parent() || !m_plugin->pluginFuncs()->setwindow)
        return;

    CGContextRef newContextRef = 0;
    WindowRef newWindowRef = 0;
    if (platformPluginWidget()) {
        newContextRef = cgHandleFor(platformPluginWidget());
        newWindowRef = nativeWindowFor(platformPluginWidget());
        m_npWindow.type = NPWindowTypeWindow;
    } else {
        newContextRef = m_contextRef;
        newWindowRef = m_fakeWindow;
        m_npWindow.type = NPWindowTypeDrawable;
    }

    if (!newContextRef || !newWindowRef)
        return;

    m_npWindow.window = (void*)&m_npCgContext;
    m_npCgContext.window = newWindowRef;
    m_npCgContext.context = newContextRef;

    m_npWindow.x = m_windowRect.x();
    m_npWindow.y = m_windowRect.y();
    m_npWindow.width = m_windowRect.width();
    m_npWindow.height = m_windowRect.height();

    // TODO: (also clip against scrollbars, etc.)
    m_npWindow.clipRect.left = max(0, m_windowRect.x());
    m_npWindow.clipRect.top = max(0, m_windowRect.y());
    m_npWindow.clipRect.right = m_windowRect.x() + m_windowRect.width();
    m_npWindow.clipRect.bottom = m_windowRect.y() + m_windowRect.height();

    LOG(Plugins, "PluginView::setNPWindowIfNeeded(): window=%p, context=%p,"
            " window.x:%ld window.y:%ld window.width:%d window.height:%d window.clipRect size:%dx%d",
            newWindowRef, newContextRef, m_npWindow.x, m_npWindow.y, m_npWindow.width, m_npWindow.height,
            m_npWindow.clipRect.right - m_npWindow.clipRect.left, m_npWindow.clipRect.bottom - m_npWindow.clipRect.top);

    PluginView::setCurrentPluginView(this);
    JSC::JSLock::DropAllLocks dropAllLocks(JSC::SilenceAssertionsOnly);
    setCallingPlugin(true);
    m_plugin->pluginFuncs()->setwindow(m_instance, &m_npWindow);
    setCallingPlugin(false);
    PluginView::setCurrentPluginView(0);
}

void PluginView::updatePluginWidget()
{
    if (!parent())
       return;

    ASSERT(parent()->isFrameView());
    FrameView* frameView = static_cast<FrameView*>(parent());

    IntRect oldWindowRect = m_windowRect;
    IntRect oldClipRect = m_clipRect;

    m_windowRect = frameView->contentsToWindow(frameRect());
    IntPoint offset = topLevelOffsetFor(platformPluginWidget());
    m_windowRect.move(offset.x(), offset.y());

    if (!platformPluginWidget()) {
        if (m_windowRect.size() != oldWindowRect.size()) {
            CGContextRelease(m_contextRef);
#if PLATFORM(QT)
            m_pixmap = QPixmap(m_windowRect.size());
            m_pixmap.fill(Qt::transparent);
            m_contextRef = qt_mac_cg_context(&m_pixmap);
#endif
        }
    }

    m_clipRect = windowClipRect();
    m_clipRect.move(-m_windowRect.x(), -m_windowRect.y());

    if (platformPluginWidget() && (m_windowRect != oldWindowRect || m_clipRect != oldClipRect))
        setNPWindowIfNeeded();
}

void PluginView::paint(GraphicsContext* context, const IntRect& rect)
{
    if (!m_isStarted || m_status != PluginStatusLoadedSuccessfully) {
        paintMissingPluginIcon(context, rect);
        return;
    }

    if (context->paintingDisabled())
        return;

    setNPWindowIfNeeded();

    CGContextRef cgContext = m_npCgContext.context;
    if (!cgContext)
        return;

    CGContextSaveGState(cgContext);
    if (platformPluginWidget()) {
        IntPoint offset = frameRect().location();
        CGContextTranslateCTM(cgContext, offset.x(), offset.y());
    }

    IntRect targetRect(frameRect());
    targetRect.intersects(rect);

    // clip the context so that plugin only updates the interested area.
    CGRect r;
    r.origin.x = targetRect.x() - frameRect().x();
    r.origin.y = targetRect.y() - frameRect().y();
    r.size.width = targetRect.width();
    r.size.height = targetRect.height();
    CGContextClipToRect(cgContext, r);

    if (!platformPluginWidget() && m_isTransparent) { // clean the pixmap in transparent mode
#if PLATFORM(QT)
        QPainter painter(&m_pixmap);
        painter.setCompositionMode(QPainter::CompositionMode_Clear);
        painter.fillRect(QRectF(r.origin.x, r.origin.y, r.size.width, r.size.height), Qt::transparent);
#endif
    }

    EventRecord event;
    event.what = updateEvt;
    event.message = (long unsigned int)m_npCgContext.window;
    event.when = TickCount();
    event.where.h = 0;
    event.where.v = 0;
    event.modifiers = GetCurrentKeyModifiers();

    if (!dispatchNPEvent(event))
        LOG(Events, "PluginView::paint(): Paint event not accepted");

    CGContextRestoreGState(cgContext);

    if (!platformPluginWidget()) {
#if PLATFORM(QT)
        QPainter* painter = context->platformContext();
        painter->drawPixmap(targetRect.x(), targetRect.y(), m_pixmap, 
                            targetRect.x() - frameRect().x(), targetRect.y() - frameRect().y(), targetRect.width(), targetRect.height());
#endif
    }
}

void PluginView::invalidateRect(const IntRect& rect)
{
    if (platformPluginWidget())
#if PLATFORM(QT)
        platformPluginWidget()->update(convertToContainingWindow(rect));
#else
        platformPluginWidget()->RefreshRect(convertToContainingWindow(rect));
#endif
    else
        invalidateWindowlessPluginRect(rect);
}

void PluginView::invalidateRect(NPRect* rect)
{
    IntRect r(rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top);
    invalidateRect(r);
}

void PluginView::invalidateRegion(NPRegion region)
{
    // TODO: optimize
    invalidate();
}

void PluginView::forceRedraw()
{
    notImplemented();
}


// ----------------- Event handling --------------------

void PluginView::handleMouseEvent(MouseEvent* event)
{
    if (!m_isStarted)
        return;

    EventRecord record;

    if (event->type() == eventNames().mousemoveEvent) {
        record.what = nullEvent;
    } else if (event->type() == eventNames().mouseoverEvent) {
        record.what = adjustCursorEvent;
    } else if (event->type() == eventNames().mouseoutEvent) {
        record.what = adjustCursorEvent;
    } else if (event->type() == eventNames().mousedownEvent) {
        record.what = mouseDown;
        // The plugin needs focus to receive keyboard events
        if (Page* page = m_parentFrame->page())
            page->focusController()->setFocusedFrame(m_parentFrame);
        m_parentFrame->document()->setFocusedNode(m_element);
    } else if (event->type() == eventNames().mouseupEvent) {
        record.what = mouseUp;
    } else {
        return;
    }

    if (platformPluginWidget()) {
        record.where = globalMousePosForPlugin();
    } else {
        if (event->button() == 2) {
            // always pass the global position for right-click since Flash uses it to position the context menu
            record.where = globalMousePosForPlugin();
        } else {
            IntPoint postZoomPos = roundedIntPoint(m_element->renderer()->absoluteToLocal(event->absoluteLocation()));
            record.where.h = postZoomPos.x() + m_windowRect.x();
            // The number 22 is the height of the title bar. As to why it figures in the calculation below 
            // is left as an exercise to the reader :-)
            record.where.v = postZoomPos.y() + m_windowRect.y() - 22;
        }
    }
    record.modifiers = modifiersForEvent(event);

    if (!event->buttonDown())
        record.modifiers |= btnState;

    if (event->button() == 2)
        record.modifiers |= controlKey;

    if (!dispatchNPEvent(record)) {
        if (record.what == adjustCursorEvent)
            return; // Signals that the plugin wants a normal cursor

        LOG(Events, "PluginView::handleMouseEvent(): Mouse event type %d at %d,%d not accepted",
                record.what, record.where.h, record.where.v);
    } else {
        event->setDefaultHandled();
    }
}

void PluginView::handleKeyboardEvent(KeyboardEvent* event)
{
    if (!m_isStarted)
        return;

    LOG(Plugins, "PluginView::handleKeyboardEvent() ----------------- ");

    LOG(Plugins, "PV::hKE(): KE.keyCode: 0x%02X, KE.charCode: %d",
            event->keyCode(), event->charCode());

    EventRecord record;

    if (event->type() == eventNames().keydownEvent) {
        // This event is the result of a PlatformKeyboardEvent::KeyDown which
        // was disambiguated into a PlatformKeyboardEvent::RawKeyDown. Since
        // we don't have access to the text here, we return, and wait for the
        // corresponding event based on PlatformKeyboardEvent::Char.
        return;
    } else if (event->type() == eventNames().keypressEvent) {
        // Which would be this one. This event was disambiguated from the same
        // PlatformKeyboardEvent::KeyDown, but to a PlatformKeyboardEvent::Char,
        // which retains the text from the original event. So, we can safely pass
        // on the event as a key-down event to the plugin.
        record.what = keyDown;
    } else if (event->type() == eventNames().keyupEvent) {
        // PlatformKeyboardEvent::KeyUp events always have the text, so nothing
        // fancy here.
        record.what = keyUp;
    } else {
        return;
    }

    const PlatformKeyboardEvent* platformEvent = event->keyEvent();
    int keyCode = platformEvent->nativeVirtualKeyCode();

    const String text = platformEvent->text();
    if (text.length() < 1) {
        event->setDefaultHandled();
        return;
    }

    WTF::RetainPtr<CFStringRef> cfText(WTF::AdoptCF, text.createCFString());

    LOG(Plugins, "PV::hKE(): PKE.text: %s, PKE.unmodifiedText: %s, PKE.keyIdentifier: %s",
            text.ascii().data(), platformEvent->unmodifiedText().ascii().data(),
            platformEvent->keyIdentifier().ascii().data());

    char charCodes[2] = { 0, 0 };
    if (!CFStringGetCString(cfText.get(), charCodes, 2, CFStringGetSystemEncoding())) {
        LOG_ERROR("Could not resolve character code using system encoding.");
        event->setDefaultHandled();
        return;
    }

    record.where = globalMousePosForPlugin();
    record.modifiers = modifiersForEvent(event);
    record.message = ((keyCode & 0xFF) << 8) | (charCodes[0] & 0xFF);
    record.when = TickCount();

    LOG(Plugins, "PV::hKE(): record.modifiers: %d", record.modifiers);

#if PLATFORM(QT)
    LOG(Plugins, "PV::hKE(): PKE.qtEvent()->nativeVirtualKey: 0x%02X, charCode: %d",
               keyCode, int(uchar(charCodes[0])));
#endif

    if (!dispatchNPEvent(record))
        LOG(Events, "PluginView::handleKeyboardEvent(): Keyboard event type %d not accepted", record.what);
    else
        event->setDefaultHandled();
}

static int modifiersForEvent(UIEventWithKeyState* event)
{
    int modifiers = 0;

    if (event->ctrlKey())
        modifiers |= controlKey;

    if (event->altKey())
        modifiers |= optionKey;

    if (event->metaKey())
        modifiers |= cmdKey;

    if (event->shiftKey())
        modifiers |= shiftKey;

     return modifiers;
}

static bool tigerOrBetter()
{
    static SInt32 systemVersion = 0;

    if (!systemVersion) {
        if (Gestalt(gestaltSystemVersion, &systemVersion) != noErr)
            return false;
    }

    return systemVersion >= 0x1040;
}

Point PluginView::globalMousePosForPlugin() const
{
    Point pos;
    GetGlobalMouse(&pos);

    float scaleFactor = tigerOrBetter() ? HIGetScaleFactor() : 1;

    pos.h = short(pos.h * scaleFactor);
    pos.v = short(pos.v * scaleFactor);

#if PLATFORM(WX)
    // make sure the titlebar/toolbar size is included
    WindowRef windowRef = nativeWindowFor(platformPluginWidget());
    ::Rect content, structure;

    GetWindowBounds(windowRef, kWindowStructureRgn, &structure);
    GetWindowBounds(windowRef, kWindowContentRgn, &content);

    int top = content.top  - structure.top;
    pos.v -= top;
#endif

    return pos;
}

bool PluginView::dispatchNPEvent(NPEvent& event)
{
    PluginView::setCurrentPluginView(this);
    JSC::JSLock::DropAllLocks dropAllLocks(JSC::SilenceAssertionsOnly);
    setCallingPlugin(true);

    bool accepted = m_plugin->pluginFuncs()->event(m_instance, &event);

    setCallingPlugin(false);
    PluginView::setCurrentPluginView(0);
    return accepted;
}

// ------------------- Miscellaneous  ------------------

NPError PluginView::handlePostReadFile(Vector<char>& buffer, uint32 len, const char* buf)
{
    String filename(buf, len);

    if (filename.startsWith("file:///"))
        filename = filename.substring(8);

    if (!fileExists(filename))
        return NPERR_FILE_NOT_FOUND;

    FILE* fileHandle = fopen((filename.utf8()).data(), "r");

    if (fileHandle == 0)
        return NPERR_FILE_NOT_FOUND;

    int bytesRead = fread(buffer.data(), 1, 0, fileHandle);

    fclose(fileHandle);

    if (bytesRead <= 0)
        return NPERR_FILE_NOT_FOUND;

    return NPERR_NO_ERROR;
}

void PluginView::halt()
{
}

void PluginView::restart()
{
}

} // namespace WebCore

#else

#include "../PluginViewNone.cpp"

#endif // !__LP64__
