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


# Copyright (C) 2009 Google Inc. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#    * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#    * Neither the name of Google Inc. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import unittest

from webkitpy.bugzilla import Attachment
from webkitpy.mock import Mock
from webkitpy.mock_bugzillatool import MockBugzillaTool
from webkitpy.outputcapture import OutputCapture


class MockQueueEngine(object):
    def __init__(self, name, queue):
        pass

    def run(self):
        pass


class QueuesTest(unittest.TestCase):
    mock_work_item = Attachment({
        "id" : 1234,
        "bug_id" : 345,
        "attacher_email": "adam@example.com",
    }, None)

    def assert_queue_outputs(self, queue, args=None, work_item=None, expected_stdout=None, expected_stderr=None, options=Mock(), tool=MockBugzillaTool()):
        if not expected_stdout:
            expected_stdout = {}
        if not expected_stderr:
            expected_stderr = {}
        if not args:
            args = []
        if not work_item:
            work_item = self.mock_work_item
        tool.user.prompt = lambda message: "yes"

        queue.execute(options, args, tool, engine=MockQueueEngine)

        OutputCapture().assert_outputs(self,
                queue.queue_log_path,
                expected_stdout=expected_stdout.get("queue_log_path", ""),
                expected_stderr=expected_stderr.get("queue_log_path", ""))
        OutputCapture().assert_outputs(self,
                queue.work_item_log_path,
                args=[work_item],
                expected_stdout=expected_stdout.get("work_item_log_path", ""),
                expected_stderr=expected_stderr.get("work_item_log_path", ""))
        OutputCapture().assert_outputs(self,
                queue.begin_work_queue,
                expected_stdout=expected_stdout.get("begin_work_queue", ""),
                expected_stderr=expected_stderr.get("begin_work_queue", ""))
        OutputCapture().assert_outputs(self,
                queue.should_continue_work_queue,
                expected_stdout=expected_stdout.get("should_continue_work_queue", ""), expected_stderr=expected_stderr.get("should_continue_work_queue", ""))
        OutputCapture().assert_outputs(self,
                queue.next_work_item,
                expected_stdout=expected_stdout.get("next_work_item", ""),
                expected_stderr=expected_stderr.get("next_work_item", ""))
        OutputCapture().assert_outputs(self,
                queue.should_proceed_with_work_item,
                args=[work_item],
                expected_stdout=expected_stdout.get("should_proceed_with_work_item", ""),
                expected_stderr=expected_stderr.get("should_proceed_with_work_item", ""))
        OutputCapture().assert_outputs(self,
                queue.process_work_item,
                args=[work_item],
                expected_stdout=expected_stdout.get("process_work_item", ""),
                expected_stderr=expected_stderr.get("process_work_item", ""))
        OutputCapture().assert_outputs(self,
                queue.handle_unexpected_error,
                args=[work_item, "Mock error message"],
                expected_stdout=expected_stdout.get("handle_unexpected_error", ""),
                expected_stderr=expected_stderr.get("handle_unexpected_error", ""))
