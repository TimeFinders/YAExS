from django.contrib.auth.decorators import permission_required
from django.views.decorators.csrf import csrf_exempt
from django.http import HttpResponse, Http404
from courses.models import Course


@permission_required('Accounts.scheduler', raise_exception=True)
@csrf_exempt
def updateExam(request, status=None):
    if request.method == 'POST':
        id = request.POST.get('id', None)
        if id:
            a = Course.objects.get(id=id)
            a.status = status
            a.save()
        else:
            raise Http404()
    return HttpResponse()
