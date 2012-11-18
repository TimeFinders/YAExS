/*
----- Location Reader Class Implementation-----
A class to read in information about rooms and room 
groups to create exam location objects.
*/

#include "locationreader.h"
#include <stdio.h>
#include <string.h>
#include <sstream>

// read in both rooms and room groups, return a list of Rooms and RoomGroups
std::list<ExamLocation*> LocationReader::readLocations(std::string roomFilePath, std::string roomGroupFilePath)
{	
	std::list<ExamLocation*> rooms = readRooms(roomFilePath);
	if (rooms.empty())
	{
		std::cerr << "error, no rooms were read. will not attempt to read room groups." << std::endl;
	}
	else
	{
		std::list<ExamLocation*> roomGroups  = readRoomGroups(roomGroupFilePath);
		rooms.splice(rooms.begin(), roomGroups);
	}
	return rooms;
}

// read in room information from the room file at roomFilePath and create rooms.
// roomFilePath should be a directory to a file with format
// building,room,capacity
// e.g. AmosEaton,214,81
// the capacity should be the number of students allowed for an exam in that room
// current RPI policy is half the number of seats
//
// you must read the rooms before you can read the room groups
// so call readRooms before readRoomGroups
std::list<ExamLocation*>  LocationReader::readRooms(std::string roomFilePath)
{
	const char * roomFileP = roomFilePath.c_str();
	std::ifstream roomFile(roomFileP);

	if(roomFile.is_open())
	{
		std::list<ExamLocation *> roomList;
		std::stringstream iss;

		std::string line;
		std::string building, room, capacity;
		Room::ROOM_ID roomName;
		int roomSize;
		while ( roomFile.good() )
	    {
	    	// read in a whole line (one room)
	    	while (getline(roomFile, line))
	    	{
	    		// split information about the room
	    		iss << line;
		     	getline (iss, building, ',');
		     	getline (iss, room, ',');
		     	getline (iss, capacity, ',');
		     	
		     	roomName = roomID(building, room);
		     	std::istringstream buffer(capacity);
		     	buffer >> roomSize;

		     	//create a room object
		     	Room * theRoom = new Room(roomName, roomSize);
		     	// add to rooms_ map for the RoomGroup objects to use
		     	rooms_[roomName] = theRoom;

		     	ExamLocation * elRoom = theRoom;
		     	roomList.push_back(elRoom);
		     	
		     	// clear the stream for the next line
		     	iss.clear();
	     	}
	     	
	    }
		roomFile.close();

		return roomList;
	}
	else
	{
		std::cerr << "could not open rooms file." << std::endl;
		std::list<ExamLocation *> emptyList;
		return emptyList;
	}
}



// read in room group information from the room group file at roomGroupFilePath and create room groups.
// roomGroup FilePath should be a directory to a file with format:
// building room,building room, building room
// there can be as many buildings in a line as necessary.
// e.g. AmosEaton 214,Troy 2012,Troy 2018,Low 4050,Sage 4101,Sage 5101
// these rooms must be listed in the rooms file to get the capacities.
//
// you must read the rooms before you can read the room groups
// so call readRooms before readRoomGroups
std::list<ExamLocation*>  LocationReader::readRoomGroups(std::string roomGroupFilePath)
{
	if (rooms_.empty())
	{
		std::cerr << "rooms map is empty. you must call readRooms before readRoomGroups" << std::endl;
	}

	const char * roomGroupFileP = roomGroupFilePath.c_str();
	std::ifstream roomGroupFile(roomGroupFileP);

	if(roomGroupFile.is_open())
	{
		std::list<ExamLocation *> roomGroups;
		std::stringstream liness, roomss;

		while ( roomGroupFile.good() )
	    {
	    	
	    	// read in a whole line ( a single room group)
	    	std::string line;
	    	while (getline(roomGroupFile, line))
	    	{
	    		std::vector<Room> containedRooms;
	    		liness << line;

	    		// read in information about a constituent room
	    		std::string roomString;
	    		while( getline(liness, roomString, ','))
	    		{
	    			roomss << roomString;

	    			std::string building, roomNumber;
			     	getline(roomss, building, ' ');
			     	getline(roomss, roomNumber, ' ');

			     	// clear out for next time;
			     	roomss.clear();
			     	

			     	if (roomNumber.length() < 1)
			     	{
			     		std::cerr << "invalid room number on line:" << line << std::endl;
			     		continue;
			     	}

			     	//add the room to the contained rooms list if valid
			     	roomNumber = fixRoomNumber( roomNumber);
			        Room::ROOM_ID roomName = roomID(building, roomNumber);

			     	Room * room = rooms_[roomName];
			     	if (room == NULL)
			     	{
			     		std::cerr << "error, room " << roomName;
			     		std::cerr << " not found in room list" << std::endl;
			     		continue;
			     	}

			     	containedRooms.push_back(*room);		   
				     
		     	}

		     	ExamLocation * roomGroup = new RoomGroup(containedRooms);
		     	roomGroups.push_back(roomGroup);
		     	liness.clear();
	     	}
	     	
	    }
		roomGroupFile.close();

		return roomGroups;
	}
	else
	{
		std::cerr << "could not open room groups file." << std::endl;
		std::list<ExamLocation *> emptyList;
		return emptyList;
	}
}

// remove non-alphanumeric characters at the end of the roomNumber
std::string LocationReader::fixRoomNumber(std::string roomNumber)
{
 	char lastChar;
 	do
 	{
     	lastChar = roomNumber.at( roomNumber.length() - 1 );	
     	if (!isalnum(lastChar))
     	{
     		roomNumber = roomNumber.substr(0, roomNumber.size()-1);
     	}
    } while (!isalnum(lastChar)); 

    return roomNumber;
}

// return a room_id used to create a Room object from the building a room number
Room::ROOM_ID LocationReader::roomID(std::string building, std::string roomNumber)
{
	return building + "_" + roomNumber;
}