from django import forms

class Scheduler(forms.Form):
	examsPerDay = forms.IntegerField(initial=4, label="Exams Per Day")
	examDays = forms.IntegerField(initial=5, label="Exam Days", help_text='Number of days during which exams take place')
	registrations = forms.FileField(label="Student Registration File", help_text="The file MUST be a CSV file in the form studentID,crn with NO headers.")