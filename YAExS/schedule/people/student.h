/*
  ----- Student Class Declaration-----
  A class that represents a student.
*/

#ifndef _student_h_
#define _student_h_

#include <string>
#include <list>
#include "person.h"

class Student : public Person
{
	private:
		std::string identifier;
		std::list<Exam> exams;
	
	public:
		//Student() : identifier(""), exams(0) {};

		Student(std::string id) : identifier(id), exams(0) {};

		Student(std::string id, std::list<Exam> exams) 
			: identifier(id), exams(exams) {};	

		Person::PERSON_ID getId() const { return identifier; };

		std::list<Exam> getExams() const { return exams; };
                
                Person* clone() const {return new Student(identifier, exams);}
};
#endif
