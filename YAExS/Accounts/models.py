from django.db import models
from django.contrib.auth.models import User

# Create your models here.

class Schedule(models.Model):
	section = models.ForeignKey('courses.Section', related_name="schedule")
	timeslot = models.IntegerField()
	location = models.CharField(max_length=300)

class User(User):

	class Meta:
		permissions=(
		             ('registrar', "Members of the Registar's Office"),
		             ('scheduler', "Department Schedulers")
		             )