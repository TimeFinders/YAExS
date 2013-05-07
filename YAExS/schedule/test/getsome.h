/*
  Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
  All rights reserved.
  
  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
  Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  
  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  
  The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
  
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
  -----Just a helper class for testing SCIP-----
*/

#include "../exams/exam.h"
#include "../people/student.h"
#include <vector>
#include <list>
#include "../people/person.h"
#include "../people/instructor.h"

class GetSome
{

public:

	// EXAMS
	static std::list<Exam> getFourExams()
	{
		std::list<Exam> exams;

		Exam cla(25, "CLA");
		cla.assignTime(1);
		exams.push_back(cla);

		Exam sdd(80, "SDD");
		sdd.assignTime(2);
		exams.push_back(sdd);

		Exam bio(226, "BIO");
		bio.assignTime(1);
		exams.push_back(bio);

		Exam ccn(15, "CCN");
		ccn.assignTime(1);
		exams.push_back(ccn);

		return exams;
	}

	static std::list<Exam> getSomeExams()
	{
		return getFourExams();
	}

	static std::list<Exam> getOtherExams()
	{
		std::list<Exam> exams;

		Exam diff(198, "DIFFEQ");
		diff.assignTime(2);
		exams.push_back(diff);

		Exam algo(45, "algo");
		algo.assignTime(2);
		exams.push_back(algo);

		return exams;
	}

	static std::vector<TimeSlot> getSomeTimeSlots()
	{
		std::vector<TimeSlot> ts;
		ts.push_back(TimeSlot(1));
		ts.push_back(TimeSlot(2));
		return ts;
	}


	static std::vector<Person*> getSomePeople()
	{
   	 	std::vector<Person *> people;

	    	std::list<Exam> andrewExams = getSomeExams();
	    	andrewExams.erase(andrewExams.begin());
	    	Exam * ccn = new Exam(andrewExams.back());
	    	andrewExams.pop_back();
	    	andrewExams.pop_back();
	    	andrewExams.push_back(*ccn);

	    	Person * andrewPtr = new Student("Andrew", andrewExams);
	    	people.push_back(andrewPtr);

	    	std::list<Exam> austonExams = andrewExams;
	    	austonExams.pop_back();
	    Person * austonPtr = new Student("Auston", austonExams);
	    	people.push_back(austonPtr);

	    	std::list<Exam> johnExams(austonExams);
	    	Person * johnPtr = new Instructor("John", johnExams);
	    	people.push_back(johnPtr);

	    	
	    	std::list<Exam> jeffExams = getSomeExams();
	    	jeffExams.erase(jeffExams.begin());
	    	Person * jeffPtr = new Student("Jeff", jeffExams);
	    	people.push_back(jeffPtr);

	    	std::list<Exam> veraExams = getSomeExams();
	    	veraExams.pop_back();
	    	Person * veraPtr = new Student("Vera", veraExams);
	    	people.push_back(veraPtr);

    	std::list<Exam> zachExams = getOtherExams();
    	Person * zachPtr = new Student("Zach", zachExams);
    	people.push_back(zachPtr);

		return people;
	}
	

	static std::vector<Student> getSomeStudents()
	{
		std::vector<Student> students;
		Student s("Vera");
		students.push_back(s);
		return students;
	}
	//static std::vector<Instructor> getSomeInstructors();
};
