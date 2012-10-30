from django.shortcuts import render
from django.contrib.auth.decorators import login_required
from django.http import HttpRequest, HttpResponse


@login_required
def updateExam(request, status=None):
    if request.method == 'POST':
        pass

    return HttpResponse(status)

