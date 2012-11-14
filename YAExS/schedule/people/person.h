/*
  ----- Person Class Declaration-----
  An abstract class that represents a person
  Either an instructor or a student
*/

#ifndef _person_h_
#define _person_h_

#include <string>
#include <vector>
#include "../exams/exam.h"

class Person
{
	public:

		typedef std::string PERSON_ID;
		
		virtual PERSON_ID getId() const = 0;
		virtual std::vector<Exam> getExams() = 0;

		static bool personHasExam(Person * p, Exam::EXAM_ID examID)
		{
			if (p == NULL)
			{
				std::cerr << "person is null, returning false" << std::endl;
				return false;
			}

			std::vector<Exam> exams = p->getExams();
			for (std::vector<Exam>::iterator it = exams.begin(); it != exams.end(); it++)
			{
				if ( it->getId() == examID)
					return true;
			}
			return false;
		}
};
#endif
