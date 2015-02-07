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
 * Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies)
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
#include "PluginDebug.h"
#include "PlatformString.h"

#if !LOG_DISABLED

namespace WebCore {

static const char* const errorStrings[] = {
    "No errors occurred.", /* NPERR_NO_ERROR */
    "Error with no specific error code occurred.", /* NPERR_GENERIC_ERROR */
    "Invalid instance passed to the plug-in.", /* NPERR_INVALID_INSTANCE_ERROR */
    "Function table invalid.", /* NPERR_INVALID_FUNCTABLE_ERROR */
    "Loading of plug-in failed.", /* NPERR_MODULE_LOAD_FAILED_ERROR */
    "Memory allocation failed.", /* NPERR_OUT_OF_MEMORY_ERROR */
    "Plug-in missing or invalid.", /* NPERR_INVALID_PLUGIN_ERROR */
    "Plug-in directory missing or invalid.", /* NPERR_INVALID_PLUGIN_DIR_ERROR */
    "Versions of plug-in and Communicator do not match.", /* NPERR_INCOMPATIBLE_VERSION_ERROR */
    "Parameter missing or invalid.", /* NPERR_INVALID_PARAM */
    "URL missing or invalid.", /* NPERR_INVALID_URL */
    "File missing or invalid.", /* NPERR_FILE_NOT_FOUND */
    "Stream contains no data.", /* NPERR_NO_DATA */
    "Seekable stream expected.", /* NPERR_STREAM_NOT_SEEKABLE */
    "Unknown error code"
};

#ifdef XP_MACOSX
static const char* const drawingModels[] = {
    "NPDrawingModelQuickDraw",
    "NPDrawingModelCoreGraphics",
    "NPDrawingModelOpenGL",
    "NPDrawingModelCoreAnimation"
};

static const char* const eventModels[] = {
    "NPEventModelCarbon",
    "NPEventModelCocoa"
};
#endif //XP_MACOSX

const char* prettyNameForNPError(NPError error)
{
    return errorStrings[error];
}

#ifdef XP_MACOSX
const char* prettyNameForDrawingModel(NPDrawingModel drawingModel)
{
    return drawingModels[drawingModel];
}

const char* prettyNameForEventModel(NPEventModel eventModel)
{
    return eventModels[eventModel];
}
#endif //XP_MACOSX

CString prettyNameForNPNVariable(NPNVariable variable)
{
    switch (variable) {
    case NPNVxDisplay: return "NPNVxDisplay";
    case NPNVxtAppContext: return "NPNVxtAppContext";
    case NPNVnetscapeWindow: return "NPNVnetscapeWindow";
    case NPNVjavascriptEnabledBool: return "NPNVjavascriptEnabledBool";
    case NPNVasdEnabledBool: return "NPNVasdEnabledBool";
    case NPNVisOfflineBool: return "NPNVisOfflineBool";

    case NPNVserviceManager: return "NPNVserviceManager (not supported)";
    case NPNVDOMElement: return "NPNVDOMElement (not supported)";
    case NPNVDOMWindow: return "NPNVDOMWindow (not supported)";
    case NPNVToolkit: return "NPNVToolkit (not supported)";
    case NPNVSupportsXEmbedBool: return "NPNVSupportsXEmbedBool (not supported)";

    case NPNVWindowNPObject: return "NPNVWindowNPObject";
    case NPNVPluginElementNPObject: return "NPNVPluginElementNPObject";
    case NPNVSupportsWindowless: return "NPNVSupportsWindowless";
    case NPNVprivateModeBool: return "NPNVprivateModeBool";

#ifdef XP_MACOSX
    case NPNVpluginDrawingModel: return "NPNVpluginDrawingModel";
#ifndef NP_NO_QUICKDRAW
    case NPNVsupportsQuickDrawBool: return "NPNVsupportsQuickDrawBool";
#endif
    case NPNVsupportsCoreGraphicsBool: return "NPNVsupportsCoreGraphicsBool";
    case NPNVsupportsOpenGLBool: return "NPNVsupportsOpenGLBool";
    case NPNVsupportsCoreAnimationBool: return "NPNVsupportsCoreAnimationBool";
#ifndef NP_NO_CARBON
    case NPNVsupportsCarbonBool: return "NPNVsupportsCarbonBool";
#endif
    case NPNVsupportsCocoaBool: return "NPNVsupportsCocoaBool";
#endif

    default: return "Unknown variable";
    }
}

CString prettyNameForNPPVariable(NPPVariable variable, void* value)
{
    switch (variable) {
    case NPPVpluginNameString: return "NPPVpluginNameString";
    case NPPVpluginDescriptionString: return "NPPVpluginDescriptionString";
    case NPPVpluginWindowBool: return "NPPVpluginWindowBool";
    case NPPVpluginTransparentBool: return "NPPVpluginTransparentBool";

    case NPPVjavaClass: return "NPPVjavaClass (not supported)";
    case NPPVpluginWindowSize: return "NPPVpluginWindowSize (not supported)";
    case NPPVpluginTimerInterval: return "NPPVpluginTimerInterval (not supported)";
    case NPPVpluginScriptableInstance: return "NPPVpluginScriptableInstance (not supported)";
    case NPPVpluginScriptableIID: return "NPPVpluginScriptableIID (not supported)";
    case NPPVjavascriptPushCallerBool: return "NPPVjavascriptPushCallerBool (not supported)";
    case NPPVpluginKeepLibraryInMemory: return "NPPVpluginKeepLibraryInMemory (not supported)";
    case NPPVpluginNeedsXEmbed: return "NPPVpluginNeedsXEmbed (not supported)";

    case NPPVpluginScriptableNPObject: return "NPPVpluginScriptableNPObject";

    case NPPVformValue: return "NPPVformValue (not supported)";
    case NPPVpluginUrlRequestsDisplayedBool: return "NPPVpluginUrlRequestsDisplayedBool (not supported)";

    case NPPVpluginWantsAllNetworkStreams: return "NPPVpluginWantsAllNetworkStreams";
    case NPPVpluginCancelSrcStream: return "NPPVpluginCancelSrcStream";

#ifdef XP_MACOSX
    case NPPVpluginDrawingModel: {
        String result("NPPVpluginDrawingModel, ");
        result.append(prettyNameForDrawingModel(NPDrawingModel(uintptr_t(value))));
        return result.latin1();
    }
    case NPPVpluginEventModel: {
        String result("NPPVpluginEventModel, ");
        result.append(prettyNameForEventModel(NPEventModel(uintptr_t(value))));
        return result.latin1();
    }
    case NPPVpluginCoreAnimationLayer: return "NPPVpluginCoreAnimationLayer";
#endif

    default: return "Unknown variable";
    }
}

} // namespace WebCore

#endif // !LOG_DISABLED
