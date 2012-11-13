/*
  ----- Location Assigner Class Declaration-----
  A class to assign locations to exams after they have been
  scheduled into time slots. Locations could be single rooms
  or a group of rooms.
*/

#ifndef _locationassigner_h_
#define _locationassigner_h_

#include <list>

#include "exam.h"
//#include "examlocation.h"
#include "room.h"

class LocationAssigner 
{
public:
	// Assign exam locations to exams that have had times assigned. 
	// After call, exam objects will have ExamLocation assigned.
	//Returns 0 if all goes well	
	static int assignLocations( std::list<Exam> & exams, std::list<ExamLocation *> examLocations);

private:
	// Return the "best" location for exam e from the available list.
	// Returns NULL if no location is suitable.
	static ExamLocation* bestLocation(Exam e, std::list<ExamLocation*> available);

	// remove from locList all locations that overlap with loc. E.g. if loc is AE214
	// and there is a RoomGroup[AE214, AE215, AE216] it will be removed.
	static std::list<ExamLocation *> removeOverlappingLocations( const ExamLocation* loc, const std::list<ExamLocation*>  & locList);


};

#endif
