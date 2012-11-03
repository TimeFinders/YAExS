/*
  ----- Location Assigner Implementation-----
  A class to assign locations to exams after they have been
  scheduled into time slots. Locations could be single rooms
  or a group of rooms.
*/

#include "Exam.h"
#include "ExamLocation.h"

// Assign exam locations to exams that have had times assigned. 
// After call, exam objects will have ExamLocation assigned.	
void LocationAssigner::assignLocations( 
	std::vector<Exam> exams, 
	std::vector<ExamLocation> examLocations)
{

	// Sort the examLocations by size
	
	// Sort exams by time slot

	// For each time slot, assign locations to exams greedily by size


}


