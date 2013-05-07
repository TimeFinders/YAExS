"""
Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
All rights reserved.
  
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  
The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
  
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
"""
from optparse import make_option

from django.core.management.base import BaseCommand
from django.db import transaction

from courses.bridge import import_courses
from courses.bridge.rpi import add_cross_listing


class Command(BaseCommand):
    help = "Downloads the course data and imports it into the database."
    option_list = BaseCommand.option_list + (
        make_option('--force', '-f', dest='force', action='store_true',
            default=False,
            help='Force update of all semesters.'),
        make_option('--all', '-a', dest='all', action='store_true',
            default=False,
            help='Imports all semester course data that YACS can find.'),
    )

    def handle(self, *args, **options):
        if options.get('force'):
            print "Forcing update..."
        #with transaction.commit_on_success():
        #    import_courses(force=options.get('force', False))
        import_courses(
            force=options.get('force'),
            all=options.get('all')
        )

        #calculate cross listing
        add_cross_listing()
