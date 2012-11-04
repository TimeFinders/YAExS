/*
  ----- Room Class Declaration-----
  A class that represents a room on 
  campus that can hold exams. This is considered
  the most basic form of ExamLocation. Other
  ExamLocations can be groupings of rooms.
*/

#ifndef _room_h_
#define _room_h_

#include <string>
#include "exam.h"
#include "examlocation.h"

// for debugging
#include <iostream>

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

	protected:
		int getCapacity() { return capacity; };

	public:
		//bool isOverlapping(ExamLocation * e) {return false;};

		Room() : building(""), roomAddress(""), capacity(0) {};

		// e.g. new Room("AmosEaton", "214", 162) 
		Room(std::string building, std::string roomAddress, int totalSeats) : 
			building(building), 
			roomAddress(roomAddress), 
			capacity(totalSeats / 2) {};

		// Return true if the given exam can fit in this room, false otherwise.
		bool willExamFit(Exam exam) { return this->capacity >= exam.size(); };

		std::vector<Room> contains() { return std::vector<Room>(1,*this); }; 

		// for debugging
		void print() { std::cout << building << " " << roomAddress;
			std::cout << " " << " capacity: " << capacity << std::endl; };	

		bool operator== (const Room & r)
		{	
			return (r.building == this-> building && r.roomAddress == this->roomAddress);
		}

		bool static overlaps(ExamLocation * e1, ExamLocation * e2) 
		{
			std::vector<Room> rooms1 = e1->contains();
			std::vector<Room> rooms2 = e2->contains();
			for(unsigned i = 0; i < rooms1.size(); i++)
			{
				for (unsigned j = 0; j < rooms2.size(); j++)
				{
					if (rooms1[i] == rooms2[j])
						return true;
				}
			}
			return false;
		};

};
#endif
