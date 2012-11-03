/*
  ----- Exam Class Implementation-----
  A class that represents an exam
*/

#include "exam.h"

// the number of students taking this exam
int Exam::size()
{
	return numStudents;
}

void Exam::assignTime(std::string time)
{
	timeSlot = time;
}


void Exam::assignLocation(ExamLocation * location) 
{
	examLocation = location;
}

bool Exam::hasLocation()
{
	return examLocation != NULL;
}


