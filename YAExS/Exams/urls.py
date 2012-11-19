from django.conf.urls import patterns, include, url

# Uncomment the next two lines to enable the admin:
from django.contrib import admin
admin.autodiscover()

urlpatterns = patterns('Exams.views',
    url(r'^hasexam/', 'updateExam', {'status': 'hasexam'}, name="hasexam"),
    url(r'^unknown/', 'updateExam', {'status': 'unknown'}, name="unknown"),
    url(r'^noexam/', 'updateExam', {'status': 'noexam'}, name="noexam"),
)
