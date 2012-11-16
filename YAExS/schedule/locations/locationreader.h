/*
  ----- Location Reader Class Declaration-----
  A class to read in information about rooms and room 
  groups to create exam location objects.
*/

#include <list>
#include <string>
#include <fstream>
#include <unordered_map>
#include "ctype.h"


#include "examlocation.h"
#include "room.h"
#include "roomgroup.h"  


class LocationReader
{
public:
	// read in both rooms and room groups, return a list of Rooms followed by RoomGroups
	std::list<ExamLocation*> readLocations(std::string roomFilePath, std::string roomGroupFilePath);

private:

	// the rooms from the room file. used to create room groups.
	std::unordered_map<Room::ROOM_ID, Room*> rooms_;


	// read in rooms. you must read rooms before the room groups.
	std::list<ExamLocation*>  readRooms(std::string roomFilePath);

	// read in room groups. you must read rooms before the room groups.
	std::list<ExamLocation*>  readRoomGroups(std::string roomGroupFilePath);

	// remove non-alphanumeric characters at the end of a roomNumber read from a file;
	std::string fixRoomNumber( std::string roomNumber);

	// return a room_id used to create a Room object from the building a room number
	Room::ROOM_ID roomID(std::string building, std::string roomNumber);

};