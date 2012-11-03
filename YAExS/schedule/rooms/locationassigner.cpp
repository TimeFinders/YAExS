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

// Assign exam locations to exams that have had times assigned. 
// After call, exam objects will have ExamLocation assigned.	
void LocationAssigner::assignLocations( 
	std::list<Exam> exams, 
	std::list<ExamLocation> examLocations)
{
	if (exams.empty())
		return;

	// Sort the examLocations by decreasing size
	sort( examLocations.begin(), examLocations.end(), ExamLocation::isLarger);	

	// Sort exams by time first, then within time by descreasing size
	sort( exams.begin(), exams.end(), Exam::isLargerByTime);

	
	list<Exam> availableLocations = examLocations;
	
	int currentSlot = exams[0].getTime();

	// For each exam
	for (int i = 0; i < exams.size(); i++)
	{
		// reset the availableLocations when we hit a new time slot
		if (currentSlot != exams[i].getTime())
		{
			availableLocations = examLocations;
		}
		
		// now assign the smallest ExamLocation that will fit this exam.
		// this uses linear search for now, could make much faster later.
		ExamLocation * bestLocation;
		
	
	}
}


