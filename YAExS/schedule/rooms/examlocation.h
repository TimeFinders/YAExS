/*
  ----- Exam Location Class Declaration-----
  An abstract class that represents a location that can 
  hold an exam. This could be a single room or a group 
  of rooms. 
*/

#ifndef _examlocation_h_
#define _examlocation_h_

class Exam;


class ExamLocation 
{		
	public:
		// Return true if the given exam can fit in this location, false otherwise.
		virtual bool willExamFit(Exam exam) = 0;

		static bool isLarger(ExamLocation * e1, ExamLocation * e2)
			{ return e1->getCapacity() > e2->getCapacity(); };

		//for debugging
		virtual void print() = 0;
		
	protected:
		virtual int getCapacity() = 0;
		
};
#endif
