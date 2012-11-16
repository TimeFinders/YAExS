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
	// read in both rooms and room groups
	std::list<ExamLocation*> readLocations(std::string roomFilePath, std::string roomGroupFilePath);

private:
	// read in rooms. you must read rooms before the room groups.
	std::list<ExamLocation*>  readRooms(std::string roomFilePath);

	// read in room groups. you must read rooms before the room groups.
	std::list<ExamLocation*>  readRoomGroups(std::string roomGroupFilePath);

	std::unordered_map<Room::ROOM_ID, Room*> rooms_;
};