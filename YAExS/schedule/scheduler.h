/*
  -----Scheduler Class Declaration-----
  A class to act as a mediator betweeen an Optimizer, a DBManager, 
  a LocationReader and a LocationAssigner
  Scheduler coordinates all scheduling activities. 

  Example Usage:
	Scheduler sched(&dbManager, &opt);

        //Load Information
        sched.loadExams();
        sched.loadStudents("StudentIDs2012CRNs.csv");
        sched.loadLocations("locations/Rooms.csv", "locations/groupedRooms.csv");

        //Schedule exams to times
        int examDays = 5;
        int slotsPerDay = 4;
        sched.startScheduling(examDays,  slotsPerDay );

	//Assign rooms to scheduled exams
        sched.assignRooms();

	//Print the schedule
        sched.printRooms();

        // Write the schedule to the database
	sched.writeScheduleToDB();

        Clearing the schedule from the database
        sched.clearDBSchedule();	
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

#ifndef DEBUG_PRINT
#define DEBUG_PRINT(x)
#ifdef debugMode
#undef DEBUG_PRINT
#define DEBUG_PRINT(x) std::cout << x << std::endl;
#endif
#endif

class Scheduler
{
public:
        typedef std::map<Person::PERSON_ID, std::vector<Exam::EXAM_ID> > Registrations;
        typedef std::string CRN;

        // Constructors
        Scheduler();
        Scheduler(DBManager* dbIn, Optimizer* optIn);

	// Load information functions
        bool loadExams();
        bool loadStudents(std::string filename);
        bool loadInstructors();
        bool loadLocations(std::string roomFilePath, std::string roomGroupFilePath);

        // Scheduling functions
        bool startScheduling(int numberOfExamDays, int numberOfSlotsPerDay);
        std::string status();
        bool stopScheduling();
        void printSchedule();

	// Room Assignment Functions
        bool assignRooms();
        void printRooms();

	// Database interaction functions
        void writeScheduleToDB();
        void clearDBSchedule();
        void deactivateDB();
        void reactivateDB();

private:
        //Helpers for loadStudents() and loadExams()
        void parseLine(Registrations & reg, const std::string & line, std::map<CRN, Exam::EXAM_ID> & match);
        int studentsInExam(const Registrations & reg, const Exam::EXAM_ID & examID);
        std::list<Exam> convertToExam(const Registrations & reg, const std::vector<Exam::EXAM_ID> & input);
        void updateNumStudents(const Registrations & reg);

        // write an individual exam to the database (called by writeScheduleToDB)
        void writeExamToDB( Exam & exam );

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
