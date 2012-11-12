// Test Timeslot

#include "timeslot.h"
#include <iostream>

int main()
{
	TimeSlot t(5);
	TimeSlot u(6);

	if (t < u)
		std::cout << "t before u" <<  std::endl;
	else
		std::cout << "t not before u" << std::endl;

	TimeSlot x(5);
	TimeSlot y(1);

	if (x < y)
		std::cout << "x before y" <<  std::endl;
	if (x > y)
		std::cout << "x after y" << std::endl;
	
	if (x == y)
		std::cout << "x == y" << std::endl;
	if (x != y)
		std::cout << "x!=y" << std::endl;

}
