"""
Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
All rights reserved.
  
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  
The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
  
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
"""
import StringIO
import hotshot
import hotshot.stats
import os
import tempfile
import traceback
from django.conf import settings
from django.template import Template, Context
from debug_toolbar.panels import DebugPanel


class ProfilingPanel(DebugPanel):
    """
    Panel that runs the hotshot profiler during the request.
    """

    name = 'Profiling'
    has_content = True

    def __init__(self, *args, **kwargs):
        super(ProfilingPanel, self).__init__(*args, **kwargs)
        self.formatted_stats = ''
        self.stats = None

    def nav_title(self):
        """Title showing in toolbar"""
        return 'Profiling'

    def nav_subtitle(self):
        """Subtitle showing until title in toolbar"""
        if self.stats:
            return "%d function calls in %.3f CPU seconds" % (self.stats.total_calls, self.stats.total_tt)
        else:
            return "self.stats evaluates to False"

    def title(self):
        """Title showing in panel"""
        return 'Profiling'

    def url(self):
        return ''

    def content(self):
        if self.stats and not self.formatted_stats:
            try:
                buffer = StringIO.StringIO()
                self.stats.stream = buffer
                self.stats.sort_stats('time', 'calls')
                self.stats.print_stats(400)
                self.formatted_stats = buffer.getvalue()
            except:
                print "Error getting hotshot stats:"
                traceback.print_exc()

        template = Template("""<code>{{ formatted_stats }}</code>""")
        context = Context()
        context.update(self.context)
        context.update({
            'formatted_stats': self.formatted_stats,
        })
        return template.render(context)

    def process_view(self, request, view_func, view_args, view_kwargs):
        if request.path.startswith(settings.ADMIN_MEDIA_PREFIX) or request.path.startswith('/__debug__/'):
            return None

        # Add a timestamp to the profile output when the callable is actually called.
        handle, filename = tempfile.mkstemp(prefix='profiling')
        os.close(handle)
        prof = hotshot.Profile(filename)
        try:
            try:
                try:
                    result = prof.runcall(view_func, *((request,) + view_args), **view_kwargs)
                finally:
                    prof.close()
            except:
                raise
            else:
                self.stats = hotshot.stats.load(filename)
                return result
        finally:
            os.unlink(filename)
