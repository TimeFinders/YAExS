/*
  Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
  All rights reserved.
  
  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
  Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  
  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  
  The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
  
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
  ----- Location Reader Class Declaration-----
  A class to read in information about rooms and room 
  groups to create exam location objects.
*/

#include <list>
#include <string>
#include <fstream>
#include <unordered_map>

// this will include examlocation.h
#include "room.h"
#include "roomgroup.h"


class LocationReader
{
public:
	// read in both rooms and room groups, return a list of Rooms and RoomGroups
	std::list<ExamLocation*> readLocations(std::string roomFilePath, std::string roomGroupFilePath);

private:

	// the rooms from the room file. used to create room groups.
	std::unordered_map<std::string, Room*> rooms_;


	// read in rooms. you must read rooms before the room groups.
	std::list<ExamLocation*>  readRooms(std::string roomFilePath);

	// read in room groups. you must read rooms before the room groups.
	std::list<ExamLocation*>  readRoomGroups(std::string roomGroupFilePath);

	// remove non-alphanumeric characters at the end of a roomNumber read from a file;
	std::string fixRoomNumber( std::string roomNumber);

	// return a room_id used to create a Room object from the building a room number
	std::string roomID(std::string building, std::string roomNumber);

};
