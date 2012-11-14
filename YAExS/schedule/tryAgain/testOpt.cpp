/*
  -----Optimizer Test Main-----
*/

#include "optimizer.h"
#include <iostream>
#include <vector>
#include "getsome.h"
#include <algorithm>

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
    
    std::vector<Exam> exams = GetSome::getSomeExams();
    
    int numDays = 2;
    int slotsPerDay = 2;


    //Create an Optimizer
    Optimizer scipScheduler(true);
        //Load a model
    scipScheduler.loadModel(exams, people, numDays, slotsPerDay);
    
    //Get the best solution
    scipScheduler.schedule();
    
    scipScheduler.printSolutionAndNonzeroValues();
	
    // print just the exam schedule
    std::cout << "\n_________________________\n" << std::endl;
    scipScheduler.printExamSchedule();
    std::cout << "\n_________________________\n" << std::endl;

    std::remove_if( people.begin(), people.end(), deleteAll );
	

    return 0;
}
