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

		virtual PERSON_ID getId() = 0;
		virtual std::vector<Exam> getExams() = 0;
};
#endif
