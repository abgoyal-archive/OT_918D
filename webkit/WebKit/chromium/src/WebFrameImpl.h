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
 * Copyright (C) 2009 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef WebFrameImpl_h
#define WebFrameImpl_h

// FIXME: remove this relative path once consumers from glue are removed.
#include "../public/WebFrame.h"
#include "Frame.h"
#include "FrameLoaderClientImpl.h"
#include "PlatformString.h"
#include <wtf/OwnPtr.h>
#include <wtf/RefCounted.h>

#include "WebAnimationControllerImpl.h"

namespace WebCore {
class HistoryItem;
class KURL;
class Node;
class Range;
class SubstituteData;
struct WindowFeatures;
}

namespace WebKit {
class ChromePrintContext;
class WebDataSourceImpl;
class WebInputElement;
class WebFrameClient;
class WebPasswordAutocompleteListener;
class WebView;
class WebViewImpl;

// Implementation of WebFrame, note that this is a reference counted object.
class WebFrameImpl : public WebFrame, public RefCounted<WebFrameImpl> {
public:
    // WebFrame methods:
    virtual WebString name() const;
    virtual void clearName();
    virtual WebURL url() const;
    virtual WebURL favIconURL() const;
    virtual WebURL openSearchDescriptionURL() const;
    virtual WebString encoding() const;
    virtual WebSize scrollOffset() const;
    virtual WebSize contentsSize() const;
    virtual int contentsPreferredWidth() const;
    virtual int documentElementScrollHeight() const;
    virtual bool hasVisibleContent() const;
    virtual WebView* view() const;
    virtual WebFrame* opener() const;
    virtual WebFrame* parent() const;
    virtual WebFrame* top() const;
    virtual WebFrame* firstChild() const;
    virtual WebFrame* lastChild() const;
    virtual WebFrame* nextSibling() const;
    virtual WebFrame* previousSibling() const;
    virtual WebFrame* traverseNext(bool wrap) const;
    virtual WebFrame* traversePrevious(bool wrap) const;
    virtual WebFrame* findChildByName(const WebString&) const;
    virtual WebFrame* findChildByExpression(const WebString&) const;
    virtual WebDocument document() const;
    virtual void forms(WebVector<WebFormElement>&) const;
    virtual WebAnimationController* animationController(); 
    virtual WebSecurityOrigin securityOrigin() const;
    virtual void grantUniversalAccess();
    virtual NPObject* windowObject() const;
    virtual void bindToWindowObject(const WebString& name, NPObject*);
    virtual void executeScript(const WebScriptSource&);
    virtual void executeScriptInIsolatedWorld(
        int worldId, const WebScriptSource* sources, unsigned numSources,
        int extensionGroup);
    virtual void addMessageToConsole(const WebConsoleMessage&);
    virtual void collectGarbage();
#if WEBKIT_USING_V8
    virtual v8::Local<v8::Context> mainWorldScriptContext() const;
#endif
    virtual bool insertStyleText(const WebString& css, const WebString& id);
    virtual void reload();
    virtual void loadRequest(const WebURLRequest&);
    virtual void loadHistoryItem(const WebHistoryItem&);
    virtual void loadData(
        const WebData&, const WebString& mimeType, const WebString& textEncoding,
        const WebURL& baseURL, const WebURL& unreachableURL, bool replace);
    virtual void loadHTMLString(
        const WebData& html, const WebURL& baseURL, const WebURL& unreachableURL,
        bool replace);
    virtual bool isLoading() const;
    virtual void stopLoading();
    virtual WebDataSource* provisionalDataSource() const;
    virtual WebDataSource* dataSource() const;
    virtual WebHistoryItem previousHistoryItem() const;
    virtual WebHistoryItem currentHistoryItem() const;
    virtual void enableViewSourceMode(bool enable);
    virtual bool isViewSourceModeEnabled() const;
    virtual void setReferrerForRequest(WebURLRequest&, const WebURL& referrer);
    virtual void dispatchWillSendRequest(WebURLRequest&);
    virtual void commitDocumentData(const char* data, size_t length);
    virtual unsigned unloadListenerCount() const;
    virtual bool isProcessingUserGesture() const;
    virtual bool willSuppressOpenerInNewFrame() const;
    virtual void replaceSelection(const WebString&);
    virtual void insertText(const WebString&);
    virtual void setMarkedText(const WebString&, unsigned location, unsigned length);
    virtual void unmarkText();
    virtual bool hasMarkedText() const;
    virtual WebRange markedRange() const;
    virtual bool executeCommand(const WebString&);
    virtual bool executeCommand(const WebString&, const WebString& value);
    virtual bool isCommandEnabled(const WebString&) const;
    virtual void enableContinuousSpellChecking(bool);
    virtual bool isContinuousSpellCheckingEnabled() const;
    virtual bool hasSelection() const;
    virtual WebRange selectionRange() const;
    virtual WebString selectionAsText() const;
    virtual WebString selectionAsMarkup() const;
    virtual bool selectWordAroundCaret();
    virtual int printBegin(const WebSize& pageSize);
    virtual float printPage(int pageToPrint, WebCanvas*);
    virtual float getPrintPageShrink(int page);
    virtual void printEnd();
    virtual bool find(
        int identifier, const WebString& searchText, const WebFindOptions&,
        bool wrapWithinFrame, WebRect* selectionRect);
    virtual void stopFinding(bool clearSelection);
    virtual void scopeStringMatches(
        int identifier, const WebString& searchText, const WebFindOptions&,
        bool reset);
    virtual void cancelPendingScopingEffort();
    virtual void increaseMatchCount(int count, int identifier);
    virtual void resetMatchCount();
    virtual void registerPasswordListener(
        WebInputElement, WebPasswordAutocompleteListener*);

    virtual WebURL completeURL(const WebString& url) const;
    virtual WebString contentAsText(size_t maxChars) const;
    virtual WebString contentAsMarkup() const;
    virtual WebString renderTreeAsText() const;
    virtual WebString counterValueForElementById(const WebString& id) const;
    virtual int pageNumberForElementById(const WebString& id,
                                         float pageWidthInPixels,
                                         float pageHeightInPixels) const;

    static PassRefPtr<WebFrameImpl> create(WebFrameClient* client);
    ~WebFrameImpl();

    // Called by the WebViewImpl to initialize its main frame:
    void initializeAsMainFrame(WebViewImpl*);

    PassRefPtr<WebCore::Frame> createChildFrame(
        const WebCore::FrameLoadRequest&, WebCore::HTMLFrameOwnerElement*);

    void layout();
    void paint(WebCanvas*, const WebRect&);
    void createFrameView();

    static WebFrameImpl* fromFrame(WebCore::Frame* frame);
    static WebFrameImpl* fromFrameOwnerElement(WebCore::Element* element);

    WebViewImpl* viewImpl() const;

    WebCore::Frame* frame() const { return m_frame; }
    WebCore::FrameView* frameView() const { return m_frame ? m_frame->view() : 0; }

    // Getters for the impls corresponding to Get(Provisional)DataSource. They
    // may return 0 if there is no corresponding data source.
    WebDataSourceImpl* dataSourceImpl() const;
    WebDataSourceImpl* provisionalDataSourceImpl() const;

    // Returns which frame has an active match. This function should only be
    // called on the main frame, as it is the only frame keeping track. Returned
    // value can be 0 if no frame has an active match.
    const WebFrameImpl* activeMatchFrame() const { return m_activeMatchFrame; }

    // When a Find operation ends, we want to set the selection to what was active
    // and set focus to the first focusable node we find (starting with the first
    // node in the matched range and going up the inheritance chain). If we find
    // nothing to focus we focus the first focusable node in the range. This
    // allows us to set focus to a link (when we find text inside a link), which
    // allows us to navigate by pressing Enter after closing the Find box.
    void setFindEndstateFocusAndSelection();

    void didFail(const WebCore::ResourceError&, bool wasProvisional);

    // Sets whether the WebFrameImpl allows its document to be scrolled.
    // If the parameter is true, allow the document to be scrolled.
    // Otherwise, disallow scrolling.
    void setAllowsScrolling(bool);

    // Returns the password autocomplete listener associated with the passed
    // user name input element, or 0 if none available.
    // Note that the returned listener is owner by the WebFrameImpl and should not
    // be kept around as it is deleted when the page goes away.
    WebPasswordAutocompleteListener* getPasswordListener(WebCore::HTMLInputElement*);

    WebFrameClient* client() const { return m_client; }
    void dropClient() { m_client = 0; }

    static void selectWordAroundPosition(WebCore::Frame*, WebCore::VisiblePosition);

private:
    class DeferredScopeStringMatches;
    friend class DeferredScopeStringMatches;
    friend class FrameLoaderClientImpl;

    // A bit mask specifying area of the frame to invalidate.
    enum AreaToInvalidate {
      InvalidateNothing,
      InvalidateContentArea,
      InvalidateScrollbar,   // Vertical scrollbar only.
      InvalidateAll          // Both content area and the scrollbar.
    };

    WebFrameImpl(WebFrameClient*);

    // Informs the WebFrame that the Frame is being closed, called by the
    // WebFrameLoaderClient
    void closing();

    // Notifies the delegate about a new selection rect.
    void reportFindInPageSelection(
        const WebRect& selectionRect, int activeMatchOrdinal, int identifier);

    // Invalidates a certain area within the frame.
    void invalidateArea(AreaToInvalidate);

    // Add a WebKit TextMatch-highlight marker to nodes in a range.
    void addMarker(WebCore::Range*, bool activeMatch);

    // Sets the markers within a range as active or inactive.
    void setMarkerActive(WebCore::Range*, bool active);

    // Returns the ordinal of the first match in the frame specified. This
    // function enumerates the frames, starting with the main frame and up to (but
    // not including) the frame passed in as a parameter and counts how many
    // matches have been found.
    int ordinalOfFirstMatchForFrame(WebFrameImpl*) const;

    // Determines whether the scoping effort is required for a particular frame.
    // It is not necessary if the frame is invisible, for example, or if this
    // is a repeat search that already returned nothing last time the same prefix
    // was searched.
    bool shouldScopeMatches(const WebCore::String& searchText);

    // Queue up a deferred call to scopeStringMatches.
    void scopeStringMatchesSoon(
        int identifier, const WebString& searchText, const WebFindOptions&,
        bool reset);

    // Called by a DeferredScopeStringMatches instance.
    void callScopeStringMatches(
        DeferredScopeStringMatches*, int identifier, const WebString& searchText,
        const WebFindOptions&, bool reset);

    // Determines whether to invalidate the content area and scrollbar.
    void invalidateIfNecessary();

    // Clears the map of password listeners.
    void clearPasswordListeners();

    void loadJavaScriptURL(const WebCore::KURL&);

    FrameLoaderClientImpl m_frameLoaderClient;

    WebFrameClient* m_client;

    // This is a weak pointer to our corresponding WebCore frame.  A reference to
    // ourselves is held while frame_ is valid.  See our Closing method.
    WebCore::Frame* m_frame;

    // A way for the main frame to keep track of which frame has an active
    // match. Should be 0 for all other frames.
    WebFrameImpl* m_activeMatchFrame;

    // The range of the active match for the current frame.
    RefPtr<WebCore::Range> m_activeMatch;

    // The index of the active match.
    int m_activeMatchIndex;

    // This flag is used by the scoping effort to determine if we need to figure
    // out which rectangle is the active match. Once we find the active
    // rectangle we clear this flag.
    bool m_locatingActiveRect;

    // The scoping effort can time out and we need to keep track of where we
    // ended our last search so we can continue from where we left of.
    RefPtr<WebCore::Range> m_resumeScopingFromRange;

    // Keeps track of the last string this frame searched for. This is used for
    // short-circuiting searches in the following scenarios: When a frame has
    // been searched and returned 0 results, we don't need to search that frame
    // again if the user is just adding to the search (making it more specific).
    WebCore::String m_lastSearchString;

    // Keeps track of how many matches this frame has found so far, so that we
    // don't loose count between scoping efforts, and is also used (in conjunction
    // with m_lastSearchString and m_scopingComplete) to figure out if we need to
    // search the frame again.
    int m_lastMatchCount;

    // This variable keeps a cumulative total of matches found so far for ALL the
    // frames on the page, and is only incremented by calling IncreaseMatchCount
    // (on the main frame only). It should be -1 for all other frames.
    size_t m_totalMatchCount;

    // This variable keeps a cumulative total of how many frames are currently
    // scoping, and is incremented/decremented on the main frame only.
    // It should be -1 for all other frames.
    int m_framesScopingCount;

    // Keeps track of whether the scoping effort was completed (the user may
    // interrupt it before it completes by submitting a new search).
    bool m_scopingComplete;

    // Keeps track of when the scoping effort should next invalidate the scrollbar
    // and the frame area.
    int m_nextInvalidateAfter;

    // A list of all of the pending calls to scopeStringMatches.
    Vector<DeferredScopeStringMatches*> m_deferredScopingWork;

    // Valid between calls to BeginPrint() and EndPrint(). Containts the print
    // information. Is used by PrintPage().
    OwnPtr<ChromePrintContext> m_printContext;

    // The input fields that are interested in edit events and their associated
    // listeners.
    typedef HashMap<RefPtr<WebCore::HTMLInputElement>,
                    WebPasswordAutocompleteListener*> PasswordListenerMap;
    PasswordListenerMap m_passwordListeners;

    // Keeps a reference to the frame's WebAnimationController.
    WebAnimationControllerImpl m_animationController;
};

} // namespace WebKit

#endif
