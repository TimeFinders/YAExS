"""
Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
All rights reserved.
  
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  
The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
  
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
"""
import sys
from functools import wraps

from django.conf import settings
from django.http import HttpResponse, Http404
from django.template import RequestContext
from django.shortcuts import render_to_response
from django.core.exceptions import ObjectDoesNotExist, PermissionDenied
from django.contrib.auth.decorators import login_required

__all__ = ['Renderer', 'login_required', 'staff_required']


def staff_required(fn):
    @login_required
    @wraps(fn)
    def wrapper(request, *args, **kwargs):
        if not request.user.is_staff:
            raise PermissionDenied("You are not an admin")
        return fn(request, *args, **kwargs)
    return wrapper


class AlternativeResponse(Exception):
    def __init__(self, response):
        self.response = response


class Renderer(object):
    def __init__(self, **settings):
        self.settings = {
            'error_handler': self.handle_error,
            'template_prefix': '',
            'template_name': 'template.html',
            'context': {},
            'mimetype': None,
            'headers': {},
            'encoder': None,
            'posthook': None,
        }
        self.settings.update(settings)

    def __repr__(self):
        return "Renderer(settings=%r)" % (self.settings,)

    def __call__(self, **custom_settings):
        "Behaves like a decorator"
        def decorator(fn):
            @wraps(fn)
            def decorated(request, *args, **kwargs):
                true_settings = {}
                true_settings.update(self.settings)
                true_settings.update(custom_settings)
                try:
                    result = fn(request, *args, **kwargs)
                    true_settings.update(result)
                    if callable(true_settings['posthook']):
                        true_settings = true_settings['posthook'](true_settings, request, *args, **kwargs)
                    response = self.create_response(request, true_settings)
                    return self.assign_headers(response, true_settings['headers'])
                except AlternativeResponse as altresponse:
                    return altresponse.response
                except Exception as error:
                    (_, _, traceback) = sys.exc_info()
                    return self.handle_error(error, traceback)
            decorated.raw_view = fn
            decorated.decorator = self
            return decorated
        return decorator

    def handle_error(self, error, traceback):
        if settings.DEBUG:
            raise (error, None, traceback)  # remove tuple, if this is broken
        raise Http404

    def extract_view(self, decorated_view, original=False):
        if not original:
            return decorated_view.raw_view
        v = decorated_view
        while hasattr(v, 'raw_view'):
            v = v.raw_view
        return v

    def assign_headers(self, response, headers):
        for name, value in headers.items():
            response[name] = value
        return response

    def process_context(self, context, encoder):
        if encoder is None:
            return context
        for key in context.keys():
            context[key] = encoder(context[key])
        return context

    def create_response(self, request, settings):
        context = self.process_context(settings['context'], settings['encoder'])
        full_template_path = settings['template_prefix'] + settings['template_name']
        return render_to_response(full_template_path,
                context,
                context_instance=RequestContext(request),
                mimetype=settings['mimetype'])
