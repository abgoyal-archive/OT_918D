# Copyright Statement:
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. and/or its licensors.
# Without the prior written permission of MediaTek inc. and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.
#
# MediaTek Inc. (C) 2010. All rights reserved.
#
# BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
# THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
# RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
# AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
# NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
# SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
# SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
# THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
# THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
# CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
# SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
# STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
# CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
# AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
# OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
# MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.


## 
##
## Copyright 2008, The Android Open Source Project
##
## Licensed under the Apache License, Version 2.0 (the "License");
## you may not use this file except in compliance with the License.
## You may obtain a copy of the License at
##
##     http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS,
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
## See the License for the specific language governing permissions and
## limitations under the License.
##

LOCAL_SRC_FILES := \
	android/WebCoreSupport/CachedFramePlatformDataAndroid.cpp \
	android/WebCoreSupport/ChromeClientAndroid.cpp \
	android/WebCoreSupport/ContextMenuClientAndroid.cpp \
	android/WebCoreSupport/DragClientAndroid.cpp \
	android/WebCoreSupport/EditorClientAndroid.cpp \
	android/WebCoreSupport/FrameLoaderClientAndroid.cpp \
	android/WebCoreSupport/GeolocationPermissions.cpp \
	android/WebCoreSupport/MediaPlayerPrivateAndroid.cpp \
	android/WebCoreSupport/PlatformBridge.cpp \
	android/WebCoreSupport/ResourceLoaderAndroid.cpp \
	android/WebCoreSupport/V8Counters.cpp \
	\
	android/RenderSkinAndroid.cpp \
	android/RenderSkinButton.cpp \
	android/RenderSkinCombo.cpp \
	android/RenderSkinMediaButton.cpp \
	android/RenderSkinRadio.cpp \
	android/TimeCounter.cpp \
	\
	android/benchmark/Intercept.cpp \
	android/benchmark/MyJavaVM.cpp \
	\
	android/icu/unicode/ucnv.cpp \
	\
	android/jni/GeolocationPermissionsBridge.cpp \
	android/jni/JavaBridge.cpp \
	android/jni/JavaSharedClient.cpp \
	android/jni/MIMETypeRegistry.cpp \
	android/jni/MockGeolocation.cpp \
	android/jni/PictureSet.cpp \
	android/jni/WebCoreFrameBridge.cpp \
	android/jni/WebCoreJni.cpp \
	android/jni/WebCoreResourceLoader.cpp \
	android/jni/WebFrameView.cpp \
	android/jni/WebHistory.cpp \
	android/jni/WebIconDatabase.cpp \
	android/jni/WebStorage.cpp \
	android/jni/WebSettings.cpp \
	android/jni/WebViewCore.cpp \
	\
	android/nav/CacheBuilder.cpp \
	android/nav/CachedFrame.cpp \
	android/nav/CachedHistory.cpp \
	android/nav/CachedInput.cpp \
	android/nav/CachedLayer.cpp \
	android/nav/CachedNode.cpp \
	android/nav/CachedRoot.cpp \
	android/nav/FindCanvas.cpp \
	android/nav/SelectText.cpp \
	android/nav/WebView.cpp \
	\
	android/plugins/ANPBitmapInterface.cpp \
	android/plugins/ANPCanvasInterface.cpp \
	android/plugins/ANPEventInterface.cpp \
	android/plugins/ANPLogInterface.cpp \
	android/plugins/ANPMatrixInterface.cpp \
	android/plugins/ANPPaintInterface.cpp \
	android/plugins/ANPPathInterface.cpp \
	android/plugins/ANPSoundInterface.cpp \
	android/plugins/ANPSurfaceInterface.cpp \
	android/plugins/ANPSystemInterface.cpp \
	android/plugins/ANPTypefaceInterface.cpp \
	android/plugins/ANPWindowInterface.cpp \
	android/plugins/PluginDebugAndroid.cpp \
	android/plugins/PluginTimer.cpp \
	android/plugins/PluginViewBridgeAndroid.cpp \
	android/plugins/PluginWidgetAndroid.cpp \
	android/plugins/SkANP.cpp \
	\
	android/wds/Command.cpp \
	android/wds/Connection.cpp \
	android/wds/DebugServer.cpp
