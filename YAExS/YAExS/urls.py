from django.conf.urls import patterns, include, url

# Uncomment the next two lines to enable the admin:
from django.contrib import admin
admin.autodiscover()

urlpatterns = patterns('',
    url(r'^$', 'YAExS.views.home', name="default"),
    url(r'^accounts/', include('Accounts.urls')),
    url(r'^exam/', include('Exams.urls')),
    url(r'^login/$', 'django_cas.views.login', name="login"),
    url(r'^logout/$', 'django_cas.views.logout',name="logout"),
    # Examples:
    # url(r'^$', 'YAExS.views.home', name='home'),
    # url(r'^YAExS/', include('YAExS.foo.urls')),

    # Uncomment the admin/doc line below to enable admin documentation:
    # url(r'^admin/doc/', include('django.contrib.admindocs.urls')),

    # Uncomment the next line to enable the admin:
    url(r'^admin/', include(admin.site.urls)),
)

handler403 = 'YAExS.views.restricted'
