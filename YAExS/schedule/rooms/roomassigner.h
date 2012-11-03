/*
  ----- Room Assigner Class Declaration-----
  A class to assign rooms to exams after they have been
  scheduled into time slots.
*/

#ifndef _roomassigner_h_
#define _roomassigner_h_

class RoomAssigner 
{
public:
	// Constructor
	RoomAssigner();

	// Assign Rooms to exams that have had times assigned. 
	// Returns true if rooms were assigned successfully, else false.	
	bool assignRooms();

	// Destructor
	~RoomAssigner();

private:
	DBReader dbReader;
}

#endif