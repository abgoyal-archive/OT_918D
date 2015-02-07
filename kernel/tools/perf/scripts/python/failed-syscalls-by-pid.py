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


# failed system call counts, by pid
# (c) 2010, Tom Zanussi <tzanussi@gmail.com>
# Licensed under the terms of the GNU GPL License version 2
#
# Displays system-wide failed system call totals, broken down by pid.
# If a [comm] arg is specified, only syscalls called by [comm] are displayed.

import os
import sys

sys.path.append(os.environ['PERF_EXEC_PATH'] + \
	'/scripts/python/Perf-Trace-Util/lib/Perf/Trace')

from perf_trace_context import *
from Core import *

usage = "perf trace -s syscall-counts-by-pid.py [comm]\n";

for_comm = None

if len(sys.argv) > 2:
	sys.exit(usage)

if len(sys.argv) > 1:
	for_comm = sys.argv[1]

syscalls = autodict()

def trace_begin():
	pass

def trace_end():
	print_error_totals()

def raw_syscalls__sys_exit(event_name, context, common_cpu,
	common_secs, common_nsecs, common_pid, common_comm,
	id, ret):
	if for_comm is not None:
		if common_comm != for_comm:
			return

	if ret < 0:
		try:
			syscalls[common_comm][common_pid][id][ret] += 1
		except TypeError:
			syscalls[common_comm][common_pid][id][ret] = 1

def print_error_totals():
    if for_comm is not None:
	    print "\nsyscall errors for %s:\n\n" % (for_comm),
    else:
	    print "\nsyscall errors:\n\n",

    print "%-30s  %10s\n" % ("comm [pid]", "count"),
    print "%-30s  %10s\n" % ("------------------------------", \
                                 "----------"),

    comm_keys = syscalls.keys()
    for comm in comm_keys:
	    pid_keys = syscalls[comm].keys()
	    for pid in pid_keys:
		    print "\n%s [%d]\n" % (comm, pid),
		    id_keys = syscalls[comm][pid].keys()
		    for id in id_keys:
			    print "  syscall: %-16d\n" % (id),
			    ret_keys = syscalls[comm][pid][id].keys()
			    for ret, val in sorted(syscalls[comm][pid][id].iteritems(), key = lambda(k, v): (v, k),  reverse = True):
				    print "    err = %-20d  %10d\n" % (ret, val),
