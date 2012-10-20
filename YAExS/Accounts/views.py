from django.shortcuts import render
from django.contrib.auth.decorators import login_required
from django.http import HttpRequest, HttpResponse


@login_required
def home(request):
    return render(request, 'accounts/home.html', {'user':request.user})
