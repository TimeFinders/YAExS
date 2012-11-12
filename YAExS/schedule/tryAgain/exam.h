/*
  ----- Exam Class Declaration-----
  A class that represents an exam.
*/

#ifndef _exam_h_
#define _exam_h_

class ExamLocation;

#include <string>
#include "timeslot.h"


class Exam
{
	private:
		int numStudents;	
		ExamLocation * examLocation;
		TimeSlot timeSlot;
		std::string name;

	public:

		typedef std::string EXAM_ID;

		Exam(){ Exam(-1, ""); };

		Exam(int numberOfStudents, std::string name) :
			numStudents(numberOfStudents),
			examLocation(NULL),
			timeSlot(-1),
			name(name) {};

		// the number of students taking this exam
		int size();

		void assignTime(TimeSlot timeSlot);
		TimeSlot getTime(){return timeSlot;};
		void assignLocation(ExamLocation * examLocation);
		ExamLocation * getLocation() { return examLocation; };

		bool hasLocation();

		// this should return a UNIQUE id for this exam
		EXAM_ID getId() const { return name; };

		

		static bool isEarlier( Exam e1, Exam e2);
		static bool isLarger( Exam e1, Exam e2);

		// for sorting by timeslot, then within time slot by non-decreasing size
		static bool isLargerByTime( Exam e1, Exam e2);

		// for debugging
		void print();
};
#endif
