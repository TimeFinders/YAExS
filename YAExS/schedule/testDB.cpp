/*
  -----PQXX Database Test-----
  Quick test main to ensure complation and linking work.
*/

#include <iostream>

#include "scheduler.h"

int main(int argc, char* argv[])
{
        //Create a DBManager and Optimizer
        DBManager dbManager("user=andrew password=andrew host=ec2-23-20-128-235.compute-1.amazonaws.com dbname=django port=5432");
        Optimizer opt(false);

        //Create a Scheduler
        Scheduler sched(&dbManager, &opt);

        //Try to load exams and students
        sched.loadExams();
        sched.loadStudents("StudentIDs2012CRNs.csv");
        sched.loadLocations("locations/Rooms.csv", "locations/groupedRooms.csv");

        //Run it!
        int examDays = 5;
        int slotsPerDay = 4;
        sched.startScheduling(examDays,  slotsPerDay );

        //Print output
        sched.printSchedule();

        std::cout << "now assigning the rooms and then printing them" << std::endl;
        sched.assignRooms();
        sched.printRooms();

        return 0;
}
