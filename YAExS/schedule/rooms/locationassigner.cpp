/*
  ----- Location Assigner Implementation-----
  A class to assign locations to exams after they have been
  scheduled into time slots. Locations could be single rooms
  or a group of rooms.
*/

#include <algorithm>

#include "locationassigner.h"



// for debugging
#include <iostream>


// Assign exam locations to exams that have had times assigned. 
// After call, exam objects will have ExamLocation assigned.	

//Returns 0 if all goes well,
int LocationAssigner::assignLocations( 
	std::list<Exam> exams, 
	std::list<ExamLocation *> examLocations)
{
	if (exams.empty())
		return -1;

	// Sort the examLocations by decreasing size
	//sort( examLocations.begin(), examLocations.end(), ExamLocation::isLarger); // for vectors
	examLocations.sort(ExamLocation::isLarger);

	// Sort exams by time first, then within time by descreasing size
	//sort( exams.begin(), exams.end(), Exam::isLargerByTime); // for vectors
	exams.sort(Exam::isLargerByTime);
	
	std::list<ExamLocation *> availableLocs = examLocations;
	
	int currentSlot = exams.begin()->getTime();
	Exam currentExam;

	// For each exam, assign an available location stingily

	// This should be changed to prefer single rooms to grouped rooms 
	for (std::list<Exam>::iterator eit = exams.begin(); eit != exams.end(); eit++)
	{
		currentExam = *eit;


		// reset the availableLocs when we hit a new time slot
		if (currentSlot != currentExam.getTime())
		{
			std::cout << "now at timeslot " << currentSlot << std::endl;
			availableLocs = examLocations;
		}
		
		// now assign the smallest ExamLocation that will fit this exam.
		// this uses linear search for now, could use binary search later
		// (which I guess means switching to a data structur with slower remove).

		std::list<ExamLocation *>::iterator it = availableLocs.begin(); 
		while( it !=availableLocs.end() && (*it) -> willExamFit(currentExam) )
		{
			it++;
		}

		// cannot fit the exam into the biggest location!
		if (it == availableLocs.begin())
		{
			return 1;
		}

		// now we need to take one step back so we can fit;
		it--;	

		// cannot fit the exam into any locations!
		if ( ! (*it)->willExamFit(currentExam) )
		{
			return 2;
		}
		else
		{
			std::cout << "assigning location ";
			(*it) -> print();
			std::cout << " to exam:";
			currentExam.print();
			std::cout << std::endl;

			// assign the exam this location
			currentExam.assignLocation( *it );

			// location is no longer availabe
			availableLocs = removeOverlappingLocations( *it, availableLocs);
		}
		
		
		currentSlot = currentExam.getTime();
		std::cout << "updated the current time slot to " << currentSlot << std::endl;
	}

	return 0;
}

// May need to override == operator for Rooms
/*
class olap {    // function object that returns true when there is an overlap
  private:
    ExamLocation * loc;       // call for which to return true
  public:
    olap (ExamLocation * examLoc) : loc(examLoc){
    }

    bool operator() (ExamLocation * l) 
    {
        return overlaps(loc, l);
    }
};
*/
//NEED TO WRITE overlapsWithLoc predicate
bool everything (const ExamLocation* value) { return true; }

// Remove locations from locList that overlap with the given loc. 
// Can assume that locList is sorted in order by decreasing size.
std::list<ExamLocation *> LocationAssigner::removeOverlappingLocations( ExamLocation* loc, const  std::list<ExamLocation*> & locList)
{
	std::list<ExamLocation*> locations(locList);

	locations.remove_if( everything );
	//locations.remove_if( overlapsWithLoc(loc) );

	// we need to remove all locations l in locList
	// for which l.contains().contains(loc);
	

	return locations;
}



