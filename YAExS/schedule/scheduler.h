/*
  -----Scheduler Class Declaration-----
  A class to act as a mediator betweeen an Optimizer and a DBManager.
*/

#ifndef _scheduler_h_
#define _scheduler_h_

#include "dbmanager.h"
#include "optimizer.h"
#include "exams/exam.h"
#include "people/student.h"
#include "scheduleData/scheduleData.h"
#include "locations/locationreader.h"
#include "locations/locationassigner.h"

class Scheduler
{
public:
        //Constructors
        Scheduler();
        Scheduler(DBManager* dbIn, Optimizer* optIn);

        bool loadExams();
        bool loadStudents(std::string filename);
        bool loadInstructors();
        bool loadLocations(std::string roomFilePath, std::string roomGroupFilePath);

        //Scheduling functions
        bool startScheduling(int numberOfExamDays, int numberOfSlotsPerDay);
        std::string status();
        bool stopScheduling();
        void printSchedule();

        bool assignRooms();
        void printRooms();

        typedef std::map<Person::PERSON_ID, std::vector<Exam::EXAM_ID> > Registrations;
        typedef std::string CRN;

private:
        //Helpers for loadStudents()
        void parseLine(Registrations & reg, const std::string & line, std::map<CRN, Exam::EXAM_ID> & match);
        int studentsInExam(const Registrations & reg, const Exam::EXAM_ID & examID);
        std::list<Exam> convertToExam(const Registrations & reg, const std::vector<Exam::EXAM_ID> & input);
        void updateNumStudents(const Registrations & reg);
        
        //Pointer to a DBManager
        DBManager* db_;

        //Pointer to an Optimizer
        Optimizer* optimizer_;

        //Storage for any data being transferred around
        ScheduleData data_;

        //exam locations to be used by the roomAssigner
        std::list<ExamLocation* > locations_;
};


#endif
