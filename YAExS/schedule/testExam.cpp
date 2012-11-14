// test exam

#include "exam.h"
#include "timeslot.h"

int main()
{
	Exam e(52, "CLA");
	TimeSlot timeSlot(1);
	e.assignTime(timeSlot);
	e.print();


	return 0;
}
