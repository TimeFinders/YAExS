"""
Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
All rights reserved.
  
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  
The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
  
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
"""
from django.conf.urls.defaults import patterns, include, url
from django.views.decorators.cache import cache_page

from courses import views
from courses.views import newviews as nviews

cache_duration = 60 * 10

urlpatterns = patterns('',
    url(r'^$', nviews.semester_list, name='semesters'),
    url(r'^(?P<year>[1-9]\d*)/$', cache_page(nviews.semester_list, cache_duration), name='semesters'),

    url(r'^(?P<year>[1-9]\d*)/(?P<month>[1-9]\d*)/$', cache_page(nviews.department_list, cache_duration), name='departments'),
    url(r'^(?P<year>[1-9]\d*)/(?P<month>[1-9]\d*)/search/$', cache_page(nviews.course_list_by_dept, cache_duration), {'is_search': True}, name='search-all-courses'),
    url(r'^(?P<year>[1-9]\d*)/(?P<month>[1-9]\d*)/(?P<code>[A-Z]+)/$', cache_page(nviews.course_list_by_dept, cache_duration), name='courses-by-dept'),

    # courses
    url(r'^(?P<year>[1-9]\d*)/(?P<month>[1-9]\d*)/(?P<code>[A-Z]+)/(?P<number>[1-9]\d*)/$', views.CourseDetailView.as_view(), name='course'),

    # pretty much a static page..
    url(r'^(?P<year>[1-9]\d*)/(?P<month>[1-9]\d*)/selected/$', cache_page(views.SelectedCoursesListView.as_view(), 60 * 3600), name='selected-courses'),
    # actions
    #url(r'^(?P<year>[1-9]\d*)/(?P<month>[1-9]\d*)/select/$', views.SelectCoursesView.as_view(), name='select-courses'),
    #url(r'^(?P<year>[1-9]\d*)/(?P<month>[1-9]\d*)/deselect/$', views.DeselectCoursesView.as_view(), name='deselect-courses'),
    # other possible urls to have:
    # /semesters/ => view all past semesters
    # /<year>/ => view semesters for a given year
    # /<year>/<month>/<code>/<number>/ => view course by number
    # /<year>/<month>/course-<id>/ => view course by id
    # /<year>/<month>/sections/<crn>/ => view section by crn
)
