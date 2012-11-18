/*
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
        std::vector<Exam> e = people[i]->getExams();
        for(int j = 0; j < e.size(); j++)
        {
            std::cout << '\t' << e[j].getId() << std::endl;
        }
    }

    

    std::vector<Exam> someexams = GetSome::getSomeExams();
    std::vector<Exam> otherExams = GetSome::getOtherExams();
    std::vector<Exam> exams;
    exams.insert( exams.end(), someexams.begin(), someexams.end() );
    exams.insert( exams.end(), otherExams.begin(), otherExams.end() );

    for (int i = 0; i < exams.size(); i++)
    {
        Exam e = exams[i];
        TimeSlot t = e.getTime();
        std::cout << "exam " << e.getId() << " scheduled at " << t.getId() << std::endl;
    }




    int numDays = 2;
    int slotsPerDay = 2;


    //Create an Optimizer
    Optimizer scipScheduler(true);
        //Load a model
    scipScheduler.loadModel(exams, people, numDays, slotsPerDay);

    for (int i = 0; i < exams.size(); i++)
    {
        Exam e = exams[i];
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
    std::cout << "first location is ";
    (locs.front())->print();

    std::list<Exam> examList( exams.begin(),  exams.end() );
    int status = LocationAssigner::assignLocations(examList, locs);
    std::cout << " location assigner exited with status " << status << std::endl;

    std::cout << " ENTIRE EXAM SCHEDULE " << std::endl;
    std::cout << "\n_________________________\n" << std::endl;
    for (std::list<Exam>::iterator it = examList.begin(); it!= examList.end(); it++)
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

    for (int i = 0; i < exams.size(); i++)
    {
        Exam e = exams[i];
        TimeSlot t = e.getTime();
        std::cout << "exam " << e.getId() << " scheduled at " << t.getId() << std::endl;
    }
	

    return 0;
}
