/*
  Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
  All rights reserved.
  
  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
  Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  
  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  
  The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
  
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
  ----- Time Slot Class Declaration-----
  A class that represents a time slot.
  Really just a wrapper for an in time slot id.
*/

#ifndef _timeslot_h_
#define _timeslot_h_


class TimeSlot
{
	private:
		int id; // e.g. 1..4 on day 1, 5...8 on day 2, etc.
		
		
	public:
		typedef int TIMESLOT_ID;

		TimeSlot() : id(-1) {};

		TimeSlot(int id) : id(id) {};

		// compare by smaller (earlier) id
		bool operator< (const TimeSlot & other){ return this->id < other.id; }; 

		// compare by id
		bool operator== (const TimeSlot & other){ return this->id == other.id; }; 

		// compare by id
		bool operator!= (const TimeSlot & other){ return this->id != other.id; }; 

		// compare by id
		bool operator> (const TimeSlot & other){ return this->id > other.id; }; 

		int getId() const { return id; };

		// for debugging
		int toPrint() const { return id; }
		
};
#endif
