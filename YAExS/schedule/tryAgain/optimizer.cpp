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

    shouldPrint_ = false;
}

Optimizer::Optimizer(bool enablePrinting)
{
    //Initialize SCIP
    SCIPcreate(&scip_);

    //Load default plugins
    SCIPincludeDefaultPlugins(scip_);

    //Create a problem
    SCIPcreateProb(scip_, "Exam Scheduling",
                   NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    
    shouldPrint_ = enablePrinting;
}


//Destructor
Optimizer::~Optimizer()
{
     // Close things up
	if (shouldPrint_)
	{
		std::cout << "closing things up" << std::endl;
	}

	// release variables
	releaseExamIsAtVariables();
	releaseTwoPlusVariables();
	releaseThreePlusVariables();
	releaseConflictAtVariables();
	
	// release constaints
	releaseOnceConstraints();
	releaseOverloadConstraints();

	//close the SCIP environment:
    SCIPfree(&scip_);
}

void Optimizer::releaseExamIsAtVariables()
{
	if (shouldPrint_)
	{
		std::cout << "releasing exam is at variables" << std::endl;
	}

	for (std::unordered_map<Exam::EXAM_ID, std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR*> >::iterator examIt = examIsAt_.begin();
		examIt != examIsAt_.end(); examIt++)
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
 	if (shouldPrint_)
 	{
 		std::cout << "releasing two plus variables" << std::endl;
 	}

 		//std::unordered_map <PERSON_ID, SCIP_VAR * > twoPlus;
	for (std::unordered_map <Person::PERSON_ID, SCIP_VAR *>::iterator it = twoPlus_.begin();
		 it != twoPlus_.end(); it++)
	{
		SCIPreleaseVar(scip_, & it->second);
	}

}

void Optimizer::releaseThreePlusVariables()
{
 	if (shouldPrint_)
	{
		std::cout << "releasing three plus at variables" << std::endl;
	}

 		//std::unordered_map <PERSON_ID, SCIP_VAR * > threePlus;
	for (std::unordered_map <Person::PERSON_ID, SCIP_VAR *>::iterator it = threePlus_.begin();
		 it != threePlus_.end(); it++)
	{
		SCIPreleaseVar(scip_, & it->second);
	}

}

void Optimizer::releaseConflictAtVariables()
{
	if (shouldPrint_)
	{
		std::cout << "releasing conflict at variables" << std::endl;
	}

	for (std::unordered_map<Person::PERSON_ID, std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR*> >::iterator personIt = conflictAt_.begin();
		personIt != conflictAt_.end(); personIt++)
	{
		std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR *> theMap;
		theMap  = personIt->second;
		for (std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR *>::iterator tsIt= theMap.begin();
			tsIt != theMap.end(); tsIt++)
		{	
			//if (shouldPrint_)
			//	std::cout << "\t" << personIt->first << " at time " << tsIt->first << std::endl;
			
			SCIPreleaseVar(scip_, & tsIt->second);
		}
	}
}

void Optimizer::releaseOnceConstraints()
{

	if (shouldPrint_)
	{
		std::cout << "releasing once constraints " << std::endl;
	}

	for (std::unordered_map<Exam::EXAM_ID, std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR*> >::iterator examIt = examIsAt_.begin();
		examIt != examIsAt_.end(); examIt++)
	{
		//std::cout << "releasing once constraint" << std::endl;
		SCIPreleaseCons(scip_, & onceCon_[examIt->first]);
	}
}

void Optimizer::releaseOverloadConstraints()
{
	if (shouldPrint_)
	{
		std::cout << "releasing overload constraints" << std::endl;
	}
	// set the two plus and three plus variables constraint
    // std::unordered_map< Person::PERSON_ID, std::unordered_map<Day::DAY_ID, SCIP_CONS *> >overloadCon;

    for ( std::unordered_map< Person::PERSON_ID, std::unordered_map<Day::DAY_ID, SCIP_CONS *> >::iterator pit = overloadCon_.begin();
    	pit != overloadCon_.end(); pit++)
    {
    	//if (shouldPrint_)
    	//	std::cout << "\t for person " << pit->first << std::endl;

    	std::unordered_map<Day::DAY_ID, SCIP_CONS*> theMap = pit->second;
    	for (std::unordered_map<Day::DAY_ID, SCIP_CONS*>::iterator mapIt = theMap.begin(); 
    		mapIt != theMap.end(); mapIt++)
    	{
    		//if (shouldPrint_)
	    	//	std::cout << " \t\t day " << mapIt->first << std::endl;
    		SCIPreleaseCons(scip_, & mapIt->second );
    	}
    }
}



//Loads a model into SCIP
void Optimizer::loadModel(const std::vector<Exam> & exams, 
		const std::vector<Person* > & people,
		int numDays, int slotsPerDay)
{
	if (shouldPrint_)
	{
		std::cout << "loading model into SCIP" << std::endl;
	}

	// set up the days
	loadDays(numDays, slotsPerDay);
		// set up the people
	loadAllPeople(people);

	// load the exam is at variables
	loadExamIsAtVariables(exams);


	// load the once constraints (each exam meets at exactly one time slot)
	// doesn't need parameters because just uses the examIsAtVariables
	loadOnceConstraints();
	

	// load the too many exams in a day variables
	loadTwoPlusVariables(people);
	loadThreePlusVariables(people);

	// load the overload constraints to constrain the two and three plus variables
	loadOverloadConstraints();

	// load conflict  at variables
	loadConflictAtVariables(people);

	// set the conflict at variables with conflict constraints
	// loadConflictConstraints;
}


void Optimizer::loadExamIsAtVariables(const std::vector<Exam> & exams)
{
	// check that the variables haven't been loaded already;
	if ( !examIsAt_.empty() )
		std::cerr << " exam is at variable map is NOT empty. Only call loadExamIsAtVariables once." << std::endl;

	else 
	{
		bool isInitial = true;
		bool canRemoveInAging = false;

		if (shouldPrint_)
			std::cout << "\nloading exam is at variables" << std::endl;

		for (std::vector<Exam>::const_iterator examIt = exams.begin(); 
			examIt != exams.end(); examIt++)
		{	
			std::unordered_map<TimeSlot::TIMESLOT_ID,  SCIP_VAR *> aMap;

			for (std::vector<Day>::const_iterator dayIt = days_.begin();
				dayIt != days_.end(); dayIt++)
			{
				std::vector<TimeSlot> slots = dayIt->getSlots();

				for (std::vector<TimeSlot>::iterator tsIt = slots.begin(); 
						tsIt != slots.end(); tsIt++)
				{
					if (shouldPrint_)
					{
						std::cout << "adding " << examIt->getId();
						std::cout << " exam variable for time slot ";
						std::cout  << tsIt->getId() << std::endl;
					}

					const char * name = examAtVariableName(*examIt, *tsIt);

					// for now we don't care what time an exam is scheduled in particular
					double objCoefExam = 0.0;

					// create the exam is at variable
					SCIP_VAR * examVar;
					SCIPcreateVar(scip_, & examVar, name, 0.0, 1.0,
							objCoefExam, SCIP_VARTYPE_BINARY,
							isInitial, canRemoveInAging,
							NULL, NULL, NULL, NULL, NULL);

					// add exam is at variable to SCIP
					SCIPaddVar(scip_, examVar);

					// store exam is at variable point for later use
					aMap[tsIt->getId()] = examVar;

				} // end time slot for loop
				
			} // end day for loop

			// store the exam is at variables for this exam for later use
			examIsAt_[examIt->getId()] = aMap;
		} // end exam for loop

	} // end else
}

void Optimizer::loadConflictAtVariables(const std::vector<Person *> & people)
{
	// check that the variables haven't been loaded already;
	if ( !conflictAt_.empty() )
		std::cerr << " conflict at variable map is NOT empty. Only call loadConflictAtVariables once." << std::endl;

	else 
	{
		bool isInitial = true;
		bool canRemoveInAging = false;

		if (shouldPrint_)
			std::cout << "\nloading conflict  at variables" << std::endl;

	// std::unordered_map <Person::PERSON_ID, std::unordered_map <TimeSlot::TIMESLOT_ID, SCIP_VAR *> > conflictAt_;
	
		for (std::vector<Person *>::const_iterator personIt = people.begin(); 
			personIt != people.end(); personIt++)
		{	
			std::unordered_map<TimeSlot::TIMESLOT_ID,  SCIP_VAR *> aMap;
			Person::PERSON_ID personID = (*personIt)->getId();

			// loop through the time slots, creating conflict variable for each
			for (std::vector<TimeSlot>::iterator tsIt = allTimeSlots_.begin(); 
					tsIt != allTimeSlots_.end(); tsIt++)
			{
				if (shouldPrint_)
				{
					std::cout << "adding " << personID << "'s";
					std::cout << " 'conflict at' variable for time slot "  << tsIt->getId();
				}

				const char * name = conflictAtVariableName(personID, tsIt->getId());

				// Conflicts are worth 20
				double objCoefConflict = 20.0;

				// create the exam is at variable
				SCIP_VAR * conflictVar;
				SCIPcreateVar(scip_, & conflictVar, name, 0.0, 1.0,
						objCoefConflict, SCIP_VARTYPE_BINARY,
						isInitial, canRemoveInAging,
						NULL, NULL, NULL, NULL, NULL);

				// add exam is at variable to SCIP
				SCIPaddVar(scip_, conflictVar);

				// store exam is at variable point for later use
				aMap[tsIt->getId()] = conflictVar;

			} // end time slot for loop
				
			
			// store the exam is at variables for this exam for later use
			conflictAt_[personID] = aMap;
		} // end exam for loop

	} // end else
}

// load once constraints, which require that each exam meets at exactly one time slot
// doesn't need parameters because just uses examIsAt
void Optimizer::loadOnceConstraints()
{

	// check that the variables haven't been loaded already;
	if ( !onceCon_.empty() )
		std::cerr << " once constraints map is not empty. only call loadOnceConstraints once" << std::endl;
	else if ( examIsAt_.empty() )
	{
		std::cerr << " examIsAt variable map is null or empty. need to call loadExamIsAtVariables beforeLoadOnceConstraints." << std::endl;
	}
	else 
	{
		if(shouldPrint_)
			std::cout << "\nloading the once constraints." << std::endl;

		bool isInitial = true;
		double lbound = 1.0;
		double ubound = 1.0;
	

		for (std::unordered_map<Exam::EXAM_ID, std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR*> >::iterator examIt = examIsAt_.begin();
			examIt != examIsAt_.end(); examIt++)
		{

			Exam::EXAM_ID eid = examIt->first;

			if (shouldPrint_)
				std::cout << "creating once constraint for exam " << eid << std::endl;

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
				if (shouldPrint_)
				{
					std::cout << " adding variable to once constraint for exam " << examIt->first;
					std::cout << " and time " << tsIt ->first << std::endl;
				}
				double coef = 1.0;
				SCIPaddCoefLinear(scip_, constraintPointer, tsIt->second, coef);
			} // end time slot loop

		
			// add the constraint to the problem: (not in queens documentation but I think its necessary)
		 	SCIPaddCons(scip_, constraintPointer);

		 	// keep track of the constraint for later
		 	onceCon_[eid] = constraintPointer;

		} // end exam loop

		
	} // end else
}

// variables that indicate when a person has two or more exams in one day
void Optimizer::loadTwoPlusVariables(const std::vector<Person* > & people)
{
	if (!twoPlus_.empty())
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
			if (shouldPrint_)	
			{
				std::cout << "adding a two plus variable for person ";
				std::cout << (*it)->getId() << std::endl;
			}
			
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
			twoPlus_[(*it)->getId()] = theVar;

		} // end person for loop

	} // end else
}

// variables that indicate when a person has three or more exams in one day
void Optimizer::loadThreePlusVariables(const std::vector<Person* > & people)
{
	if (!threePlus_.empty())
		std::cerr << " threePlus variable map is not empty. Only call loadThreePlusVariables once!" << std::endl;
	else
	{
		//std::cout << "loading the three plus variables" << std::endl;
		
		bool isInitial = true;
		bool canRemoveInAging = false;

		// three exams in one day is worth 10;
		double objCoefTwo = 10.0;

		 //std::unordered_map <PERSON_ID, SCIP_VAR * > threePlus;
		for (std::vector<Person*>::const_iterator it = people.begin(); it != people.end(); it++)
		{
			if (shouldPrint_)
			{
				std::cout << "adding a three plus variable for person ";
				std::cout << (*it)->getId() << std::endl;
			}

			const char * name = threePlusVariableName(*it);

			// create the variable
			SCIP_VAR * theVar;
			SCIPcreateVar(scip_, & theVar, name, 0.0, 1.0,
				objCoefTwo, SCIP_VARTYPE_BINARY,
				isInitial, canRemoveInAging,
				NULL, NULL, NULL, NULL, NULL);

			// add variable to SCIP problem		
			SCIPaddVar(scip_, theVar);

			// keep variable pointer for later use
			threePlus_[(*it)->getId()] = theVar;

		} // end person for loop

	} // end else
}

// constraints that set up the two plus and three plus values
void Optimizer::loadOverloadConstraints()
{
	// set up:
	//  std::unordered_map< Person::PERSON_ID, 
	//			std::unordered_map<Day::DAY_ID, SCIP_CONS *> >overloadCon;

	//in zimpl:
	//subto overload: 
	//	forall <p,d> in PEOPLE cross DAYS do
	//		sum <e> in EXAMS[p]: 
	//			(sum <t> in DAYSLOT[d]: examIsAt [e,t] - twoPlus[p] - threePlus[p]) <= 1;

	bool isInitial = true;
	// there is no lower bound, so use negative infinity
	double lbound = -SCIPinfinity(scip_);
	double ubound = 1.0;

	// loop over people
	for (std::unordered_map <Person::PERSON_ID, SCIP_VAR *>::iterator pIt= twoPlus_.begin();
			pIt != twoPlus_.end(); pIt++)
	{
		std::unordered_map<Day::DAY_ID, SCIP_CONS *> personConMap;

		Person::PERSON_ID personID = pIt->first;

		// loop over days
		for (std::vector<Day>::const_iterator dIt = days_.begin();
			dIt != days_.end(); dIt++)
		{
			Day::DAY_ID dayId = dIt->getId(); 

			// create a constraint
			SCIP_CONS * constraintPointer;
			const char* conName = overloadConName( personID, *dIt );

			if (shouldPrint_)
			{
				std::cout << "creating an overload constraint for person " << personID;
				std::cout << " and day " << dayId << std::endl;
			}

			SCIPcreateConsLinear(scip_, & constraintPointer, conName,
				0, NULL, NULL, lbound, ubound, isInitial,
				TRUE, TRUE, TRUE, TRUE, FALSE,
				FALSE, FALSE, FALSE, FALSE);

			std::vector<TimeSlot> slots = dIt->getSlots();

			// loop over examIsAt variables for this day, 
			for (std::unordered_map<Exam::EXAM_ID, std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR*> >::iterator eIt = examIsAt_.begin();
					eIt != examIsAt_.end(); eIt++)
			{
				// we only add an exam for this person if they have this exam!
				if ( personHasExam(personID, eIt->first) )
				{

					std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR*> theMap = eIt->second;

					for (std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR*>::iterator tsIt = theMap.begin();
						tsIt != theMap.end(); tsIt++)
					{
						// only add the variable for this time slot if the time slot is on the proper day 
						TimeSlot::TIMESLOT_ID tsID = tsIt->first;
						if ( dayHasSlot(dayId, tsID) )
						{
							if (shouldPrint_)
							{
								std::cout << "\t adding exam is at variable for exam " << eIt->first;
								std::cout << " at timeslot " << tsID << std::endl;
							}

							//add examIsAt variable to constraint
							double coef = 1.0;
							SCIPaddCoefLinear(scip_, constraintPointer, tsIt->second, coef);
						}

					} // end time slot loop
				}
			}
			// add two plus variables
			double coef = -1.0;
			SCIPaddCoefLinear(scip_, constraintPointer, twoPlus_[personID], coef);

			// add three plus variables
			coef = -1.0;
			SCIPaddCoefLinear(scip_, constraintPointer, threePlus_[personID], coef);

			// add the constraint to the problem
		 	SCIPaddCons(scip_, constraintPointer);

		 	//save this pointer for later use
			personConMap[dayId] = constraintPointer;

			
		 } // end day loop

		
		 // keep track of the all the constraints for this person for later
		 overloadCon_[personID] = personConMap;

	} // end person loop
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
		std::cout << "\n\nSolution found!" << std::endl;

		std::cout << "Objective value: " << SCIPgetSolOrigObj(scip_, sol) << std::endl;
		
		printExamIsAtVariables(sol);
		
		printTwoPlusVariables(sol);
		printThreePlusVariables(sol);
		
		std::cout << std::endl;
	} // end else
}

// print the nonzero values of the exam is at variables
void Optimizer::printExamIsAtVariables(SCIP_SOL* sol)
{
	for (std::unordered_map<Exam::EXAM_ID, std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR*> >::iterator examIt = examIsAt_.begin();
		examIt != examIsAt_.end(); examIt++)
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
}

// Print the nonzero values of the two plus variables
void Optimizer::printTwoPlusVariables(SCIP_SOL* sol)
{
 	//std::unordered_map <PERSON_ID, SCIP_VAR * > twoPlus;
	for (std::unordered_map <Person::PERSON_ID, SCIP_VAR *>::iterator twoIt = twoPlus_.begin();
		 twoIt != twoPlus_.end(); twoIt++)
	{
		double value =  SCIPgetSolVal(scip_, sol, twoIt->second);
		if (value != 0.0)
		{
			std::cout << '\t' << twoIt->first << " (Person)";
			std::cout << " has two or more exams on some day."<< std::endl;
		}
		/*
		else
		{
			std::cout << '\t' << twoIt->first << " (Person)";
			std::cout << " never has two or more exams in one day" << std::endl;
		}
		*/
	} // end two plus for loop
}


// Print the nonzero values of the three plus variables
void Optimizer::printThreePlusVariables(SCIP_SOL* sol)
{
 	//std::unordered_map <PERSON_ID, SCIP_VAR * > threePlus;
	for (std::unordered_map <Person::PERSON_ID, SCIP_VAR *>::iterator threeIt = threePlus_.begin();
		 threeIt != threePlus_.end(); threeIt++)
	{
		double value =  SCIPgetSolVal(scip_, sol, threeIt->second);
		if (value != 0.0)
		{
			std::cout << '\t' << threeIt->first << " (Person)";
			std::cout << " has three or more exams on some day!"<< std::endl;
		}
		/*
		else
		{
			std::cout << '\t' << threeIt->first << " (Person)";
			std::cout << " never has three or more exams in one day" << std::endl;
		}
		*/
	} // end three plus for loop
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
   ss << "_examAt";
   std::string str(ss.str());
   const char* name = str.c_str();

   return name;
}


const char* Optimizer::twoPlusVariableName( Person * person)
{
	return (person->getId() + "_twoPlus").c_str();
}

const char* Optimizer::threePlusVariableName( Person * person)
{
	return (person->getId() + "_threePlus").c_str();
}

const char * Optimizer::conflictAtVariableName(Person::PERSON_ID pID, TimeSlot::TIMESLOT_ID tsID)
{
	std::stringstream ss;
	ss << pID << "_" << tsID;
	ss << "_conflictAt";
	std::string str(ss.str());
	const char* name = str.c_str();

	return name;
}

const char* Optimizer::onceConName( Exam::EXAM_ID eid )
{
	return ("once_" + eid).c_str();
}

const char* Optimizer::onceConName( const Exam & exam )
{
	return onceConName(exam.getId());
}

const char* Optimizer::overloadConName( Person::PERSON_ID pID, const Day & day )
{
	std::string s = "overload_" + pID;
	s += "_" + day.getId();
	return s.c_str();
}


// set up the days with their time slots
void Optimizer::loadDays( int numDays, int slotsPerDay)
{
	if (!days_.empty())
		std::cerr << "days is not empty. only call loadDays once." << std::endl;
	if (!allTimeSlots_.empty())
		std::cerr << "all time slots is not empty. only call loadDays once." << std::endl;

	int slotId = 0;
	for (int dayNum = 0; dayNum < numDays; dayNum++)
	{
		std::vector<TimeSlot> slots;
		for (int slotNum = 0; slotNum < slotsPerDay; slotNum++)
		{
			TimeSlot s(slotId);
			slots.push_back(s);

			allTimeSlots_.push_back(s);

			slotId++;
		}
		Day d(dayNum, slots);
		days_.push_back(d);
	}

	if (shouldPrint_)
	{
		std::cout << "the following days were created:" << std::endl;
		for (std::vector<Day>::iterator dIt = days_.begin(); 
				dIt != days_.end(); dIt++)
		{
			std::cout << "\tday: " << dIt->getId() << std::endl;
			std::vector<TimeSlot> slots = dIt->getSlots();
			for(std::vector<TimeSlot>::iterator tsIt = slots.begin();
				 tsIt != slots.end(); tsIt++)
			{
				std::cout << "\t\ttimeslot: " << tsIt->getId() << std::endl;
			}
			std::cout << std::endl;
		}
	}

}

 void Optimizer::loadAllPeople( const std::vector<Person*> & people)
 {
 	if (shouldPrint_)
 		std::cout << "loading all the people" << std::endl;

 	for (std::vector<Person*>::const_iterator it = people.begin(); 
 			it != people.end(); it++)
 	{
 		Person::PERSON_ID pId= (*it)->getId();
 		allPeople_[pId] = *it;
 	}
 }
 	
bool Optimizer::personHasExam(Person::PERSON_ID personID, Exam::EXAM_ID examID)
{
	Person * p = allPeople_[personID];
	if (p == NULL)
	{
		std::cerr << "person not found in allPeople map. Call loadAllPeople before personHasExam." << std::endl;
		return false;
	}

	return Person::personHasExam(p, examID);
}

bool Optimizer::dayHasSlot(Day::DAY_ID dayID, TimeSlot::TIMESLOT_ID tsID)
{
	for (std::vector<Day>::iterator dayIt = days_.begin();
			dayIt != days_.end(); dayIt++)
	{
		// if we are at the right day, check the time slots
		if (dayIt->getId() == dayID)
		{
			std::vector<TimeSlot> slots = dayIt->getSlots();
			for (std::vector<TimeSlot>::iterator tsIt = slots.begin();
					tsIt != slots.end(); tsIt++)
			{
				if (tsIt->getId() == tsID)
				{
					return true;
				}
			}
		}
	}
	return false;
}