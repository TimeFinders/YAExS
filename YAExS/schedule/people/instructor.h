/*
  ----- Instructor Class Declaration-----
  A class that represents a course instructor.
*/

#ifndef _instructor_h_
#define _instructor_h_

#include "../exams/exam.h"
#include <string>
#include <list>
#include "person.h"

class Instructor : public Person
{
	private:
		std::string identifier;
		std::list<Exam> exams;
	
	public:
		Instructor() :identifier(""), exams(0) {};

		Instructor(std::string id) : identifier(id), exams(0) {};
		
		Instructor(std::string id, std::list<Exam> theExams) : 
			identifier(id), exams(theExams) {};

		std::string getId() const { return identifier; };
		std::list<Exam> getExams() const { return exams; };
                Person* clone() const {return new Instructor(identifier, exams);}
};
#endif
