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


from webkit.steps import *
from buildbot.process import factory

s = factory.s

class BuildFactory(factory.BuildFactory):
    useProgress = False
    def __init__(self):
        factory.BuildFactory.__init__(self, [s(CheckOutSource)])

class StandardBuildFactory(BuildFactory):
    def __init__(self):
        BuildFactory.__init__(self)
        self.steps.append(s(SetConfiguration, configuration="release"))
        self.addCompileStep()
        self.addJavaScriptCoreTestStep()
        self.addLayoutTestStep()
        self.steps.append(s(UploadLayoutResults))
#        self.steps.append(s(UploadDiskImage))

    def addCompileStep(self):
        self.steps.append(s(CompileWebKit, configuration="release"))

    def addJavaScriptCoreTestStep(self):
        self.steps.append(s(JavaScriptCoreTest))

    def addLayoutTestStep(self):
        self.steps.append(s(LayoutTest))


class NoSVGBuildFactory(StandardBuildFactory):
    def addCompileStep(self):
        self.steps.append(s(CompileWebKitNoSVG, configuration="release"))


class PixelTestBuildFactory(BuildFactory):
    def __init__(self):
        BuildFactory.__init__(self)
        self.steps.append(s(SetConfiguration, configuration="release"))
        self.steps.append(s(CompileWebKit, configuration="release"))
        self.steps.append(s(PixelLayoutTest))
        self.steps.append(s(UploadLayoutResults))


class LeakBuildFactory(BuildFactory):
    def __init__(self):
        BuildFactory.__init__(self)
        self.steps.append(s(SetConfiguration, configuration="debug"))
        self.steps.append(s(CompileWebKit, configuration="debug"))
        self.steps.append(s(JavaScriptCoreTest))
        self.steps.append(s(LeakTest))
        self.steps.append(s(UploadLayoutResults))
#        self.steps.append(s(UploadDiskImage))


class PageLoadTestBuildFactory(BuildFactory):
    def __init__(self):
        BuildFactory.__init__(self)
        self.steps.append(s(CompileWebKit, configuration="release"))
        self.steps.append(s(PageLoadTest))


class Win32BuildFactory(StandardBuildFactory):
    def addCompileStep(self):
        self.steps.append(s(InstallWin32Dependencies))
        self.steps.append(s(SetConfiguration, configuration="debug"))
        self.steps.append(s(CompileWebKitWindows, configuration="debug"))

    def addLayoutTestStep(self):
        self.steps.append(s(LayoutTestWindows))

class GtkBuildFactory(StandardBuildFactory):
    def addCompileStep(self):
#        self.steps.append(s(CleanWebKitGtk, configuration="release"))
        self.steps.append(s(CompileWebKitGtk, configuration="release"))

    def addJavaScriptCoreTestStep(self):
        self.steps.append(s(JavaScriptCoreTestGtk))

    def addLayoutTestStep(self):
        pass


class WxBuildFactory(StandardBuildFactory):
    def addCompileStep(self):
        self.steps.append(s(CleanWebKitWx, configuration="release"))
        self.steps.append(s(CompileWebKitWx, configuration="release"))

    def addJavaScriptCoreTestStep(self):
        self.steps.append(s(JavaScriptCoreTestWx))

    def addLayoutTestStep(self):
        pass


class QtBuildFactory(StandardBuildFactory):
    def addCompileStep(self):
        self.steps.append(s(CleanWebKit, configuration="release"))
        self.steps.append(s(CompileWebKit, configuration="release"))

    def addLayoutTestStep(self):
        pass # self.steps.append(s(LayoutTestQt))


class CoverageDataBuildFactory(BuildFactory):
    def __init__(self):
        BuildFactory.__init__(self)
        self.steps.append(s(GenerateCoverageData))
        self.steps.append(s(UploadCoverageData))
