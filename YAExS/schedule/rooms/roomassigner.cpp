/*
  ----- Room Assigner Class Declaration-----
  A class to assign rooms to exams after they have been
  scheduled into time slots.
*/

 #include "roomassigner.h"

 // Default Constructor
RoomAssigner::RoomAssigner()
{
	dbReader = new DBReader();
}

// Assign Rooms to exams that have had times assigned. 
// Returns true if rooms were assigned successfully, else false.	
bool RoomAssigner::assignRooms() 
{
	// Get course information from database

	// Get room information (from database?)

	// Assign courses to rooms

	// Stick room information back in the database
}

// Destructor
RoomAssigner::~RoomAssigner() 
{
	// close the dbReader?
}