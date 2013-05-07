"""
Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
All rights reserved.
  
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  
The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
  
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
"""
from contextlib import contextmanager

from django.db import transaction
from django.test import TestCase
from django.test.client import Client
from django.core.urlresolvers import reverse
from django.contrib.auth.models import User
from django.utils.simplejson import loads


@contextmanager
def commit_all_or_rollback():
    transaction.commit_manually()
    try:
        yield
        transaction.commit()
    except:
        transaction.rollback()
        raise


class ShortcutTestCase(TestCase):
    def get(self, url_name, *args, **kwargs):
        status = kwargs.pop('status_code', None)
        params = kwargs.pop('get', '')

        headers = kwargs.pop('headers', {})
        if kwargs.pop('ajax_request', False):
            headers['X_REQUESTED_WITH'] = 'XMLHttpRequest'
        headers = self._process_headers(headers)

        response = self.client.get(reverse(url_name, args=args, kwargs=kwargs) + params, **headers)
        if status is not None:
            self.assertEqual(response.status_code, status)
        return response

    def json_get(self, *args, **kwargs):
        prefix = kwargs.pop('prefix', '')
        response = self.get(*args, **kwargs)
        self.assertTrue(response.content.startswith(prefix))
        try:
            return loads(response.content[len(prefix):])
        except:
            print "Got:", response
            raise

    def post(self, url_name, *args, **kwargs):
        status = kwargs.pop('status_code', None)
        data = kwargs.pop("data", {})
        params = kwargs.pop('get', '')

        headers = kwargs.pop('headers', {})
        if kwargs.pop('ajax_request', False):
            headers['X_REQUESTED_WITH'] = 'XMLHttpRequest'
        headers = self._process_headers(headers)

        response = self.client.post(reverse(url_name, args=args, kwargs=kwargs) + params, data, **headers)
        if status is not None:
            self.assertEqual(response.status_code, status)
        return response

    def json_post(self, *args, **kwargs):
        prefix = kwargs.pop('prefix', '')
        response = self.post(*args, **kwargs)
        self.assertTrue(response.content.startswith(prefix))
        try:
            return loads(response.content[len(prefix):])
        except:
            print "Got:", response
            raise

    def _process_headers(self, kwargs):
        headers = {}
        for key, value in kwargs.items():
            if key.startswith('HTTP_'):
                headers[key.upper().replace('-', '_')] = value
            else:
                headers['HTTP_' + key.upper().replace('-', '_')] = value
        return headers
