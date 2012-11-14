from django.db import models

# Create your models here.

class Schedule(models.Model):
	section = models.ForeignKey('courses.Section', related_name="schedule")
	timeslot = models.IntegerField()
	room = models.CharField(max_length=150)
