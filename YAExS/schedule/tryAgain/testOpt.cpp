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

    //Create an Optimizer
    Optimizer scipScheduler;

    std::string solution;
    //Load a model
    Exam e(5, "CLA");

    std::vector<Person* > people = GetSome::getSomePeople();

    std::vector<Exam> exams = GetSome::getSomeExams();
    
    int numDays = 1;
    int slotsPerDay = 2;
    scipScheduler.loadModel(exams, people, numDays, slotsPerDay);
    
    //Get the best solution
    scipScheduler.schedule();
    
    scipScheduler.printSolutionAndNonzeroValues();
	//solution = scipScheduler.getBestSolution();

    //Print the solution to cout
    //std::cout << solution << std::endl;


    std::remove_if( people.begin(), people.end(), deleteAll );
	

    return 0;
}
