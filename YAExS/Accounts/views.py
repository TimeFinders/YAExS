"""
Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
All rights reserved.
  
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  
The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
  
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
"""
from django.shortcuts import render, redirect
from django.contrib.auth.decorators import login_required, permission_required
from django.http import HttpRequest
from django.core.exceptions import PermissionDenied
from courses.models import Department, Course, Section, ExamMapping
from models import Schedule
from forms import Scheduler
from os import kill, remove
from YAExS.settings import PID_FILE, REG_FILE, OPT_EXEC, CONF_FILE
from subprocess import check_output, STDOUT, CalledProcessError


@login_required
def main(request):
    user = request.user

    if user.has_perm('Acccounts.registrar'):
    	from django.db.models import Q

    	message = request.session.get('message', None)
    	if message:
    		del request.session['message']

    	info = Course.objects.filter(Q(status='noexam') | Q(status='hasexam')).count()
    	exams = ExamMapping.objects.distinct('examID').filter(examID__course__status='hasexam').count()
    	is_schedule = Schedule.objects.all().exists()
    	info_all = Course.objects.all().count()
    	running = _isScheduleRunning()

        return render(request, 'accounts/registrar.html', {'info' : info, 'all' : info_all, 'running' : running, 'exams' : exams, 'schedule' : is_schedule, 'message' : message})

    elif user.has_perm('Accounts.scheduler'):
        depts = Department.objects.all()


        return render(request, 'accounts/scheduler.html', {'depts': depts})

    else:
    	raise PermissionDenied()

@permission_required('Accounts.registrar', raise_exception=True)
def scheduling(request):
	if not _isScheduleRunning():
		print "test"
		errors = None
		if request.method == 'POST':
			form = Scheduler(request.POST, request.FILES)
			if form.is_valid():
				with open(REG_FILE, 'w') as f:
					for c in form.cleaned_data['registrations'].chunks():
						f.write(c)
				try:
					check_output([OPT_EXEC, str(form.cleaned_data['examDays']), str(form.cleaned_data['examsPerDay']), REG_FILE, PID_FILE, CONF_FILE], stderr=STDOUT)
					request.session['message'] = 'Scheduling Started Successfully'
					return redirect( 'home')
				except CalledProcessError as e:
					errors = e.output.split("\n")
		else:
			form = Scheduler()

		return render(request, 'accounts/scheduling.html', {'schedule' : form, 'errors' : errors})
	else:

		return render(request, 'accounts/running.html')

@permission_required('Accounts.registrar', raise_exception=True)
def cancel(request):
	if _isScheduleRunning():
		try:
			with open(PID_FILE) as f:
				kill(int(f.read()), 9)
				request.session['message'] = 'Scheduling successfully canceled.'
		except:
			request.session['message'] = 'There was an error terminating scheduling.  Please contact @PERSON'

		try:
			remove(PID_FILE)
		except:
			pass
	else:
		request.session['message'] = 'Scheduler was not running.  Perhaps it completed while you were waiting?'

	return redirect('home')

@permission_required('Accounts.registrar', raise_exception=True)
def displaySchedule(request):

	info = []

	for sched in Schedule.objects.all().prefetch_related('section__mapping'):
		c = {'location' : sched.location, 'timeslot' : sched.timeslot, 'sections' : []}

		crns = (x[0] for x in sched.section.mapping.all().values_list('crn'))
		c['sections'].extend(list(Section.objects.filter(crn__in=crns).prefetch_related('course__department')))

		info.append(c)


	return render(request, 'accounts/schedule.html', {'schedule': info})


def _isScheduleRunning():
	try:
		with open(PID_FILE) as f:
			kill(int(f.read()), 0)
			return True;
	except:
		return False
