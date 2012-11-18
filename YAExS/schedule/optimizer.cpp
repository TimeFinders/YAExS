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
	releaseConflictConstraints();

	//close the SCIP environment:
    SCIPfree(&scip_);
}

void Optimizer::releaseExamIsAtVariables()
{
	if (shouldPrint_)
	{
		std::cout << "releasing exam is at variables" << std::endl;
	}

	for (std::unordered_map<Exam::EXAM_ID, std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR*> >::const_iterator examIt = examIsAt_.begin();
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

	for (std::unordered_map<Person::PERSON_ID, std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR*> >::const_iterator personIt = conflictAt_.begin();
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

void Optimizer::releaseConflictConstraints()
{
	if (shouldPrint_)
	{
		std::cout << "releasing conflict constraints" << std::endl;
	}
	
	// std::unordered_map< Person::PERSON_ID, std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_CONS *> >conflictCon_;

    for ( std::unordered_map<Person::PERSON_ID, std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_CONS*> >::iterator personIt = conflictCon_.begin();
    	personIt != conflictCon_.end(); personIt++)
    {
    	//if (shouldPrint_)
    	//	std::cout << "\t for person " << personIt->first << std::endl;

    	std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_CONS *> theMap = personIt->second;
    	for (std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_CONS *>::iterator mapIt = theMap.begin(); 
    		mapIt != theMap.end(); mapIt++)
    	{
    		//if (shouldPrint_)
	    	//	std::cout << " \t\t time slot " << mapIt->first << std::endl;
    		SCIPreleaseCons(scip_, & mapIt->second );
    	}
    }
}


//Load a model into SCIP
void Optimizer::loadModel(std::list<Exam> & exams, 
		const std::vector<Person* > & people,
		int numDays, int slotsPerDay)
{
	if (shouldPrint_)
	{
		std::cout << "loading model into SCIP" << std::endl;
	}

	// save the exams for later;
	std::list<Exam> * examPointer = &exams;
	this->exams_ = examPointer;

	// set up the days
	loadDays(numDays, slotsPerDay);
		// set up the people
	loadAllPeople(people);

	// load the exam is at variables
	loadExamIsAtVariables();


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
	 loadConflictConstraints();
}


void Optimizer::loadExamIsAtVariables()
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

		for (std::list<Exam>::const_iterator examIt = exams_->begin(); 
			examIt != exams_->end(); examIt++)
		{	
			std::unordered_map<TimeSlot::TIMESLOT_ID,  SCIP_VAR *> aMap;

			for (std::vector<TimeSlot>::iterator tsIt = allTimeSlots_.begin(); 
					tsIt != allTimeSlots_.end(); tsIt++)
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

			// store the exam is at variables for this exam for later use
			examIsAt_[examIt->getId()] = aMap;
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
		double lBound = 1.0;
		double uBound = 1.0;
	

		for (std::unordered_map<Exam::EXAM_ID, std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR*> >::iterator examIt = examIsAt_.begin();
			examIt != examIsAt_.end(); examIt++)
		{

			Exam::EXAM_ID eid = examIt->first;

			if (shouldPrint_)
				std::cout << "creating once constraint for exam " << eid << std::endl;

			SCIP_CONS * constraintPointer;
			const char * conName = onceConName(eid);
			SCIPcreateConsLinear(scip_, & constraintPointer, conName,
				0, NULL, NULL, lBound, uBound, isInitial,
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

		// two exams in one day is worth very little in the objective
		double objCoefTwo = 0.1;

		 //std::unordered_map <PERSON_ID, SCIP_VAR * > twoPlus;
		for (std::vector<Person*>::const_iterator it = people.begin(); it!=people.end(); it++)
		{
			if (shouldPrint_)	
			{
			//	std::cout << "adding a two plus variable for person ";
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
			//	std::cout << "adding a three plus variable for person ";
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

std::unordered_map<TimeSlot::TIMESLOT_ID, std::list<SCIP_VAR *> > Optimizer::personalExamIsAtVariables(Person * person)
{
	std::unordered_map<TimeSlot::TIMESLOT_ID, std::list<SCIP_VAR *> > variables;

	std::list<Exam> exams = person->getExams();

	for (std::list<Exam>::iterator examIt = exams.begin(); 
		examIt != exams.end(); examIt++)
	{
		Exam::EXAM_ID examID = examIt->getId();
		std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR *> examIsAtMap;
		examIsAtMap = examIsAt_[examID];

		// add exam is at variables to the variables map
		for (std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR *>::iterator varIt = examIsAtMap.begin();
			varIt != examIsAtMap.end(); varIt++)
		{
			TimeSlot::TIMESLOT_ID timeslotID = varIt->first;
			SCIP_VAR * variable = varIt->second;
			variables[timeslotID].push_back(variable); 
		}
		
	} // end exam loop

	return variables;
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
					std::cout << " 'conflict at' variable for time slot ";
					std::cout << tsIt->getId() << std::endl;;
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

void Optimizer::loadConflictConstraints(Person * person)
{
	// same for all conflict constraints
	double lBound = - SCIPinfinity(scip_);
	double uBound = 1.0;
	bool isInitial = false;
	double conflictCoef = -1.0;
	double examCoef = 1.0;
	
	Person::PERSON_ID personID = person->getId();

	if (shouldPrint_)
		std::cout << "\t for person " << personID << std::endl;

	// for each person the variables map of a given time slot has all of the 
	// exam is at variables for that time slot that that person takes.
	std::unordered_map<TimeSlot::TIMESLOT_ID, std::list<SCIP_VAR *> > personalExamVarMap;
	personalExamVarMap = personalExamIsAtVariables(person);

	// note if the person only has one exam we dont have to bother making conflict constraints;

	/*
	if (shouldPrint_)
	{
		std::cout << " we will be using the followig variables:" << std::endl;
		for (std::unordered_map<TimeSlot::TIMESLOT_ID, std::list<SCIP_VAR*> >::iterator it = variables.begin();
			it != variables.end(); it++)
		{void printExamSchedule
			std::cout << "\ttime: " << it->first << std::endl;
			std::list<SCIP_VAR*> varList = it->second;
			for (std::list<SCIP_VAR*>::iterator varIt = varList.begin();
				varIt != varList.end(); varIt++)
			{
				std::cout << "\t\t some variable" << std::endl;
				// cant seem to do (*varIt)->name
			}
		}
	}
	*/
	
	//will be filled up with constraint pointers
	std::unordered_map <TimeSlot::TIMESLOT_ID, SCIP_CONS *> constraintMap;

	// all the conflict variables for this person
	std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR *> conflictVariableMap;
	conflictVariableMap = conflictAt_[personID];
	
	// loop through the time slots, creating a constraint for each
	for ( std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR *>::iterator conflictVarIt = conflictVariableMap.begin();
		conflictVarIt != conflictVariableMap.end(); conflictVarIt++ )
	{
		TimeSlot::TIMESLOT_ID slotID = conflictVarIt->first;

		if (shouldPrint_)
			std::cout << "\t\t at time " << slotID << std::endl;

		// make a constraint
		SCIP_CONS * constraintPointer;
		const char* constraintName = conflictConName( personID, slotID );

		SCIPcreateConsLinear(scip_, & constraintPointer, constraintName,
			0, NULL, NULL, lBound, uBound, isInitial,
			TRUE, TRUE, TRUE, TRUE, FALSE,
			FALSE, FALSE, FALSE, FALSE);

		// add the variables to the constraint

		// conflict variable
		if(shouldPrint_)
				std::cout << "\t\t\t adding conflict variable" << std::endl;
		SCIP_VAR * conflictVariable = conflictVarIt->second;
		SCIPaddCoefLinear(scip_, constraintPointer, conflictVariable, conflictCoef);


		// exam is at variables
		std::list<SCIP_VAR *> examVariables = personalExamVarMap[slotID];
		for(std::list<SCIP_VAR *>::iterator examVarIt = examVariables.begin();
			examVarIt != examVariables.end(); examVarIt++)
		{
			if(shouldPrint_)
				std::cout << "\t\t\t adding an exam is at variable " << std::endl;

			SCIPaddCoefLinear(scip_, constraintPointer, *examVarIt, examCoef);
		} // end exam is at variable loop

		// add the constraint to the problem
	 	SCIPaddCons(scip_, constraintPointer);

	 	// keep track of the constraint for later
	 	constraintMap[slotID] = constraintPointer;
	 	
	} // end time slot loop

	// save all of this persons constraints
	conflictCon_[personID] = constraintMap;
}

void Optimizer::loadConflictConstraints()
{
	if (shouldPrint_)
		std::cout << "loading conflict constraints" << std::endl;

	// for each person's conflict variables we will amke a
	for (std::unordered_map<Person::PERSON_ID, std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR*> >::iterator personIt = conflictAt_.begin();
		personIt != conflictAt_.end(); personIt++)
	{
		Person::PERSON_ID personID = personIt->first;
		Person * person = allPeople_[personID];

		if (person == NULL)
		{
			std::cerr << "person not found!" << std::endl;
			continue;
		}
		else
		{
			loadConflictConstraints(person);
		}
		
	} // end person loop

}

// create a single overload constraint for the given person and the  day
SCIP_CONS * Optimizer::createPersonalOverloadConstraint(Person::PERSON_ID personID, const Day & day)
{
	//in zimpl:
	//subto overload: 
	//	forall <p,d> in PEOPLE cross DAYS do
	//		sum <e> in EXAMS[p]: 
	//			(sum <t> in DAYSLOT[d]: examIsAt [e,t] - twoPlus[p] - threePlus[p]) <= 1;

	bool isInitial = true;
	// there is no lower bound, so use negative infinity
	double lBound = -SCIPinfinity(scip_);
	double uBound = 1.0;

	Day::DAY_ID dayID = day.getId();

	// create a constraint
	SCIP_CONS * constraintPointer;
	const char* constraintName = overloadConName( personID, dayID );

	if (shouldPrint_)
	{
		std::cout << "creating an overload constraint for person " << personID;
		std::cout << " and day " << dayID << std::endl;
	}

	SCIPcreateConsLinear(scip_, & constraintPointer, constraintName,
		0, NULL, NULL, lBound, uBound, isInitial,
		TRUE, TRUE, TRUE, TRUE, FALSE,
		FALSE, FALSE, FALSE, FALSE);

	std::vector<TimeSlot> slots = day.getSlots();

	Person * person = allPeople_[personID];
	std::list<Exam> exams = person->getExams();

	for (std::list<Exam>::const_iterator examIt = exams.begin(); 
		examIt != exams.end(); examIt++)
	{
		Exam::EXAM_ID examID = examIt->getId();
		std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR*> examIsAtVariableMap;
		examIsAtVariableMap = examIsAt_[examID];


		for (std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR*>::const_iterator tsIt = examIsAtVariableMap.begin();
			tsIt != examIsAtVariableMap.end(); tsIt++)
		{
			// only add the variable for this time slot if the time slot is on the proper day 
			TimeSlot::TIMESLOT_ID tsID = tsIt->first;
			if ( dayHasSlot(dayID, tsID) )
			{
				if (shouldPrint_)
				{
					std::cout << "\t adding exam is at variable for exam " << examID;
					std::cout << " at timeslot " << tsID << std::endl;
				}

				//add examIsAt variable to constraint
				double examIsAtCoefficient = 1.0;
				SCIPaddCoefLinear(scip_, constraintPointer, tsIt->second, examIsAtCoefficient);
			}

		} // end time slot loop
		
	} // end exam slot loop

	// add two plus variables
	double twoPlusCoefficient = -1.0;
	SCIPaddCoefLinear(scip_, constraintPointer, twoPlus_[personID], twoPlusCoefficient);

	// add three plus variables
	double threePlusCoefficient = -1.0;
	SCIPaddCoefLinear(scip_, constraintPointer, threePlus_[personID], threePlusCoefficient);

	// add the constraint to the problem
 	SCIPaddCons(scip_, constraintPointer);

	return constraintPointer;
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

	// loop over people
	for (std::unordered_map <Person::PERSON_ID, SCIP_VAR *>::const_iterator pIt= twoPlus_.begin();
			pIt != twoPlus_.end(); pIt++)
	{
		std::unordered_map<Day::DAY_ID, SCIP_CONS *> personConMap;
		Person::PERSON_ID personID = pIt->first;

		// loop over days
		for (std::vector<Day>::const_iterator dIt = days_.begin();
			dIt != days_.end(); dIt++)
		{
			Day::DAY_ID dayID = dIt->getId(); 
			 	//save this pointer for later use
			SCIP_CONS * constraintPointer = createPersonalOverloadConstraint(personID, *dIt);
			personConMap[dayID] = constraintPointer;
			
		 } // end day loop

		 // keep track of the all the constraints for this person for later
		 overloadCon_[personID] = personConMap;

	} // end person loop
}

void Optimizer::printExamSchedule()
{
	printExamIsAtVariables();
}

void Optimizer::printSolutionAndNonzeroValues()
{

	if (solution_ == NULL)
	{
		std::cout << "no solution!" << std::endl;
	}
	else 
	{
		std::cout << "\n\nSolution found!" << std::endl;

		std::cout << "Objective value: " << SCIPgetSolOrigObj(scip_, solution_) << std::endl;
		
		printExamIsAtVariables();
		
		printTwoPlusVariables();
		printThreePlusVariables();
		printConflictVariables();
		
		std::cout << std::endl;
	} // end else
}

// print the nonzero values of the exam is at variables
void Optimizer::printExamIsAtVariables()
{
	for (std::unordered_map<Exam::EXAM_ID, std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR*> >::const_iterator examIt = examIsAt_.begin();
		examIt != examIsAt_.end(); examIt++)
	{
		std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR *> theMap;
		theMap  = examIt->second;
		for (std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR *>::iterator tsIt= theMap.begin();
			tsIt != theMap.end(); tsIt++)
		{	
			double value =  SCIPgetSolVal(scip_, solution_, tsIt->second);
			if (value != 0.0)
			{
				std::cout << "\t" << "exam " << examIt->first;
				std::cout << " is scheduled at time " << tsIt->first;
				std::cout << std::endl;
			}
		}
	}
}

// assign times to all exams based on the scip solution_
// this will overwrite previous exam time assignments.
void Optimizer::assignExamTimes()
{
	// loop through exams
	for (std::list<Exam>::iterator examIt = exams_->begin();
		examIt!=exams_->end(); examIt++)
	{
		TimeSlot slot = getOptimalExamTime(*examIt);
		examIt->assignTime(slot);

		std::cout << std::endl;
		TimeSlot::TIMESLOT_ID t= slot.getId();
		std::cout << "assigning time " << t;
		std::cout << " to exam " << examIt->getId() << std::endl;
		TimeSlot::TIMESLOT_ID s = (examIt->getTime()).getId();
		std::cout << "exam " << examIt->getId() << " has time " << s << std::endl;
	}

}

TimeSlot Optimizer::getOptimalExamTime(const Exam & exam)
{
	if (examIsAt_.empty())
	{
		std::cerr << "must load the model and create exam is at variables before getting optimal exam time" << std::endl;
		return TimeSlot(-1);
	}
	if (solution_ == NULL)
	{
		std::cerr << "must schedule and get solution before getting optimal exam time" << std::endl;
		return TimeSlot(-1);
	}

	Exam::EXAM_ID examID = exam.getId();
	std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR *> variableMap;
	variableMap = examIsAt_[examID];
	if (variableMap.empty())
	{
		std::cerr << "error invalid exam with id " << examID <<  std::endl;
		return TimeSlot(-1);
	}

	TimeSlot::TIMESLOT_ID timeID = -1;

	for (std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR*>::const_iterator varIt = variableMap.begin();
		varIt != variableMap.end(); varIt++)
	{
		double value =  SCIPgetSolVal(scip_, solution_, varIt->second);
		if (value != 0.0)
		{
			timeID = varIt->first;
			break;
		}
	}

	if (timeID == -1)
	{
		std::cerr << "error no best time found." << std::endl;
		return TimeSlot(-1);
	}
	else
	{
		return getTimeSlotById(timeID);
	}
}


void Optimizer::printConflictVariables()
{
	for (std::unordered_map<Person::PERSON_ID, std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR*> >::const_iterator personIt = conflictAt_.begin();
		personIt != conflictAt_.end(); personIt++)
	{
		std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR *> theMap;
		theMap  = personIt->second;
		for (std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR *>::const_iterator tsIt= theMap.begin();
			tsIt != theMap.end(); tsIt++)
		{	
			double value =  SCIPgetSolVal(scip_, solution_, tsIt->second);
			if (value != 0.0)
			{
				std::cout << "\t" << "person " << personIt->first;
				std::cout << " has " << value << " conflict(s) at time ";
				std::cout << tsIt->first << std::endl;
			}
		}
	}
}

// Print the nonzero values of the two plus variables
void Optimizer::printTwoPlusVariables()
{
 	//std::unordered_map <PERSON_ID, SCIP_VAR * > twoPlus;
	for (std::unordered_map <Person::PERSON_ID, SCIP_VAR *>::const_iterator twoIt = twoPlus_.begin();
		 twoIt != twoPlus_.end(); twoIt++)
	{
		double value =  SCIPgetSolVal(scip_, solution_, twoIt->second);
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
void Optimizer::printThreePlusVariables()
{
 	//std::unordered_map <PERSON_ID, SCIP_VAR * > threePlus;
	for (std::unordered_map <Person::PERSON_ID, SCIP_VAR *>::const_iterator threeIt = threePlus_.begin();
		 threeIt != threePlus_.end(); threeIt++)
	{
		double value =  SCIPgetSolVal(scip_, solution_, threeIt->second);
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
    solution_ = SCIPgetBestSol (scip_);
    assignExamTimes();
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

const char* Optimizer::overloadConName( Person::PERSON_ID personID, Day::DAY_ID dayID )
{
	std::string s = "overload_" + personID;
	s += "_" + dayID;
	return s.c_str();
}

const char* Optimizer::conflictConName( Person::PERSON_ID pID, TimeSlot::TIMESLOT_ID tsID)
{
	std::string s = "conflict_" + pID;
	s += "_" + tsID;
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
		for (std::vector<Day>::const_iterator dIt = days_.begin(); 
				dIt != days_.end(); dIt++)
		{
			std::cout << "\tday: " << dIt->getId() << std::endl;
			std::vector<TimeSlot> slots = dIt->getSlots();
			for(std::vector<TimeSlot>::const_iterator tsIt = slots.begin();
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


bool Optimizer::dayHasSlot(Day::DAY_ID dayID, TimeSlot::TIMESLOT_ID tsID)
{
	for (std::vector<Day>::const_iterator dayIt = days_.begin();
			dayIt != days_.end(); dayIt++)
	{
		// if we are at the right day, check the time slots
		if (dayIt->getId() == dayID)
		{
			std::vector<TimeSlot> slots = dayIt->getSlots();
			for (std::vector<TimeSlot>::const_iterator tsIt = slots.begin();
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


TimeSlot Optimizer::getTimeSlotById(TimeSlot::TIMESLOT_ID timeID)
{
	for (std::vector<TimeSlot>::iterator it = allTimeSlots_.begin(); 
		it != allTimeSlots_.end(); it++)
	{
		if (it->getId() == timeID)
			return *it;
	}

	std::cerr << "no time slot found" << std::endl;
	return TimeSlot(-1);
}