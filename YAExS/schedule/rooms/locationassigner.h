/*
  ----- Location Assigner Class Declaration-----
  A class to assign locations to exams after they have been
  scheduled into time slots. Locations could be single rooms
  or a group of rooms.
*/

#ifndef _locationassigner_h_
#define _locationassigner_h_

include <vector>

class Exam;
class ExamLocation;

class LocationAssigner 
{
public:
	// Assign exam locations to exams that have had times assigned. 
	// After call, exam objects will have ExamLocation assigned.	
	void assignLocations( std::vector<Exam> exams, std::vector<ExamLocation> examLocations);
}

#endif
