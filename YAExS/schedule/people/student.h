/*
  Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
  All rights reserved.
  
  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
  Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  
  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  
  The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
  
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
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
