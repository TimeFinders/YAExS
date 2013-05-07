/*
  Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
  All rights reserved.
  
  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
  Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  
  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  
  The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
  
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
  ----- Exam Class Declaration-----
  A class that represents an exam.
*/

#ifndef _exam_h_
#define _exam_h_

class ExamLocation;

#include <string>
#include "../time/timeslot.h"


class Exam
{
	private:
		int numStudents;	
		ExamLocation * examLocation;
		TimeSlot timeSlot;
		std::string name;

	public:

		typedef std::string EXAM_ID;

		Exam(){ Exam(-1, ""); };

		Exam(int numberOfStudents, std::string name) :
			numStudents(numberOfStudents),
			examLocation(NULL),
			timeSlot(-1),
			name(name) {};

		// the number of students taking this exam
		int size();

		void assignTime(TimeSlot timeSlot);
		TimeSlot getTime(){return timeSlot;};

		void assignLocation(ExamLocation * examLocation);
		ExamLocation * getLocation() { return examLocation; };

                void assignNumStudents(int num);

		bool hasLocation();

		// this should return a UNIQUE id for this exam
		EXAM_ID getId() const { return name; };

		static bool isEarlier( Exam e1, Exam e2);
		static bool isLarger( Exam e1, Exam e2);

		// for sorting by timeslot, then within time slot by non-decreasing size
		static bool isLargerByTime( Exam e1, Exam e2);

		// for debugging
		void print();
};
#endif