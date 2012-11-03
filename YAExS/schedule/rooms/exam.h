/*
  ----- Exam Class Declaration-----
  A class that represents an exam.
*/

#ifndef _exam_h_
#define _exam_h_

#include "examlocation.h"
#include <string>

class Exam
{
	private:
		int numStudents;	
		ExamLocation * examLocation;
		std::string timeSlot;

	public:
		Exam(){ Exam(0); };

		Exam(int numberOfStudents) :
			numStudents(numberOfStudents),
			examLocation(NULL),
			timeSlot("") {};

		// the number of students taking this exam
		int size();

		void assignTime(std::string time);
		void assignLocation(ExamLocation * examLocation);

		bool hasLocation();
};
#endif
