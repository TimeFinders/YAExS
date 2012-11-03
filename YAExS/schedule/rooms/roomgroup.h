/*
  ----- Room Group Class Declaration-----
  A class that represents a group of rooms on 
  campus that can hold a single exam together.
*/

#ifndef _roomgroup_h_
#define _roomgroup_h_

#include <string>
#include <vector>
#include "exam.h"
#include "room.h"

class RoomGroup : public ExamLocation
{
	private:	
		// e.g [AmosEaton-214, AmosEaton-215, AmosEaton-216]
		std::vector<Room> rooms;

		// half the number of total seats in all constituent rooms
		int capacity;

	public:
		RoomGroup() : rooms(), capacity(0) {};

		RoomGroup(std::vector<Room> constituentRooms) : rooms(constituentRooms)
		{
			int totalSize = 0;			
			for (unsigned i = 0; i < rooms.size(); i++)
			{
				totalSize += rooms[i].capacity;
			}
			capacity = totalSize;		
		 };

		// Return true if the given exam can fit in this room group, false otherwise.
		bool willExamFit(Exam exam) { return this->capacity >= exam.size(); };
};
#endif
