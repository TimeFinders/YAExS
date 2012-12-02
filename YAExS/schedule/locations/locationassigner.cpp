/*
  Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
  All rights reserved.
  
  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
  Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  
  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  
  The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
  
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
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
	if (examLocations.empty())
	{
		return -2;
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
	
		std::cout << "now trying to assign to exam " << std::endl; 
		 currentExam.print();

		ExamLocation * bestLoc = bestLocation(currentExam, availableLocs);
		activeLocs.push_back(bestLoc);
		if ( bestLoc == NULL)
		{
			return 1;
		}
		else
		{
            DEBUG_PRINT("activating location ");
			#ifdef debugMode
                bestLoc -> print();
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
					ExamLocation * frontLoc = activeLocs.front();
					std::cerr << "trying to assign ";
					frontLoc ->print();

					et->assignLocation(activeLocs.front());	

					if ( !et->hasLocation() )
					{
						std::cerr << "error exam ";
						et->print();
						std::cerr << "has no location: ";
						std::cerr << et->hasLocation();

						ExamLocation * theLoc = et->getLocation();
						if (theLoc == NULL)
						{
							std::cerr << " and indeed its location is null" << std::endl;
						}
						else
						{
							std::cerr << "but its location is not null" << std::endl;
							(et->getLocation())->print();
						}
						return 8;
					}
				}
				else
				{
					return 9;
				}
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
	// first check if the exam has no students
	if (e.size() == 0)
	{
		return Room::getEmptyRoom();
	}

	// if there are no availble locations we can't fit a best one.
	if (available.empty())
	{
		std::cerr << "no available locations at all" << std::endl;
		return NULL;
	}
	
	std::list<ExamLocation *>::iterator it = available.begin();
	
	// if exam will not fit in the first (biggest) location there is no hope
	if (!(*it)->willExamFit(e))	
	{
		std::cerr << "no available locations big enough" << std::endl;
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



// Assign null locations. returns true if assigned any
bool LocationAssigner::assignNullLocations( 
	std::list<Exam> & exams)
{
	bool assignedAny = false;
	for (std::list<Exam>::iterator it = exams.begin();
			it != exams.end(); it++)
	{
		if (!it->hasLocation())
		{	
			ExamLocation * loc =  Room::getNullRoom() ;
			it->assignLocation( loc );
			assignedAny = true;
		}
	}
	return assignedAny;
}
