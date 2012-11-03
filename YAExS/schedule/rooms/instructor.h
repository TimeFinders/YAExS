/*
  ----- Instructor Class Declaration-----
  A class that represents a course instructor.
*/

#ifndef _instructor_h_
#define _instructor_h_

#include "exam.h"
#include <string>
#include <vector>

class Instructor
{
	private:
		std::string identifier;
		std::vector<Exam> exams;
	
	public:
		Instructor() :identifier(""), exams(0) {};

		Instructor(std::string id) : identifier(id), exams(0) {};
		
		Instructor(std::string id, std::vector<Exam> theExams) : 
			identifier(id), exams(theExams) {};	
};
#endif
