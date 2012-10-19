from django.conf.urls import patterns, include, url

# Uncomment the next two lines to enable the admin:
# from django.contrib import admin
# admin.autodiscover()

urlpatterns = patterns('',
    url(r'^$', 'Accounts.views.home'),
    (r'^login/$', 'django_cas.views.login'),
    (r'^logout/$', 'django_cas.views.logout'),
    # Examples:
    # url(r'^$', 'YAExS.views.home', name='home'),
    # url(r'^YAExS/', include('YAExS.foo.urls')),

    # Uncomment the admin/doc line below to enable admin documentation:
    # url(r'^admin/doc/', include('django.contrib.admindocs.urls')),

    # Uncomment the next line to enable the admin:
    # url(r'^admin/', include(admin.site.urls)),
)
