/*
  ----- Location Assigner Implementation-----
  A class to assign locations to exams after they have been
  scheduled into time slots. Locations could be single rooms
  or a group of rooms.
*/

#include <algorithm>

#include "locationassigner.h"

#include "exam.h"
#include "examlocation.h"

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
		return;

	// Sort the examLocations by decreasing size
	sort( examLocations.begin(), examLocations.end(), ExamLocation::isLarger);	

	// Sort exams by time first, then within time by descreasing size
	sort( exams.begin(), exams.end(), Exam::isLargerByTime);

	
	list<ExamLocation *> availableLos = examLocations;
	
	int currentSlot = exams[0].getTime();
	Exam currentExam;

	// For each exam, assign an available location stingily

	// This should be changed to prefer single rooms to grouped rooms 
	for (int i = 0; i < exams.size(); i++)
	{
		currentExam = exams[i];


		// reset the availableLocs when we hit a new time slot
		if (currentSlot != currentExam.getTime())
		{
			std::cout << "now at timeslot " << currentSlot;
			availableLocs = examLocations;
		}
		
		// now assign the smallest ExamLocation that will fit this exam.
		// this uses linear search for now, could use binary search later
		// (which I guess means switching to a data structur with slower remove).

		list<ExamLocation *>::iterator it = availableLocs.begin(); 
		while( it !=availableLocs.end() && (*it) -> willExamFit(currentExam);
		{
			it++;
		}
	
		// cannot fit the exam into any locations!
		if (it == availableLocs.end())
		{
			return 1;
		}
		else
		{
			std::cout << "assigning location ";
			(*it) -> print();
			std::cout << " to exam";
			currentExam.print();

			// assign the exam this location
			currentExam.assignLocation( *(*it) );
			// location is no longer availabe
			availableLocs.remove(*it);
		}
		
		
		currentSlot = exams[i].getTime();
		std::cout << "updated the current time slot to " << currentSlot;
	}
}


