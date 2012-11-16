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
bool ScheduleData::containsExamID(Person::PERSON_ID compare)
{
  for (size_t i = 0; i < exams_.size(); i++)
    {
      if (exams_[i].getId() == compare) return true;
    }
  
  return false;
}

//Returns the examID at the given index
Person::PERSON_ID ScheduleData::examID(int index)
{
  return exams_[index].getId();
}

//Returns the exams for viewing
const std::vector<Exam>& ScheduleData::exams()
{
  return exams_;
}

//Returns the people for viewing
const std::vector<Person*>& ScheduleData::people()
{
  return people_;
}
