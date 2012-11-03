/*
  ----- Exam Class Implementation-----
  A class that represents an exam
*/

#include "exam.h"

//for debugging
#include <iostream>

// the number of students taking this exam
int Exam::size()
{
	return numStudents;
}

void Exam::assignTime(int timeSlot)
{
	this->timeSlot = timeSlot;
}


void Exam::assignLocation(ExamLocation * location) 
{
	examLocation = location;
}

bool Exam::hasLocation()
{
	return examLocation != NULL;
}

bool Exam::isEarlier(Exam e1, Exam e2)
{
	return (    e1.timeSlot != -1 
		 && e2.timeSlot != -1
		 && e1.timeSlot < e2.timeSlot );
}

bool Exam::isLarger(Exam e1, Exam e2)
{
	return ( e1.numStudents > e2.numStudents );
}

bool Exam::isLargerByTime(Exam e1, Exam e2)
{
	if (e1.timeSlot < e2.timeSlot)
		return true;
	if (e1.timeSlot == e2.timeSlot)
		return ( e1.numStudents > e2.numStudents );
	else
		return false;
}

//for debugging
void Exam::print()
{
	std::cout << "students: " << numStudents << " time: " << timeSlot << std::endl;
}


