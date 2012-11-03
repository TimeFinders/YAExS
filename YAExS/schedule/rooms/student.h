/*
  ----- Student Class Declaration-----
  A class that represents a student.
*/

#ifndef _student_h_
#define _student_h_

#include <string>
#include <vector>

class Student
{
	private:
		std::string identifier;
		std::vector<Exam> exams;
	
	public:
		Student() : identifier(""), exams(0) {};

		Student(std::string id) : identifier(id), exams(0) {};

		Student(std::string id, std::vector<Exam> exams) 
			: identifier(id), exams(exams) {};	
};
#endif
