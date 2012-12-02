/*
  Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
  All rights reserved.
  
  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
  Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  
  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  
  The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
  
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
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
       // Scheduler();
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

        // true if the rooms have been assigned successfully, else false
        bool roomsAreAssigned;
};


#endif
