/*
  -----Optimizer Test Main-----
*/

#include "optimizer.h"
#include <iostream>
#include <vector>
#include "getsome.h"

int main(int argc, char* argv[])
{

    //Create an Optimizer
    Optimizer scipScheduler;

    std::string solution;
    //Load a model
    Exam e(5, "CLA");

    std::vector<TimeSlot> slots = GetSome::getSomeTimeSlots();

    std::cout << " all the time slots are:" << std::endl;
    for (std::vector<TimeSlot>::iterator it = slots.begin(); it!=slots.end(); it++)
    {
        std::cout << it->getId() << "\t";
    }
    std::cout << "\n\n";

    std::vector<Person* > people = GetSome::getSomePeople();


    std::vector<Exam> exams = GetSome::getSomeExams();
    
    
    scipScheduler.loadModel(exams, slots, people);
    
    //Get the best solution
    scipScheduler.schedule();
    
    scipScheduler.printSolutionAndNonzeroValues();
	//solution = scipScheduler.getBestSolution();

    //Print the solution to cout
    //std::cout << solution << std::endl;
	

    return 0;
}
