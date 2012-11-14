/*
  -----Scheduler Class Declaration-----
  A class to act as a mediator betweeen an Optimizer and a DBReader.
*/

#ifndef _scheduler_h_
#define _scheduler_h_

#include "dbreader.h"
#include "optimizer.h"
#include "exams/exam.h"
#include "people/student.h"

class Scheduler
{
public:
        //Constructors
        Scheduler();
        Scheduler(DBReader* dbIn, Optimizer* optIn);

        //Destructor
        ~Scheduler();

        //Setup functions
        bool loadExams();
        bool loadStudents();
        bool loadInstructors();

        //Scheduling functions
        bool startScheduling();
        std::string status();
        bool stopScheduling();
        void printSchedule();

private:
        //Pointer to a DBReader
        DBReader* db_;

        //Pointer to an Optimizer
        Optimizer* optimizer_;

        //Current state of the scheduling process
        std::string state_;

        //A vector of exams on their way between the optimizer or database
        std::vector<Exam> exams_;

        //A vector of people pointers to use in the optimizer
        std::vector<Person*> people_;
};


#endif
