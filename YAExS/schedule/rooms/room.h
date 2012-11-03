/*
  ----- Room Class Declaration-----
  A class that represents a room on 
  campus that can hold exams.
*/

#ifndef _room_h_
#define _room_h_

#include <string>
#include "exam.h"
#include "examlocation.h"

class Room : public ExamLocation
{
	friend class RoomGroup;

	private:	
		// e.g AmosEaton
		std::string building;

		// e.g. 214
		 std::string roomAddress;

		// half the number of total seats, 
		// e.g. AmosEaton 214 has 162 seats so capacity is 81
		int capacity;

	public:
		Room() : building(""), roomAddress(""), capacity(0) {};

		// e.g. new Room("AmosEaton", "214", 162) 
		Room(std::string building, std::string roomAddress, int totalSeats) : 
			building(building), 
			roomAddress(roomAddress), 
			capacity(totalSeats / 2) {};

		// Return true if the given exam can fit in this room, false otherwise.
		bool willExamFit(Exam exam) { return this->capacity >= exam.size(); };
};
#endif
