from django.shortcuts import render
from django.contrib.auth.decorators import login_required
from django.http import HttpRequest, HttpResponse


@login_required
def home(request):
    print request.user
    return HttpResponse(request.user)
