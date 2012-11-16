/*
  ----- Location Reader Class Declaration-----
  A class to read in information about rooms and room 
  groups to create exam location objects.
*/

#include <list>
#include <string>
#include <fstream>

#include "examlocation.h"
#include "room.h"
#include "roomgroup.h"  


class LocationReader
{
public:
	// read in both rooms and room groups
	static std::list<ExamLocation*> readLocations(std::string roomFilePath, std::string roomGroupFilePath);

	// read in rooms
	static std::list<ExamLocation*>  readRooms(std::string roomFilePath);

	// read in room groups
	static std::list<ExamLocation*>  readRoomGroups(std::string roomGroupFilePath);

};