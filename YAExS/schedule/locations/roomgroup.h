/*
  ----- Room Group Class Declaration-----
  A class that represents a group of rooms on 
  campus that can hold a single exam together.
*/

#ifndef _roomgroup_h_
#define _roomgroup_h_

#include <string>
#include <vector>
#include "../exams/exam.h"
#include "room.h"

// for debugging
#include <iostream>

class RoomGroup : public ExamLocation
{
	private:	
		// e.g [AmosEaton-214, AmosEaton-215, AmosEaton-216]
		std::vector<Room> rooms;

		// half the number of total seats in all constituent rooms
		int capacity;

		std::string id;

	protected:
		int getCapacity() const { return capacity; };

	public:
		RoomGroup() : rooms(), capacity(0), id("noName") {};

		RoomGroup(std::vector<Room> constituentRooms) : rooms(constituentRooms), id("noName")
		{
			int totalSize = 0;
			id = "";
			for (unsigned i = 0; i < rooms.size(); i++)
			{
				totalSize += rooms[i].capacity;
				if (i > 0)
					id += "_";
				id += rooms[i].getId();
			}
			capacity = totalSize;		
		 };	

		std::vector<Room> contains() const {return rooms;};	

		// Return true if the given exam can fit in this room group, false otherwise.
		bool willExamFit(Exam exam) const { return this->capacity >= exam.size(); };

		std::string getId() const { return id; };

		// for debugging
		void print() const
		{ std::cout << "RoomGroup with Total capacity: " << capacity << std::endl; };	
};
#endif
