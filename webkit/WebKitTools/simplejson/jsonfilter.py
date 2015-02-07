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


import simplejson
import cgi

class JSONFilter(object):
    def __init__(self, app, mime_type='text/x-json'):
        self.app = app
        self.mime_type = mime_type

    def __call__(self, environ, start_response):
        # Read JSON POST input to jsonfilter.json if matching mime type
        response = {'status': '200 OK', 'headers': []}
        def json_start_response(status, headers):
            response['status'] = status
            response['headers'].extend(headers)
        environ['jsonfilter.mime_type'] = self.mime_type
        if environ.get('REQUEST_METHOD', '') == 'POST':
            if environ.get('CONTENT_TYPE', '') == self.mime_type:
                args = [_ for _ in [environ.get('CONTENT_LENGTH')] if _]
                data = environ['wsgi.input'].read(*map(int, args))
                environ['jsonfilter.json'] = simplejson.loads(data)
        res = simplejson.dumps(self.app(environ, json_start_response))
        jsonp = cgi.parse_qs(environ.get('QUERY_STRING', '')).get('jsonp')
        if jsonp:
            content_type = 'text/javascript'
            res = ''.join(jsonp + ['(', res, ')'])
        elif 'Opera' in environ.get('HTTP_USER_AGENT', ''):
            # Opera has bunk XMLHttpRequest support for most mime types
            content_type = 'text/plain'
        else:
            content_type = self.mime_type
        headers = [
            ('Content-type', content_type),
            ('Content-length', len(res)),
        ]
        headers.extend(response['headers'])
        start_response(response['status'], headers)
        return [res]

def factory(app, global_conf, **kw):
    return JSONFilter(app, **kw)
