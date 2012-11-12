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
void Optimizer::loadModel(const Exam & e)
{

	// JUST FOR CHECKING
	//SCIP_VAR * extraVar;
	std::string ExtravString = "extra";
	const char * ExtravName = ExtravString.c_str();
	bool isInitial = TRUE;
	bool canRemoveInAging = FALSE;
	double coefInObjective = -10.0;
	SCIPcreateVar(scip_, & extraVar, ExtravName, 0.0, 1.0, 
				coefInObjective, SCIP_VARTYPE_BINARY, 
				isInitial, canRemoveInAging,
				NULL, NULL, NULL, NULL, NULL);
	// dont forget to add variable to scip object or you will get segfaults later!
	SCIPaddVar(scip_, extraVar);
	
	//SCIP_VAR * extraVar2;
	std::string ExtravString2 = "extra2";
	const char * ExtravName2 = ExtravString2.c_str();
	double coefInObjective2 = -20;
	SCIPcreateVar(scip_, & extraVar2, ExtravName2, 0.0, 1.0,
				coefInObjective2, SCIP_VARTYPE_BINARY, 
				isInitial, canRemoveInAging,
				NULL, NULL, NULL, NULL, NULL);
	SCIPaddVar(scip_, extraVar2);

	std::string examString = e.getId();
	const char * examName = examString.c_str();
	double objCoefExam = -50;
	SCIPcreateVar(scip_, & examVar, examName, 0.0, 1.0,
				objCoefExam, SCIP_VARTYPE_BINARY,
				isInitial, canRemoveInAging,
				NULL, NULL, NULL, NULL, NULL);
	SCIPaddVar(scip_, examVar);

	SCIP_CONS * extraCon;
	double lbound = 1.0;
	double ubound = 1.0;
	std::string extraConName = "extraCon";
	const char * extraConNameChar = extraConName.c_str();
	
	SCIPcreateConsLinear(scip_, & extraCon, extraConNameChar,
			0, NULL, NULL, lbound, ubound, isInitial,
			TRUE, TRUE, TRUE, TRUE, FALSE,
			FALSE, FALSE, FALSE, FALSE);
	SCIPaddCoefLinear(scip_, extraCon, extraVar, 1.0);
	SCIPaddCoefLinear(scip_, extraCon, extraVar2, 1.0);
	SCIPaddCoefLinear(scip_, extraCon, examVar, 1.0);

	// add the constraint to the problem: (not in queens documentation but I think its necessary)
	 SCIPaddCons(scip_, extraCon);



}

void Optimizer::printSolutionAndValues()
{

	SCIP_SOL* sol = SCIPgetBestSol(scip_);

	if (sol == NULL)
	{
		std::cout << "no solution!" << std::endl;
	}
	else 
	{
		std::cout << "\nSolution found!" << std::endl;
		
		std::cout << "\t" << "extra variable : " << SCIPgetSolVal(scip_, sol, extraVar) << std::endl; 
		std::cout << "\t" << "extra variable2 : " << SCIPgetSolVal(scip_, sol, extraVar2) << std::endl; 
		std::cout << "\t" << "exam Var : " << SCIPgetSolVal(scip_, sol, examVar) << std::endl; 
	}
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
