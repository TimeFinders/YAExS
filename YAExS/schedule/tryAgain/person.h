/*
  ----- Person Class Declaration-----
  An abstract class that represents a person
  Either an instructor or a student
*/

#ifndef _person_h_
#define _person_h_

#include <string>
#include <vector>

class Person
{
	public:

		typedef std::string PERSON_ID;

		PERSON_ID getId() {return "GenericPerson";};
		virtual PERSON_ID getId() const = 0;
		virtual std::vector<Exam> getExams() = 0;
};
#endif
