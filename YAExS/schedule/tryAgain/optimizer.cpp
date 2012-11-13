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
	releaseTwoPlusVariables();
	releaseThreePlusVariables();

	
	// release constaints
	releaseOnceConstraints();
	releaseOverloadConstraints();

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

void Optimizer::releaseThreePlusVariables()
{
 		//std::unordered_map <PERSON_ID, SCIP_VAR * > threePlus;
	for (std::unordered_map <Person::PERSON_ID, SCIP_VAR *>::iterator it = threePlus.begin();
		 it != threePlus.end(); it++)
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

void Optimizer::releaseOverloadConstraints()
{
	// set the two plus and three plus variables constraint
    // std::unordered_map< Person::PERSON_ID, std::unordered_map<Day::DAY_ID, SCIP_CONS *> >overloadCon;

    for ( std::unordered_map< Person::PERSON_ID, std::unordered_map<Day::DAY_ID, SCIP_CONS *> >::iterator pit = overloadCon.begin();
    	pit != overloadCon.end(); pit++)
    {
    	std::unordered_map<Day::DAY_ID, SCIP_CONS*> theMap = pit->second;
    	for (std::unordered_map<Day::DAY_ID, SCIP_CONS*>::iterator mapIt = theMap.begin(); 
    		mapIt != theMap.end(); mapIt++)
    	{
    		std::cout << " releasing overload constraint" << std::endl;
    		SCIPreleaseCons(scip_, & mapIt->second );
    	}
    }
}

//Loads a model into SCIP
void Optimizer::loadModel(const std::vector<Exam> & exams, 
		const std::vector<Person* > & people,
		int numDays, int slotsPerDay)
{
	// set up the days with their time slots
	std::vector<Day> days;
	int slotId = 0;
	for (int dayNum = 0; dayNum < numDays; dayNum++)
	{
		std::vector<TimeSlot> slots;
		for (int slotNum = 0; slotNum < slotsPerDay; slotNum++)
		{
			TimeSlot s(slotId);
			slots.push_back(s);

			slotId++;
		}
		Day d(dayNum, slots);
		days.push_back(d);
	}

	// print everything to make sure it worked
	for (std::vector<Day>::iterator dIt = days.begin(); dIt != days.end(); dIt++)
	{
		std::cout << "day: " << dIt->getId();
		std::vector<TimeSlot> slots = dIt->getSlots();
		for(std::vector<TimeSlot>::iterator tsIt = slots.begin();
			 tsIt != slots.end(); tsIt++)
		{
			std::cout << "\t timeslot: " << tsIt->getId() << std::endl;
		}
		std::cout << std::endl;
	}

	// load the exam is at variables
	loadExamIsAtVariables(exams, days);


	// load the once constraints (each exam meets at exactly one time slot)
	// doesn't need parameters because just uses the examIsAtVariables
	loadOnceConstraints();
	

	// load the too many exams in a day variables
	loadTwoPlusVariables(people);
	loadThreePlusVariables(people);

	loadAllPeople(people);

	// load the overload constraints to constrain the two and three plus variables
	loadOverloadConstraints(days);

}


void Optimizer::loadExamIsAtVariables(const std::vector<Exam> & exams, 
	const std::vector<Day> & days)
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

			for (std::vector<Day>::const_iterator dayIt = days.begin();
				dayIt != days.end(); dayIt++)
			{
				std::vector<TimeSlot> slots = dayIt->getSlots();

				std::unordered_map<TimeSlot::TIMESLOT_ID,  SCIP_VAR *> aMap;
				for (std::vector<TimeSlot>::iterator tsIt = slots.begin(); 
						tsIt != slots.end(); tsIt++)
				{
					/*
					std::cout << "adding an " << examIt->getId();
					std::cout << " exam variable for time slot "  << tsIt->getId();
					std::cout << " on day " << dayIt->getId() << std::endl;
					*/
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
			} // end day for loop
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
				double coef = 1.0;
				SCIPaddCoefLinear(scip_, constraintPointer, tsIt->second, coef);
			} // end time slot loop

		
			// add the constraint to the problem: (not in queens documentation but I think its necessary)
		 	SCIPaddCons(scip_, constraintPointer);

		 	// keep track of the constraint for later
		 	onceCon[eid] = constraintPointer;

		} // end exam loop

		
	} // end else
}

// variables that indicate when a person has two or more exams in one day
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
			//std::cout << "adding a two plus variable for person "  << (*it)->getId() << std::endl;
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

// variables that indicate when a person has three or more exams in one day
void Optimizer::loadThreePlusVariables(const std::vector<Person* > & people)
{
	if (!threePlus.empty())
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
			//std::cout << "adding a three plus variable for person "  << (*it)->getId() << std::endl;
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
			threePlus[(*it)->getId()] = theVar;

		} // end person for loop

	} // end else
}

// constraints that set up the two plus and three plus values
void Optimizer::loadOverloadConstraints(const std::vector<Day> & days)
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
	for (std::unordered_map <Person::PERSON_ID, SCIP_VAR *>::iterator pIt= twoPlus.begin();
			pIt != twoPlus.end(); pIt++)
	{
		std::unordered_map<Day::DAY_ID, SCIP_CONS *> personConMap;

		Person::PERSON_ID personID = pIt->first;

		// loop over days
		for (std::vector<Day>::const_iterator dIt = days.begin();
			dIt != days.end(); dIt++)
		{
			Day::DAY_ID dayId = dIt->getId(); 

			// create a constraint
			SCIP_CONS * constraintPointer;
			const char* conName = overloadConName( personID, *dIt );

			std::cout << "creating an overload constraint for person " << personID;
			std::cout << " and day " << dayId << std::endl;
			SCIPcreateConsLinear(scip_, & constraintPointer, conName,
				0, NULL, NULL, lbound, ubound, isInitial,
				TRUE, TRUE, TRUE, TRUE, FALSE,
				FALSE, FALSE, FALSE, FALSE);

			std::vector<TimeSlot> slots = dIt->getSlots();

			// loop over examIsAt variables for this day, 
			for (std::unordered_map<Exam::EXAM_ID, std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR*> >::iterator eIt = examIsAt.begin();
					eIt != examIsAt.end(); eIt++)
			{
				// we only add an exam for this person if they have this exam!
				if ( personHasExam(personID, eIt->first) )
				{

					std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR*> theMap = eIt->second;

					for (std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR*>::iterator tsIt = theMap.begin();
						tsIt != theMap.end(); tsIt++)
					{
						std::cout << "\t adding exam is at variable for exam " << eIt->first;
						std::cout << " at timeslot " << tsIt->first << std::endl;

						//add examIsAt variable to constraint
						double coef = 1.0;
						SCIPaddCoefLinear(scip_, constraintPointer, tsIt->second, coef);

					} // end time slot loop
				}
			}
			// add two plus variables
			double coef = -1.0;
			SCIPaddCoefLinear(scip_, constraintPointer, twoPlus[personID], coef);

			// add three plus variables
			coef = -1.0;
			SCIPaddCoefLinear(scip_, constraintPointer, threePlus[personID], coef);

			// add the constraint to the problem
		 	SCIPaddCons(scip_, constraintPointer);

		 	//save this pointer for later use
			personConMap[dayId] = constraintPointer;

			
		 } // end day loop

		
		 // keep track of the all the constraints for this person for later
		 overloadCon[personID] = personConMap;

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
		for (std::unordered_map <Person::PERSON_ID, SCIP_VAR *>::iterator twoIt = twoPlus.begin();
			 twoIt != twoPlus.end(); twoIt++)
		{
			double value =  SCIPgetSolVal(scip_, sol, twoIt->second);
			if (value != 0.0)
			{
				std::cout << '\t' << twoIt->first << " (Person)";
				std::cout << " has two or more exams on some day!"<< std::endl;
			}
			else
			{
				std::cout << '\t' << twoIt->first << " (Person)";
				std::cout << " never has two or more exams in one day" << std::endl;
			}
		} // end two plus for loop


		// Print the three plus variables
	 	//std::unordered_map <PERSON_ID, SCIP_VAR * > threePlus;
		for (std::unordered_map <Person::PERSON_ID, SCIP_VAR *>::iterator threeIt = threePlus.begin();
			 threeIt != threePlus.end(); threeIt++)
		{
			double value =  SCIPgetSolVal(scip_, sol, threeIt->second);
			if (value != 0.0)
			{
				std::cout << '\t' << threeIt->first << " (Person)";
				std::cout << " has three or more exams on some day!"<< std::endl;
			}
			else
			{
				std::cout << '\t' << threeIt->first << " (Person)";
				std::cout << " never has three or more exams in one day" << std::endl;
			}
		} // end three plus for loop
		
	} // end else
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

const char* Optimizer::threePlusVariableName( Person * person)
{
	return (person->getId() + "_threePlus").c_str();
}

const char* Optimizer::overloadConName( Person::PERSON_ID pID, const Day & day )
{
	std::string s = "overload_" + pID;
	s += "_" + day.getId();
	return s.c_str();
}

 void Optimizer::loadAllPeople( const std::vector<Person*> & people)
 {
 	for (std::vector<Person*>::const_iterator it = people.begin(); 
 			it != people.end(); it++)
 	{
 		Person::PERSON_ID pId= (*it)->getId();
 		allPeople[pId] = *it;
 	}
 }
 	
bool Optimizer::personHasExam(Person::PERSON_ID personID, Exam::EXAM_ID examID)
{
	Person * p = allPeople[personID];
	if (p == NULL)
	{
		std::cerr << "person not found in allPeople map. Call loadAllPeople before personHasExam." << std::endl;
		return false;
	}

	return Person::personHasExam(p, examID);
}