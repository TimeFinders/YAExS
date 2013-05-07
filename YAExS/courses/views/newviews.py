"""
Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
All rights reserved.
  
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  
The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
  
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
"""
from django.shortcuts import redirect
from django.views.generic import TemplateView
from django.contrib.sites.models import Site

from courses import models
from courses.utils import DAYS
from courses.encoder import default_encoder
from courses.views.decorators import Renderer

render = Renderer(template_prefix='courses/')


@render(template_prefix='', template_name='robots.txt')
def robots_txt(request):
    return {
        'context': {
            'semesters': models.Semester.objects.all(),
            'site': Site.objects.get_current()
        },
        'mimetype': 'text/plain',
    }

################ VIEWS loaded by the a normal browser ###########################


class AjaxView(TemplateView):
    # template_name needs to be specified
    def get_context_data(self, **kwargs):
        context = super(AjaxView, self).get_context_data(**kwargs)
        year = context['sem_year'] = self.kwargs.get('year')
        month = context['sem_month'] = self.kwargs.get('month')
        context['semester'] = models.Semester.object.get(year=year, month=month)
        return context


@render(template_name='semester_list.html')
def semester_list(request, year=None, month=None):
    semesters = models.Semester.objects.optional_filter(year=year)
    return {
        'context': {
            'sem_year': year,
            'semesters': semesters
        }
    }


def redirect_to_latest_semester(request):
    semester = models.Semester.objects.all()[0]
    return redirect('departments', semester.year, semester.month)


@render(template_name='department_list.html')
def department_list(request, year=None, month=None):
    departments = models.Department.objects.optional_filter(
        semesters__year=year,
        semesters__month=month
    )
    semester = None
    if year and month:
        semester = models.Semester.objects.get(year=year, month=month)
    return {
        'context': {
            'sem_year': year,
            'sem_month': month,
            'semester': semester,
            'departments': departments,
        }
    }


@render()
def course_list_by_dept(request, code=None, year=None, month=None, is_search=False):
    query = request.GET.get('q')
    use_partial_template = request.GET.get('partial')

    semester = models.Semester.objects.get(year=year, month=month)
    courses = models.Course.objects.filter(semesters=semester)
    department = None
    if code:  # filter by department if we can
        department = models.Department.objects.get(code=code, semesters=semester)
        courses = courses.by_department(department)

    if is_search:  # filter by search query
        if not query:
            courses = courses.order_by('department__code', 'number')
        else:
            courses = courses.search(query)

    # full_select needs to be last (forces evaluation)
    courses = courses.select_related('department').full_select(year, month)

    template_name = 'course_list.html'
    if use_partial_template:
        template_name = '_' + template_name

    return {
        'template_name': template_name,
        'context': {
            'days_of_the_week': DAYS,
            'sem_year': semester.year,
            'sem_month': semester.month,
            'semester': semester,
            'department': department,
            'courses': courses,
        }
    }
