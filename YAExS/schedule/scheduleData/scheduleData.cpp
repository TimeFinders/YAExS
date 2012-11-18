/*
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
