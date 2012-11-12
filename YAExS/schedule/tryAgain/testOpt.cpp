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

    scipScheduler.loadModel(e, slots);
    
    //Get the best solution
    scipScheduler.schedule();
    
    scipScheduler.printSolutionAndValues();
	//solution = scipScheduler.getBestSolution();

    //Print the solution to cout
    //std::cout << solution << std::endl;
	

    return 0;
}
