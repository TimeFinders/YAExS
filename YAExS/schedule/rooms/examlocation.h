/*
  ----- Exam Location Class Declaration-----
  An abstract class that represents a location that can 
  hold an exam. This could be a single room or a group 
  of rooms. 
*/

#ifndef _examlocation_h_
#define _examlocation_h_

#include<vector>

// for debugging
#include <iostream>
#include <typeinfo>

class Exam;
class Room;

class ExamLocation 
{		
	public:
		// Return true if the given exam can fit in this location, false otherwise.
		virtual bool willExamFit(Exam exam) const = 0;

		static bool isLarger(ExamLocation * e1, ExamLocation * e2)
			{ return e1->getCapacity() > e2->getCapacity(); };

		// Returns a vector of the constituent Room(s)
		virtual std::vector<Room> contains() const = 0;
 
		//for debugging
		virtual void print() const = 0;


	
		
	protected:
		virtual int getCapacity() const = 0;
		
};
#endif
