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


# system call top
# (c) 2010, Tom Zanussi <tzanussi@gmail.com>
# Licensed under the terms of the GNU GPL License version 2
#
# Periodically displays system-wide system call totals, broken down by
# syscall.  If a [comm] arg is specified, only syscalls called by
# [comm] are displayed. If an [interval] arg is specified, the display
# will be refreshed every [interval] seconds.  The default interval is
# 3 seconds.

import thread
import time
import os
import sys

sys.path.append(os.environ['PERF_EXEC_PATH'] + \
	'/scripts/python/Perf-Trace-Util/lib/Perf/Trace')

from perf_trace_context import *
from Core import *
from Util import *

usage = "perf trace -s syscall-counts.py [comm] [interval]\n";

for_comm = None
default_interval = 3
interval = default_interval

if len(sys.argv) > 3:
	sys.exit(usage)

if len(sys.argv) > 2:
	for_comm = sys.argv[1]
	interval = int(sys.argv[2])
elif len(sys.argv) > 1:
	try:
		interval = int(sys.argv[1])
	except ValueError:
		for_comm = sys.argv[1]
		interval = default_interval

syscalls = autodict()

def trace_begin():
	thread.start_new_thread(print_syscall_totals, (interval,))
	pass

def raw_syscalls__sys_enter(event_name, context, common_cpu,
	common_secs, common_nsecs, common_pid, common_comm,
	id, args):
	if for_comm is not None:
		if common_comm != for_comm:
			return
	try:
		syscalls[id] += 1
	except TypeError:
		syscalls[id] = 1

def print_syscall_totals(interval):
	while 1:
		clear_term()
		if for_comm is not None:
			print "\nsyscall events for %s:\n\n" % (for_comm),
		else:
			print "\nsyscall events:\n\n",

		print "%-40s  %10s\n" % ("event", "count"),
		print "%-40s  %10s\n" % ("----------------------------------------", \
						 "----------"),

		for id, val in sorted(syscalls.iteritems(), key = lambda(k, v): (v, k), \
					      reverse = True):
			try:
				print "%-40d  %10d\n" % (id, val),
			except TypeError:
				pass
		syscalls.clear()
		time.sleep(interval)
