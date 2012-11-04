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

	std::list<ExamLocation *> activeLocs;
	std::list<Exam>::iterator firstThisSlot = exams.begin();

	// For each exam, assign an available location stingily

	// This should be changed to prefer single rooms to grouped rooms 
	for (std::list<Exam>::iterator eit = exams.begin(); eit != exams.end(); eit++)
	{
		currentExam = *eit;

		// reset the availableLocs when we hit a new time slot
		if (currentSlot != currentExam.getTime())
		{			
			// assign all the exams to activeLocs, biggest exams
			// to biggest active location
			activeLocs.sort(ExamLocation::isLarger);

			std::cout << "at time slot " << currentSlot << " the active locs are" << std::endl;	

			for (std::list<Exam>::iterator et = firstThisSlot; et!= eit; et++)
			{
				activeLocs.front()->print();	
				std::cout << "\tassigned to ";
				et->print();

				et->assignLocation(activeLocs.front());	
				
				activeLocs.pop_front();
			}
			if (!activeLocs.empty())
			{
				return 10;
			}			

			std::cout << "\n\nswitched to timeslot " << currentExam.getTime() << std::endl;
			availableLocs = examLocations;
			//activeLocs = new std::list<ExamLocation *>();
			firstThisSlot = eit;
		}
		
		// now assign the smallest ExamLocation that will fit this exam.
		
		ExamLocation * bestLoc = bestLocation(currentExam, availableLocs);
		activeLocs.push_back(bestLoc);
		if ( bestLoc == NULL)
		{
			return 1;
		}
		else
		{
			std::cout << "activating location ";
			bestLoc -> print();
			std::cout << std::endl;

			// location is no longer availabe
			availableLocs = removeOverlappingLocations( bestLoc, availableLocs);
		}
		
		
		currentSlot = currentExam.getTime();
		std::cout << "updated the current time slot to " << currentSlot << std::endl;
	}

	return 0;
}

// find the "best" ExamLocation for the exam e from the list of available locations
// if none are good, returns null;

// currently is stingy. May want to change to prefer single rooms to double rooms.
ExamLocation* LocationAssigner::bestLocation(Exam e, std::list<ExamLocation*> available)
{
		std::list<ExamLocation *>::iterator it = available.begin(); 
		while( it !=available.end() && (*it) -> willExamFit(e) )
		{
			it++;
		}

		// cannot fit the exam into the biggest location!
		if (it == available.begin())
		{
			return NULL;
		}

		// now we need to take one step back so we can fit;
		it--;	

		// cannot fit the exam into any locations!
		if ( ! (*it)->willExamFit(e) )
		{
			return NULL;
		}

		return (*it);
}


// function object that returns true when there is an overlap
class olap {    
  private:
    const ExamLocation * loc;       // call for which to return true
  public:
    olap (const ExamLocation * examLoc) : loc(examLoc){
    }

    bool operator() (ExamLocation * l) 
    {
        return Room::overlaps(loc, l);
    }
};



// Remove locations from locList that overlap with the given loc. 
// Can assume that locList is sorted in order by decreasing size.
std::list<ExamLocation *> LocationAssigner::removeOverlappingLocations( const ExamLocation* loc, const  std::list<ExamLocation*> & locList)
{
	std::list<ExamLocation*> locations(locList);

	locations.remove_if( olap(loc) );

	return locations;
}



