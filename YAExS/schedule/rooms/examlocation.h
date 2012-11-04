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

/*
bool operator== (const Room & r1, const Room & r2)
{	
	return (r1.building == r2.building && r1.roomAddress == r2.roomAddress);
}
*/

class ExamLocation 
{		
	public:
		// Return true if the given exam can fit in this location, false otherwise.
		virtual bool willExamFit(Exam exam) = 0;

		static bool isLarger(ExamLocation * e1, ExamLocation * e2)
			{ return e1->getCapacity() > e2->getCapacity(); };

		// Return true if this overlaps in space with e
		//virtual bool isOverlapping(ExamLocation * e) = 0;

		// Returns a vector of the constituent Room(s)
		virtual std::vector<Room> contains() = 0;
 
		//for debugging
		virtual void print() = 0;
	
		
	protected:
		virtual int getCapacity() = 0;
		
};
#endif
