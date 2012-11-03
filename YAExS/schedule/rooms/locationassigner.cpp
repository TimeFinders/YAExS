/*
  ----- Location Assigner Implementation-----
  A class to assign locations to exams after they have been
  scheduled into time slots. Locations could be single rooms
  or a group of rooms.
*/

#include <algorithm>

#include "Exam.h"
#include "ExamLocation.h"

// Assign exam locations to exams that have had times assigned. 
// After call, exam objects will have ExamLocation assigned.	
void LocationAssigner::assignLocations( 
	std::vector<Exam> exams, 
	std::vector<ExamLocation> examLocations)
{

	// Sort the examLocations by size
	sort( examLocations.begin(), examLocations.end(), ExamLocation::isLarger);	

	// Sort exams by time slot
	sort( exams.begin(), exams.end(), Exam::isEarlier);

	// For each exam
	for (int i = 0; i < exams.size(); i++)
	{
		//some data structure availableLocations for this time slot
	
	}
}


