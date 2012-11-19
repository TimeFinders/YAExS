/*
  Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
  All rights reserved.
  
  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
  Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  
  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  
  The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
  
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
  ----- Person Class Declaration-----
  An abstract class that represents a person
  Either an instructor or a student
*/

#ifndef _person_h_
#define _person_h_

#include <string>
#include <list>
#include <iostream>
#include "../exams/exam.h"

class Person
{
	public:

		typedef std::string PERSON_ID;
		
		virtual PERSON_ID getId() const = 0;
		virtual std::list<Exam> getExams() const = 0;
                virtual Person* clone() const = 0;
                virtual ~Person() {};

		static bool personHasExam(Person * p, Exam::EXAM_ID examID)
		{
			if (p == NULL)
			{
				std::cerr << "person is null, returning false" << std::endl;
				return false;
			}

			std::list<Exam> exams = p->getExams();
			for (std::list<Exam>::iterator it = exams.begin(); it != exams.end(); it++)
			{
				if ( it->getId() == examID)
					return true;
			}
			return false;
		}
};
#endif
