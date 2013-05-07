/*
  Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
  All rights reserved.
  
  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
  Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  
  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  
  The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
  
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
  ----- Exam Class Implementation-----
  A class that represents an exam
*/

#include "exam.h"
#include "../time/timeslot.h"

//for debugging
#include <iostream>

// the number of students taking this exam
int Exam::size()
{
	return numStudents;
}

void Exam::assignTime(TimeSlot timeSlot)
{
	this->timeSlot = timeSlot;
}


void Exam::assignLocation(ExamLocation * location) 
{
	examLocation = location;
}

void Exam::assignNumStudents(int num)
{
        numStudents = num;
}

bool Exam::hasLocation()
{
	return (examLocation != NULL);
	//	return false;
	//if ((this->examLocation)->getID() == unassignedID)
	//	return false;

	//return true;
}

bool Exam::isEarlier(Exam e1, Exam e2)
{
	return e1.timeSlot < e2.timeSlot ;
}

bool Exam::isLarger(Exam e1, Exam e2)
{
	return ( e1.numStudents > e2.numStudents );
}

bool Exam::isLargerByTime(Exam e1, Exam e2)
{
	if (e1.timeSlot < e2.timeSlot)
		return true;
	if (e1.timeSlot == e2.timeSlot)
		return ( e1.numStudents > e2.numStudents );
		
	return false;
}

//for debugging
void Exam::print()
{
	std::cout << name << " students: " << numStudents << " time: " << timeSlot.toPrint() << std::endl;
}


