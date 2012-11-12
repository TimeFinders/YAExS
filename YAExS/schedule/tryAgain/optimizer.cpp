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
     // Close things up
		
	for (std::unordered_map<Exam::EXAM_ID, std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR*> >::iterator examIt = examIsAt.begin();
		examIt != examIsAt.end(); examIt++)
	{
		std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR *> theMap;
		theMap  = examIt->second;
		for (std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR *>::iterator tsIt= theMap.begin();
			tsIt != theMap.end(); tsIt++)
		{	
			SCIPreleaseVar(scip_, & tsIt->second);
		}
	}

	//std::cout << "releasing extra constraint" << std::endl;
	SCIPreleaseCons(scip_, & extraCon);

	
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


//Loads a model into SCIP
void Optimizer::loadModel(const std::vector<Exam> & exams, const std::vector<TimeSlot> & slots)
{
	// load the exam is at variables
	loadExamIsAtVariables(exams, slots);

	// load the once constraints (each exam meets at exactly one time slot)
	// doesn't need parameters because just uses the examIsAtVariables
	loadOnceConstraints();

}


void Optimizer::loadExamIsAtVariables(const std::vector<Exam> & exams, const std::vector<TimeSlot> & slots)
{
	// check that the variables haven't been loaded already;
	if ( !examIsAt.empty() )
		std::cerr << " exam is at variable map is NOT empty. Only call loadExamIsAtVariables once." << std::endl;

	else 
	{
		bool isInitial = true;
		bool canRemoveInAging = false;

		for (std::vector<Exam>::const_iterator examIt = exams.begin(); 
			examIt != exams.end(); examIt++)
		{	
			std::unordered_map<TimeSlot::TIMESLOT_ID,  SCIP_VAR *> aMap;
			for (std::vector<TimeSlot>::const_iterator tsIt = slots.begin(); tsIt != slots.end(); tsIt++)
			{
				//std::cout << "adding an exam variable for time slot "  << tsIt->getId() << std::endl;
				const char * examName = examAtVariableName(*examIt, *tsIt);
			
				double objCoefExam = -50 + 5 * (tsIt->getId());

				SCIP_VAR * examVar;
				SCIPcreateVar(scip_, & examVar, examName, 0.0, 1.0,
						objCoefExam, SCIP_VARTYPE_BINARY,
						isInitial, canRemoveInAging,
						NULL, NULL, NULL, NULL, NULL);
				SCIPaddVar(scip_, examVar);

				aMap[tsIt->getId()] = examVar;

			} // end time slot for loop
			examIsAt[examIt->getId()] = aMap;
		} // end exam for loop

	} // end else
}

// load once constraints, which require that each exam meets at exactly one time slot
// doesn't need parameters because just uses examIsAt
void Optimizer::loadOnceConstraints()
{

	// check that the variables haven't been loaded already;
	if ( extraCon != NULL )
		std::cerr << " extra constraint is not null. only call loadOnceConstraints once" << std::endl;
	else if ( examIsAt.empty() )
	{
		std::cerr << " examIsAt variable map is null or empty. need to call loadExamIsAtVariables beforeLoadOnceConstraints." << std::endl;
	}
	else 
	{
		bool isInitial = true;
		double lbound = 1.0;
		double ubound = 1.0;
		std::string extraConName = "extraCon";
		const char * extraConNameChar = extraConName.c_str();
		
		SCIPcreateConsLinear(scip_, & extraCon, extraConNameChar,
				0, NULL, NULL, lbound, ubound, isInitial,
				TRUE, TRUE, TRUE, TRUE, FALSE,
				FALSE, FALSE, FALSE, FALSE);

		for (std::unordered_map<Exam::EXAM_ID, std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR*> >::iterator examIt = examIsAt.begin();
			examIt != examIsAt.end(); examIt++)
		{
			std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR *> theMap;
			theMap  = examIt->second;
			//std::cout << "size of the map for exam: " << theMap.size() << std::endl;
			for (std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR *>::iterator tsIt= theMap.begin();
				tsIt != theMap.end(); tsIt++)
			{	
			//	std::cout << " adding variable to constraint for exam " << examIt->first;
			//	std::cout << " and time " << tsIt ->first;
				SCIPaddCoefLinear(scip_, extraCon, tsIt->second, 1.0);
			}
		}
		

		// add the constraint to the problem: (not in queens documentation but I think its necessary)
		 SCIPaddCons(scip_, extraCon);
	}
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
		
		for (std::unordered_map<Exam::EXAM_ID, std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR*> >::iterator examIt = examIsAt.begin();
			examIt != examIsAt.end(); examIt++)
		{
			std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR *> theMap;
			theMap  = examIt->second;
			for (std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR *>::iterator tsIt= theMap.begin();
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
