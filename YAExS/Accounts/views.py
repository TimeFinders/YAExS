from django.shortcuts import render
from django.contrib.auth.decorators import login_required
from django.http import HttpRequest, HttpResponse


@login_required
def main(request):
    user = request.user

    if user.is_staff:
        return render(request, 'accounts/registrar.html')

    else:
        return render(request, 'accounts/scheduler.html', {'depts': ('ARCH','MECH', 'MATH', 'CSCI', 'ITWS')})
