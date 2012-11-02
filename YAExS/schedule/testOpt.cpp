/*
  -----Optimizer Test Main-----
*/

#include "optimizer.h"
#include <iostream>

int main(int argc, char* argv[])
{
    //Takes in one argument, the ZIMPL model
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " ZIMPL-filename" << std::endl;
        return 1;
    }

    //Create an Optimizer
    Optimizer scipScheduler;

    std::string solution;
    //Load the ZIMPL model
    scipScheduler.loadZIMPLModel(argv[1]);
    
    //Get the best solution
    scipScheduler.schedule();
    solution = scipScheduler.getBestSolution();

    //Print the solution to cout
    std::cout << solution << std::endl;
}
