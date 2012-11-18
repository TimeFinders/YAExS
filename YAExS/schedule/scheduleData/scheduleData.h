/*
  -----Schedule Data Class Declaration-----
  A class to store information about exams and people.
  Serves as a wrapper for the vectors.
*/

#ifndef _scheduledata_h_
#define _scheduledata_h_

#include <vector>
#include <list>
#include "../people/person.h"

class ScheduleData
{
public:
        //Destructor
        ~ScheduleData();
        
        //Standard usage functions
        void clearExams();
        void clearPeople();
        int numExams() const;
        int numPeople() const;
        void addExam(Exam exam);
        void addPerson(Person* person);
        bool containsExamID(Exam::EXAM_ID compare);
        //Person::PERSON_ID examID(int index);
        std::list<Exam>& exams();
        const std::vector<Person*>& people();
  
private:
        std::list<Exam> exams_;
        std::vector<Person*> people_;
};

#endif
