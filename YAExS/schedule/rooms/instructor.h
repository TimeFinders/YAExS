/*
  ----- Instructor Class Declaration-----
  A class that represents a course instructor.
*/

#ifndef _instructor_h_
#define _instructor_h_

#include "exam.h"
#include <string>
#include <vector>
#include "person.h"

class Instructor : public Person
{
	private:
		std::string identifier;
		std::vector<Exam> exams;
	
	public:
		Instructor() :identifier(""), exams(0) {};

		Instructor(std::string id) : identifier(id), exams(0) {};
		
		Instructor(std::string id, std::vector<Exam> theExams) : 
			identifier(id), exams(theExams) {};

		std::string getId() const { return identifier; };
		std::vector<Exam> getExams() { return exams; };	
};
#endif
