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


# Core.py - Python extension for perf trace, core functions
#
# Copyright (C) 2010 by Tom Zanussi <tzanussi@gmail.com>
#
# This software may be distributed under the terms of the GNU General
# Public License ("GPL") version 2 as published by the Free Software
# Foundation.

from collections import defaultdict

def autodict():
    return defaultdict(autodict)

flag_fields = autodict()
symbolic_fields = autodict()

def define_flag_field(event_name, field_name, delim):
    flag_fields[event_name][field_name]['delim'] = delim

def define_flag_value(event_name, field_name, value, field_str):
    flag_fields[event_name][field_name]['values'][value] = field_str

def define_symbolic_field(event_name, field_name):
    # nothing to do, really
    pass

def define_symbolic_value(event_name, field_name, value, field_str):
    symbolic_fields[event_name][field_name]['values'][value] = field_str

def flag_str(event_name, field_name, value):
    string = ""

    if flag_fields[event_name][field_name]:
	print_delim = 0
        keys = flag_fields[event_name][field_name]['values'].keys()
        keys.sort()
        for idx in keys:
            if not value and not idx:
                string += flag_fields[event_name][field_name]['values'][idx]
                break
            if idx and (value & idx) == idx:
                if print_delim and flag_fields[event_name][field_name]['delim']:
                    string += " " + flag_fields[event_name][field_name]['delim'] + " "
                string += flag_fields[event_name][field_name]['values'][idx]
                print_delim = 1
                value &= ~idx

    return string

def symbol_str(event_name, field_name, value):
    string = ""

    if symbolic_fields[event_name][field_name]:
        keys = symbolic_fields[event_name][field_name]['values'].keys()
        keys.sort()
        for idx in keys:
            if not value and not idx:
		string = symbolic_fields[event_name][field_name]['values'][idx]
                break
	    if (value == idx):
		string = symbolic_fields[event_name][field_name]['values'][idx]
                break

    return string

trace_flags = { 0x00: "NONE", \
                    0x01: "IRQS_OFF", \
                    0x02: "IRQS_NOSUPPORT", \
                    0x04: "NEED_RESCHED", \
                    0x08: "HARDIRQ", \
                    0x10: "SOFTIRQ" }

def trace_flag_str(value):
    string = ""
    print_delim = 0

    keys = trace_flags.keys()

    for idx in keys:
	if not value and not idx:
	    string += "NONE"
	    break

	if idx and (value & idx) == idx:
	    if print_delim:
		string += " | ";
	    string += trace_flags[idx]
	    print_delim = 1
	    value &= ~idx

    return string
