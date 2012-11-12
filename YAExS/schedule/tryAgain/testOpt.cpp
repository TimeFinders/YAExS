/*
  -----Optimizer Test Main-----
*/

#include "optimizer.h"
#include <iostream>

int main(int argc, char* argv[])
{

    //Create an Optimizer
    Optimizer scipScheduler;

    std::string solution;
    //Load a model
    Exam e(5, "CLA");

    scipScheduler.loadModel(e);
    
    //Get the best solution
    scipScheduler.schedule();
    
    scipScheduler.printSolutionAndValues();
	//solution = scipScheduler.getBestSolution();

    //Print the solution to cout
    //std::cout << solution << std::endl;
	

    return 0;
}
