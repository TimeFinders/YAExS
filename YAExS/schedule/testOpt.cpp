/*
  Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
  All rights reserved.
  
  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
  Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  
  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  
  The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
  
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
  -----Optimizer Test Main-----
*/

#include "optimizer.h"
#include <iostream>
#include <vector>
#include <list>
#include "test/getsome.h"
#include <algorithm>
#include "locations/locationreader.h"
#include "locations/examlocation.h"
#include "locations/locationassigner.h"

static bool deleteAll( Person * p ) { delete p; return true; }

int main(int argc, char* argv[])
{


    // Set up a test example

    std::vector<Person* > people = GetSome::getSomePeople();
    
    for (int i = 0; i < people.size(); i++)
    {
        std::cout << people[i]->getId() << "'s exams:" <<std::endl;
        std::list<Exam> e = people[i]->getExams();
        for(std::list<Exam>::iterator it = e.begin(); it!=e.end(); it++)
        {
            std::cout << '\t' << it->getId() << std::endl;
        }
    }

    

    std::list<Exam> someexams = GetSome::getSomeExams();
    std::list<Exam> otherExams = GetSome::getOtherExams();
    std::list<Exam> exams;
    exams.insert( exams.end(), someexams.begin(), someexams.end() );
    exams.insert( exams.end(), otherExams.begin(), otherExams.end() );

    for (std::list<Exam>::iterator it = exams.begin(); it!= exams.end(); it++)
    {
        Exam e = *it;
        TimeSlot t = e.getTime();
        std::cout << "exam " << e.getId() << " scheduled at " << t.getId() << std::endl;
    }




    int numDays = 2;
    int slotsPerDay = 2;


    //Create an Optimizer
    Optimizer scipScheduler;
        //Load a model
    scipScheduler.loadModel(exams, people, numDays, slotsPerDay);

    for (std::list<Exam>::iterator it = exams.begin(); it!= exams.end(); it++)
    {
        Exam e = *it;
        TimeSlot t = e.getTime();
        std::cout << "exam " << e.getId() << " scheduled at " << t.getId() << std::endl;
    }
    
    //Get the best solution
    scipScheduler.schedule();
    scipScheduler.printSolutionAndNonzeroValues();
	
    // print just the exam schedule
    std::cout << "\n_________________________\n" << std::endl;
    scipScheduler.printExamSchedule();
    std::cout << "\n_________________________\n" << std::endl;


    // ROOM SCHEDULING
    LocationReader lr;

    std::list<ExamLocation*> locs = lr.readLocations("locations/Rooms.csv", "locations/groupedRooms.csv");
    std::cout << "all exam locations are:" << std::endl;
    for (std::list<ExamLocation*>::iterator it = locs.begin(); it!=locs.end(); it++)
    {   
        ExamLocation::LOCATION_ID id = (*it)->getId();
        std::cout << id << std::endl;
    }

    

    int status = LocationAssigner::assignLocations(exams, locs);
    std::cout << " location assigner exited with status " << status << std::endl;

    std::cout << " ENTIRE EXAM SCHEDULE " << std::endl;
    std::cout << "\n_________________________\n" << std::endl;
    for (std::list<Exam>::iterator it = exams.begin(); it!= exams.end(); it++)
    {
        Exam e = *it;
        TimeSlot t = e.getTime();
        std::cout << "exam " << e.getId() << " scheduled at " << t.getId();
        
        ExamLocation * l = e.getLocation();
        if (l != NULL)
        {
            std::cout << " in: ";
            l->print();
        }
        else
        {
            std::cout << " no location assigned";
        }
        std::cout << std::endl;
    }
    std::cout << "\n_________________________\n" << std::endl;



    std::remove_if( people.begin(), people.end(), deleteAll );

    for (std::list<Exam>::iterator it = exams.begin(); it!= exams.end(); it++)
    {
        Exam e = *it;
        TimeSlot t = e.getTime();
        std::cout << "exam " << e.getId() << " scheduled at " << t.getId() << std::endl;
    }
	



    return 0;
}
