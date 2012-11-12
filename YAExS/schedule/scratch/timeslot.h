/*
  ----- Time Slot Class Declaration-----
  A class that represents a time slot.
  This class is just a placeholder that will
  be improved/expanded later.
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
		int toPrint() const{ return id; }
		
};
#endif
