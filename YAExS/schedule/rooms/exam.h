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
		int timeSlot;

	public:
		Exam(){ Exam(-1); };

		Exam(int numberOfStudents) :
			numStudents(numberOfStudents),
			examLocation(NULL),
			timeSlot(-1) {};

		// the number of students taking this exam
		int size();

		void assignTime(int timeSlot);
		void assignLocation(ExamLocation * examLocation);

		bool hasLocation();

		static bool isEarlier( Exam e1, Exam e2);
		static bool isLarger( Exam e1, Exam e2);

		// for debugging
		void print();
};
#endif
