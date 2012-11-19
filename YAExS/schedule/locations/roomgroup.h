/*
  Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
  All rights reserved.
  
  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
  Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  
  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  
  The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
  
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
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
