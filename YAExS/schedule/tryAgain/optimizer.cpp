/*
  -----Optimizer Class Implementation-----
  A class to manage SCIP's variables, constraints, and solving for exam scheduling.
*/

#include "optimizer.h"
#include "timeslot.h"

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
    // Close things up
		
		
	//std::cout << " releasing extra variable" << std::endl;
	SCIPreleaseVar(scip_, & extraVar);
	SCIPreleaseVar(scip_, & extraVar2);
	
	//std::cout << "releasing extra constraint" << std::endl;
	SCIPreleaseCons(scip_, & extraCon);

	
	
	for (std::unordered_map<Exam::EXAM_ID, std::unordered_map<int, SCIP_VAR*> >::iterator examIt = examVars.begin();
		examIt != examVars.end(); examIt++)
	{
		std::unordered_map<int, SCIP_VAR *> theMap;
		theMap  = examIt->second;
		for (std::unordered_map<int, SCIP_VAR *>::iterator tsIt= theMap.begin();
			tsIt != theMap.end(); tsIt++)
		{	
			SCIPreleaseVar(scip_, & tsIt->second);
		}
	}
	
	
	//Then we close the SCIP environment:
    SCIPfree(&scip_);
}


const char* Optimizer::examAtVariableName(const Exam & exam, const TimeSlot & timeslot)
{
    // could change to read later: http://www.cplusplus.com/reference/iostream/istream/read/

   std::stringstream ss;
   ss << exam.getId() << "_" << timeslot.getId();
   std::string str(ss.str());
   const char* name = str.c_str();

   return name;
}


//Loads a ZIMPL model into SCIP
void Optimizer::loadModel(const Exam & e, const std::vector<TimeSlot> & slots)
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

	
	
	for (std::vector<TimeSlot>::const_iterator it = slots.begin(); it != slots.end(); it++)
	{
		const char * examName = examAtVariableName(e, *it);
	
		double objCoefExam = -50 + 5 * (it->getId());

		SCIP_VAR * examVar;
		SCIPcreateVar(scip_, & examVar, examName, 0.0, 1.0,
				objCoefExam, SCIP_VARTYPE_BINARY,
				isInitial, canRemoveInAging,
				NULL, NULL, NULL, NULL, NULL);
		SCIPaddVar(scip_, examVar);

		std::unordered_map<int,  SCIP_VAR *> aMap;
		aMap[it->getId()] = examVar;
		examVars[e.getId()] = aMap;
	}

	//SCIP_CONS * extraCon;
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


	for (std::unordered_map<Exam::EXAM_ID, std::unordered_map<int, SCIP_VAR*> >::iterator examIt = examVars.begin();
		examIt != examVars.end(); examIt++)
	{
		std::unordered_map<int, SCIP_VAR *> theMap;
		theMap  = examIt->second;
		for (std::unordered_map<int, SCIP_VAR *>::iterator tsIt= theMap.begin();
			tsIt != theMap.end(); tsIt++)
		{	
			SCIPaddCoefLinear(scip_, extraCon, tsIt->second, 1.0);
		}
	}
	

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

		for (std::unordered_map<Exam::EXAM_ID, std::unordered_map<int, SCIP_VAR*> >::iterator examIt = examVars.begin();
			examIt != examVars.end(); examIt++)
		{
			std::unordered_map<int, SCIP_VAR *> theMap;
			theMap  = examIt->second;
			for (std::unordered_map<int, SCIP_VAR *>::iterator tsIt= theMap.begin();
				tsIt != theMap.end(); tsIt++)
			{	
				std::cout << "\t" << "exam " << examIt->first;
				std::cout << " time " << tsIt->first;
				std::cout << " Var : " << SCIPgetSolVal(scip_, sol, tsIt->second) << std::endl; 
			}
		}
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
