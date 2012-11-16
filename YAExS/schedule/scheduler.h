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
#include "scheduleData/scheduleData.h"

class Scheduler
{
public:
        //Constructors
        Scheduler();
        Scheduler(DBReader* dbIn, Optimizer* optIn);

        //Setup functions
        bool loadExams();
        bool loadStudents(std::string filename);
        bool loadInstructors();
        bool loadRooms();

        //Scheduling functions
        bool startScheduling();
        std::string status();
        bool stopScheduling();
        void printSchedule();

        typedef std::map<std::string, std::vector<std::string> > Registrations;
private:
        //Helpers for loadStudents()
        void parseLine(Registrations & reg, const std::string & line);
        int studentsInClass(const Registrations & reg, const std::string & crn);
        std::vector<Exam> convertToExam(const Registrations & reg, const std::vector<std::string> & input);
        
        //Pointer to a DBReader
        DBReader* db_;

        //Pointer to an Optimizer
        Optimizer* optimizer_;

        //Current state of the scheduling process
        std::string state_;

        //Storage for any data being transferred around
        ScheduleData data_;
};


#endif
