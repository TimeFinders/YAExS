from django.conf.urls import patterns, include, url

# Uncomment the next two lines to enable the admin:
# from django.contrib import admin
# admin.autodiscover()

urlpatterns = patterns('Accounts.views',
    url(r'^$', 'main', name="home"),
    url(r'^scheduling/', 'scheduling', name="scheduling"),
    url(r'^cancel/', 'cancel', name="cancel"),
    url(r'^schedule/', 'displaySchedule', name='schedule'),

)
