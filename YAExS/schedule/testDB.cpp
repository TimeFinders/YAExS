/*
  Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
  All rights reserved.
  
  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
  Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  
  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  
  The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
  
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
  -----PQXX Database Test-----
  Quick test main to ensure complation and linking work.
*/

#include <iostream>

#include "scheduler.h"

int main(int argc, char* argv[])
{
        //Create a DBManager and Optimizer
        DBManager dbManager("user=andrew password=andrew host=ec2-23-20-128-235.compute-1.amazonaws.com dbname=django port=5432");
        Optimizer opt;

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

        //std::cout << "writing the schedule to the database" << std::endl;
        //sched.writeScheduleToDB();

        //std::cout << "clearing the schedule from the database" << std::endl;
        //sched.clearDBSchedule();

        return 0;
}
