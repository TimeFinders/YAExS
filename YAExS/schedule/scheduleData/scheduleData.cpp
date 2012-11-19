/*
  Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
  All rights reserved.
  
  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
  Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  
  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  
  The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
  
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
  -----Schedule Data Class Implementation-----
  A class to store information about exams and people.
  Serves as a wrapper for the vectors.
*/

#include "scheduleData.h"

//Destructor
ScheduleData::~ScheduleData()
{
        clearPeople();
        clearExams();
}

//Empties the data of exams
void ScheduleData::clearExams()
{
        exams_.clear();
}

//Empties the data of people
void ScheduleData::clearPeople()
{
        for (size_t i = 0; i < people_.size(); i++)
        {
                delete people_[i];
        }
        people_.clear();
}

//Returns the number of exams
int ScheduleData::numExams() const
{
        return exams_.size();
}

//Returns the number of people
int ScheduleData::numPeople() const
{
        return people_.size();
}

//Adds an exam to the data
void ScheduleData::addExam(Exam exam)
{
        exams_.push_back(exam);
}

//Adds a person to the data
void ScheduleData::addPerson(Person* person)
{
        people_.push_back(person->clone());
}

//Returns true if the provided exam ID already exists
bool ScheduleData::containsExamID(Exam::EXAM_ID compare)
{
        for (std::list<Exam>::iterator it = exams_.begin(); it != exams_.end(); it++)
        {
                if (it->getId() == compare) return true;
        }
  
        return false;
}

 //Returns the exams for viewing
std::list<Exam>& ScheduleData::exams()
{
        return exams_;
}

//Returns the people for viewing
const std::vector<Person*>& ScheduleData::people()
{
        return people_;
}

//Updates the number of students in the given exam
void ScheduleData::updateNumStudents(std::string examID, int numStudents)
{
        for (std::list<Exam>::iterator i = exams_.begin(); i != exams_.end(); i++)
        {
                if (i->getId() == examID) i->assignNumStudents(numStudents);
        }
}
