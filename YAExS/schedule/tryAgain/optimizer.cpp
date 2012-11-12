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
     // Close things up
	//std::cout << "closing things up" << std::endl;

	// release variables
	releaseExamIsAtVariables();
	//releaseTwoPlusVariables();

	// release constaints
	releaseOnceConstraints();
	

	
	//close the SCIP environment:
    SCIPfree(&scip_);
}

void Optimizer::releaseExamIsAtVariables()
{
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
}

void Optimizer::releaseTwoPlusVariables()
{
 		//std::unordered_map <PERSON_ID, SCIP_VAR * > twoPlus;
	for (std::unordered_map <Person::PERSON_ID, SCIP_VAR *>::iterator it = twoPlus.begin();
		 it != twoPlus.end(); it++)
	{
		SCIPreleaseVar(scip_, & it->second);
	}

}

void Optimizer::releaseOnceConstraints()
{
	for (std::unordered_map<Exam::EXAM_ID, std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR*> >::iterator examIt = examIsAt.begin();
		examIt != examIsAt.end(); examIt++)
	{
		//std::cout << "releasing once constraint" << std::endl;
		SCIPreleaseCons(scip_, & onceCon[examIt->first]);
	}
}


//Loads a model into SCIP
void Optimizer::loadModel(const std::vector<Exam> & exams, 
		const std::vector<TimeSlot> & slots, 
		const std::vector<Person* > & people)
{

	// load the exam is at variables
	loadExamIsAtVariables(exams, slots);

	// load the once constraints (each exam meets at exactly one time slot)
	// doesn't need parameters because just uses the examIsAtVariables
	loadOnceConstraints();

	loadTwoPlusVariables(people);

}


void Optimizer::loadExamIsAtVariables(const std::vector<Exam> & exams, 
	const std::vector<TimeSlot> & slots)
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
			for (std::vector<TimeSlot>::const_iterator tsIt = slots.begin(); 
					tsIt != slots.end(); tsIt++)
			{
				//std::cout << "adding an exam variable for time slot "  << tsIt->getId() << std::endl;
				const char * name = examAtVariableName(*examIt, *tsIt);
			
				// just for testing, remove later
				double objCoefExam = 1 + 5 * (tsIt->getId());

				SCIP_VAR * examVar;
				SCIPcreateVar(scip_, & examVar, name, 0.0, 1.0,
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
	if ( !onceCon.empty() )
		std::cerr << " once constraints map is not empty. only call loadOnceConstraints once" << std::endl;
	else if ( examIsAt.empty() )
	{
		std::cerr << " examIsAt variable map is null or empty. need to call loadExamIsAtVariables beforeLoadOnceConstraints." << std::endl;
	}
	else 
	{
		bool isInitial = true;
		double lbound = 1.0;
		double ubound = 1.0;
	

		for (std::unordered_map<Exam::EXAM_ID, std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR*> >::iterator examIt = examIsAt.begin();
			examIt != examIsAt.end(); examIt++)
		{
			Exam::EXAM_ID eid = examIt->first;
			SCIP_CONS * constraintPointer;
			const char * conName = onceConName(eid);
			SCIPcreateConsLinear(scip_, & constraintPointer, conName,
				0, NULL, NULL, lbound, ubound, isInitial,
				TRUE, TRUE, TRUE, TRUE, FALSE,
				FALSE, FALSE, FALSE, FALSE);

			std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR *> theMap;
			theMap  = examIt->second;
			for (std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR *>::iterator tsIt= theMap.begin();
				tsIt != theMap.end(); tsIt++)
			{	
				//	std::cout << " adding variable to once constraint for exam " << examIt->first;
				//	std::cout << " and time " << tsIt ->first;
				SCIPaddCoefLinear(scip_, constraintPointer, tsIt->second, 1.0);
			} // end time slot loop

		
			// add the constraint to the problem: (not in queens documentation but I think its necessary)
		 	SCIPaddCons(scip_, constraintPointer);

		 	// keep track of the constraint for later
		 	onceCon[eid] = constraintPointer;

		} // end exam loop
		
	} // end else
}

void Optimizer::loadTwoPlusVariables(const std::vector<Person* > & people)
{
	if (!twoPlus.empty())
		std::cerr << " twoPlus variable map is not empty. Only call loadTwoPlusVariables once!" << std::endl;
	else
	{
		//std::cout << "loading the two plus variables" << std::endl;
		
		bool isInitial = true;
		bool canRemoveInAging = false;

		// two exams in one day is worth 1;
		double objCoefTwo = 1.0;

		 //std::unordered_map <PERSON_ID, SCIP_VAR * > twoPlus;
		for (std::vector<Person*>::const_iterator it = people.begin(); it!=people.end(); it++)
		{
			std::cout << "adding a two plus variable for person slot "  << (*it)->getId() << std::endl;
			const char * name = twoPlusVariableName(*it);

			// create the variable
			SCIP_VAR * theVar;
			SCIPcreateVar(scip_, & theVar, name, 0.0, 1.0,
				objCoefTwo, SCIP_VARTYPE_BINARY,
				isInitial, canRemoveInAging,
				NULL, NULL, NULL, NULL, NULL);

			// add variable to SCIP problem		
			SCIPaddVar(scip_, theVar);

			// keep variable pointer for later use
			twoPlus[(*it)->getId()] = theVar;

		} // end person for loop

	} // end else
}

void Optimizer::printSolutionAndNonzeroValues()
{

	SCIP_SOL* sol = SCIPgetBestSol(scip_);

	if (sol == NULL)
	{
		std::cout << "no solution!" << std::endl;
	}
	else 
	{
		std::cout << "\nSolution found!" << std::endl;
		
		// print the exam is at variables
		for (std::unordered_map<Exam::EXAM_ID, std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR*> >::iterator examIt = examIsAt.begin();
			examIt != examIsAt.end(); examIt++)
		{
			std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR *> theMap;
			theMap  = examIt->second;
			for (std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR *>::iterator tsIt= theMap.begin();
				tsIt != theMap.end(); tsIt++)
			{	
				double value =  SCIPgetSolVal(scip_, sol, tsIt->second);
				if (value != 0.0)
				{
					std::cout << "\t" << "exam " << examIt->first;
					std::cout << " is scheduled at time " << tsIt->first;

					std::cout << " (because examIsAt variable has value ";
					std::cout << SCIPgetSolVal(scip_, sol, tsIt->second) << ")";
					std::cout << std::endl; 
				}
			}
		}

		
		// Print the two plus variables
	 	//std::unordered_map <PERSON_ID, SCIP_VAR * > twoPlus;
		for (std::unordered_map <Person::PERSON_ID, SCIP_VAR *>::iterator tpIt = twoPlus.begin();
			 tpIt != twoPlus.end(); tpIt++)
		{
			double value =  SCIPgetSolVal(scip_, sol, tpIt->second);
			if (value != 0.0)
			{
				std::cout << '\t' << tpIt->first << " (Person)";
				std::cout << " has two or more exams on some day!"<< std::endl;
			}
			else
			{
				std::cout << '\t' << tpIt->first << " (Person)";
				std::cout << " never has more than one exam per day" << std::endl;
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


const char* Optimizer::examAtVariableName(const Exam & exam, const TimeSlot & timeslot)
{
    // could change to read later: http://www.cplusplus.com/reference/iostream/istream/read/

   std::stringstream ss;
   ss << exam.getId() << "_" << timeslot.getId();
   std::string str(ss.str());
   const char* name = str.c_str();

   return name;
}

const char* Optimizer::onceConName( const Exam::EXAM_ID & eid)
{
	return (eid + "_once").c_str();
}

const char* Optimizer::onceConName( const Exam & exam)
{
	return onceConName(exam.getId());
}

const char* Optimizer::twoPlusVariableName( Person * person)
{
	return (person->getId() + "_twoPlus").c_str();
}