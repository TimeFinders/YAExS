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

//#define debugMode



// Assign exam locations to exams that have had times assigned. 
// After call, exam objects will have ExamLocation assigned.	

// This function is huge. Should split up into smaller functions.

//Returns 0 if all goes well,
int LocationAssigner::assignLocations( 
	std::list<Exam> & exams, 
	std::list<ExamLocation *> examLocations)
{
	if (exams.empty())
	{
		return -1;
	}

	// Sort the examLocations by decreasing size
	examLocations.sort(ExamLocation::isLarger);

	// Sort exams by time first, then within time by descreasing size
	exams.sort(Exam::isLargerByTime);


	TimeSlot currentSlot;
	Exam currentExam;
	
	// ExamLocations available for a particular time slot	
	std::list<ExamLocation *> availableLocs = examLocations;
	
	// The ExamLocations that will actually be used for a particular time slot
	std::list<ExamLocation *> activeLocs;

	// the first exam for this time slot (biggest)
	std::list<Exam>::iterator firstThisSlot = exams.begin();


	// For each exam, assign an available location stingily

	for (std::list<Exam>::iterator eit = exams.begin(); eit != exams.end(); eit++)
	{
		currentExam = *eit;
		
		ExamLocation * bestLoc = bestLocation(currentExam, availableLocs);
		activeLocs.push_back(bestLoc);
		if ( bestLoc == NULL)
		{
			return 1;
		}
		else
		{
			#ifdef debugMode
			  	std::cout << "activating location ";
				bestLoc -> print();
				std::cout << std::endl;
			#endif
			

			// location is no longer availabe
			availableLocs = removeOverlappingLocations( bestLoc, availableLocs);
		}
		
		currentSlot = currentExam.getTime();
		
		// detect if at the last exam of a timeslot
		std::list<Exam>::iterator next = eit;
		next++;
		if ( next == exams.end() || next->getTime() != currentSlot )
		{
			//std::cout << "LAST EXAM OF THIS TIME SLOT " << std::endl;

			// assign all the exams to activeLocs, biggest exams
			// to biggest active location
			activeLocs.sort(ExamLocation::isLarger);

			//std::cout << "at time slot " << currentSlot.toPrint() << " the active locs are" << std::endl;	

			for (std::list<Exam>::iterator et = firstThisSlot; et != next; et++)
			{
				#ifdef debugMode
					activeLocs.front()->print();	
					std::cout << "\tassigned to ";
					et->print();
				#endif
				


				if (!activeLocs.empty())
				{	
					// assign the exam
					et->assignLocation(activeLocs.front());	

					if ( !et->hasLocation() )
						return 8;

				}
				else
					return 9;
	
				activeLocs.pop_front();
			}
			if (!activeLocs.empty())
			{
				return 10;
			}			
			
			// reset everything for next time slot, if not at end already
			if (next != exams.end() )	
			{		
			//	std::cout << "\n\nmoving on to timeslot " << next->getTime().toPrint() << std::endl;
				availableLocs = examLocations;
				firstThisSlot = next;
			}
			else
			{
			//	std::cout << "\nall time slots complete\n" << std::endl;
			}

		}//end if check for end of time slot

	}//end for loop over exams		

	return 0;
}

// find the "best" ExamLocation for the exam e from the list of available locations
// which are sorted by non-increasing sizes
// if none are good, returns null;

// picks the room location that will fit with the fewest number of constitutent rooms
// (can tell if single room by size of (ExamLocation*)->contains()
ExamLocation* LocationAssigner::bestLocation(Exam e, std::list<ExamLocation*> available)
{
	// if there are no availble locations we can't fit a best one.
	if (available.empty())
	{
		return NULL;
	}
	
	std::list<ExamLocation *>::iterator it = available.begin();
	
	// if exam will not fit in the first (biggest) location there is no hope
	if (!(*it)->willExamFit(e))	
	{
		return NULL;
	}
	
	std::list<ExamLocation *>::iterator next = it;
	next++;

	ExamLocation * bestLoc = *it;

	// number of constitutent rooms in the bestLoc found so far
	unsigned bestSize = bestLoc->contains().size();

	// keep updating bestLoc when find a smaller capacity location that
	// will fit the exam with the same number of rooms or fewer.
	while (next != available.end() && (*next)->willExamFit(e) )
	{
		it++;
		next++;
		if ( (*it)->contains().size() <= bestSize )
		{	
			bestLoc = *it;
			bestSize = bestLoc->contains().size(); 
		}
	}
	
	return bestLoc;
}




// function object that returns true when there is an overlap
class olap {    
  private:
    const ExamLocation * loc;       // call for which to return true
  public:
    olap (const ExamLocation * examLoc) : loc(examLoc){};

    bool operator() (ExamLocation * l) { return Room::overlaps(loc, l); };
};

// Remove locations from locList that overlap with the given loc. 
// Can assume that locList is sorted in order by decreasing size.
std::list<ExamLocation *> LocationAssigner::removeOverlappingLocations( 
	const ExamLocation* loc, const  std::list<ExamLocation*> & locList )
{
	std::list<ExamLocation*> locations(locList);
	locations.remove_if( olap(loc) );
	return locations;
}



