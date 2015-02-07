#!/usr/bin/env python
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


# Copyright (c) 2009, Google Inc. All rights reserved.
# Copyright (c) 2009 Apple Inc. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
# 
#     * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#     * Neither the name of Google Inc. nor the names of its
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

import os
import time
import traceback

from datetime import datetime, timedelta

from webkitpy.executive import ScriptError
from webkitpy.webkit_logging import log, OutputTee
from webkitpy.statusserver import StatusServer

class QueueEngineDelegate:
    def queue_log_path(self):
        raise NotImplementedError, "subclasses must implement"

    def work_item_log_path(self, work_item):
        raise NotImplementedError, "subclasses must implement"

    def begin_work_queue(self):
        raise NotImplementedError, "subclasses must implement"

    def should_continue_work_queue(self):
        raise NotImplementedError, "subclasses must implement"

    def next_work_item(self):
        raise NotImplementedError, "subclasses must implement"

    def should_proceed_with_work_item(self, work_item):
        # returns (safe_to_proceed, waiting_message, patch)
        raise NotImplementedError, "subclasses must implement"

    def process_work_item(self, work_item):
        raise NotImplementedError, "subclasses must implement"

    def handle_unexpected_error(self, work_item, message):
        raise NotImplementedError, "subclasses must implement"


class QueueEngine:
    def __init__(self, name, delegate):
        self._name = name
        self._delegate = delegate
        self._output_tee = OutputTee()

    log_date_format = "%Y-%m-%d %H:%M:%S"
    sleep_duration_text = "5 mins"
    seconds_to_sleep = 300
    handled_error_code = 2

    # Child processes exit with a special code to the parent queue process can detect the error was handled.
    @classmethod
    def exit_after_handled_error(cls, error):
        log(error)
        exit(cls.handled_error_code)

    def run(self):
        self._begin_logging()

        self._delegate.begin_work_queue()
        while (self._delegate.should_continue_work_queue()):
            try:
                self._ensure_work_log_closed()
                work_item = self._delegate.next_work_item()
                if not work_item:
                    self._sleep("No work item.")
                    continue
                if not self._delegate.should_proceed_with_work_item(work_item):
                    self._sleep("Not proceeding with work item.")
                    continue

                # FIXME: Work logs should not depend on bug_id specificaly.
                #        This looks fixed, no?
                self._open_work_log(work_item)
                try:
                    self._delegate.process_work_item(work_item)
                except ScriptError, e:
                    # Use a special exit code to indicate that the error was already
                    # handled in the child process and we should just keep looping.
                    if e.exit_code == self.handled_error_code:
                        continue
                    message = "Unexpected failure when landing patch!  Please file a bug against webkit-patch.\n%s" % e.message_with_output()
                    self._delegate.handle_unexpected_error(work_item, message)
            except KeyboardInterrupt, e:
                log("\nUser terminated queue.")
                return 1
            except Exception, e:
                traceback.print_exc()
                # Don't try tell the status bot, in case telling it causes an exception.
                self._sleep("Exception while preparing queue")
        # Never reached.
        self._ensure_work_log_closed()

    def _begin_logging(self):
        self._queue_log = self._output_tee.add_log(self._delegate.queue_log_path())
        self._work_log = None

    def _open_work_log(self, work_item):
        work_item_log_path = self._delegate.work_item_log_path(work_item)
        self._work_log = self._output_tee.add_log(work_item_log_path)

    def _ensure_work_log_closed(self):
        # If we still have a bug log open, close it.
        if self._work_log:
            self._output_tee.remove_log(self._work_log)
            self._work_log = None

    @classmethod
    def _sleep_message(cls, message):
        wake_time = datetime.now() + timedelta(seconds=cls.seconds_to_sleep)
        return "%s Sleeping until %s (%s)." % (message, wake_time.strftime(cls.log_date_format), cls.sleep_duration_text)

    @classmethod
    def _sleep(cls, message):
        log(cls._sleep_message(message))
        time.sleep(cls.seconds_to_sleep)
