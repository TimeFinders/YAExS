/*
  ----- Location Reader Class Declaration-----
  A class to read in information about rooms and room 
  groups to create exam location objects.
*/

#include <list>
#include "examlocation.h"
#incldue "room.h"
#include "roomgroup.h"  

class locationReader
{
private:




public:
	locationReader();

	std::list<ExamLocation*> readLocations(std::string roomFilePath, std::string roomGroupFilePath)
	{	
		std::list<ExamLocation*> rooms = readRooms(roomFilePath);

	}

	std::list<ExamLocation*>  readRooms(std::string roomFilePath)

	std::list<ExamLocation*>  readRoomGroups(std::string roomGroupFilePath)



}