/*
----- Location Reader Class Implementation-----
A class to read in information about rooms and room 
groups to create exam location objects.
*/

#include "locationreader.h"
#include <stdio.h>
#include <string.h>
#include <sstream>

std::list<ExamLocation*> LocationReader::readLocations(std::string roomFilePath, std::string roomGroupFilePath)
{	
	std::list<ExamLocation*> rooms = readRooms(roomFilePath);
	std::list<ExamLocation*> roomGroups  = readRoomGroups(roomGroupFilePath);
	
	rooms.splice(rooms.end(), roomGroups);
	return rooms;
}

std::list<ExamLocation*>  LocationReader::readRooms(std::string roomFilePath)
{
	const char * roomFileP = roomFilePath.c_str();
	std::cout << "looking for room file at:" << roomFileP << std::endl;
	std::ifstream roomFile(roomFileP);

	if(roomFile.is_open())
	{
		std::list<ExamLocation *> rooms;
		std::stringstream iss;

		std::string line;
		std::string building, room, capacity;
		Room::ROOM_ID roomName;
		int roomSize;
		while ( roomFile.good() )
	    {
	    	// read in a whole line
	    	while (getline(roomFile, line))
	    	{
	    		iss << line;
		     	getline (iss, building, ',');
		     	getline (iss, room, ',');
		     	getline (iss, capacity, ',');
		     	
		     	roomName = building + "_" + room;
		     	std::istringstream buffer(capacity);
		     	buffer >> roomSize;

		     	//std::cout << "\tcreating a new room with name " << roomName;
		     	//std::cout << " and size " << roomSize << std::endl;

		     	ExamLocation * room = new Room(roomName, roomSize);
		     	rooms.push_back(room);
		     	
		     	iss.clear();
	     	}
	     	
	    }
		roomFile.close();

		return rooms;
	}
	else
	{
		std::cerr << "could not open rooms file." << std::endl;
		std::list<ExamLocation *> emptyList;
		return emptyList;
	}
}

std::list<ExamLocation*>  LocationReader::readRoomGroups(std::string roomGroupFilePath)
{
	const char * roomGroupFileP = roomGroupFilePath.c_str();
	std::ifstream roomGroupFile(roomGroupFileP);
	if(roomGroupFile.is_open())
	{
		std::list<ExamLocation *> roomGroups;

		std::string line;
		std::string building, room, capacityString;
		while ( roomGroupFile.good() )
	    {
	    	getline (roomGroupFile,line);
	    	std::cout << line << std::endl;

	    }
		roomGroupFile.close();

		return roomGroups;
	}
	else
	{
		std::cerr << "could not open room groupsfile." << std::endl;
		std::list<ExamLocation *> emptyList;
		return emptyList;
	}
}