/*
  -----Optimizer Class Implementation-----
  A class to manage SCIP's variables, constraints, and solving for exam scheduling.
*/

#include "optimizer.h"

//Default constructor
Optimizer::Optimizer()
{
    //Initialize SCIP
    SCIPcreate(&scip_);

    //Load default plugins
    SCIPincludeDefaultPlugins(scip_);

    //Create a problem
    SCIPcreateProb(scip_, "Exam Scheduling",
                   NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

//Destructor
Optimizer::~Optimizer()
{
    //Free the SCIP structure
    SCIPfree(&scip_);
}

//Loads a ZIMPL model into SCIP
void Optimizer::loadZIMPLModel(std::string filename)
{
    //Read it in
    SCIPreadProb(scip_, filename.c_str(), "zpl");
}

//Run the solver
void Optimizer::schedule()
{
    SCIPsolve(scip_);
}

//Return the best solution as a string
std::string Optimizer::getBestSolution()
{
    //Create a stringstream to redirect cout to a string
    std::stringstream redir;

    //Store the old streambuf while linking cout to the stringstream
    std::streambuf* old = std::cout.rdbuf(redir.rdbuf());

    //Print the solution to the stringstream
    SCIPprintBestSol(scip_, NULL, false);

    //Restore the old streambuf
    std::cout.rdbuf(old);

    //Return the string stored in redir
    return redir.str();
}
