/*
  ----- Day Class Declaration-----
  A class that represents a day of exams
*/

#ifndef _day_h_
#define _day_h_


class Day
{
	private:
		int identifier;
		std::vector<TimeSlot> slots;
	
	public:
		typedef int DAY_ID;

		Day(int id, std::vector<TimeSlot> timeslots) : identifier(id), slots(timeslots) {};

		DAY_ID getId() const{return identifier; };

		std::vector<TimeSlot> getSlots() const{return slots; };
};
#endif
