/*
  -----Optimizer Test Main-----
*/

#include <iostream>
#include <objscip/objscip.h>
#include <objscip/objscipdefplugins.h>
#include "scip_exception.hpp"
#include <string>
#include <vector>
#include <list>
#include "exam.h"
#include "getsome.h"
#include "timeslot.h"

using namespace std;

int main()
{
	const int numExams = 4;
	const int numSlots = 2;

	list<Exam> exams;
	exams = GetSome::getFourExams();

	unsigned nExams = exams.size();
	if (nExams != numExams)
		cerr << "n exams is not " << numExams <<  std::endl;

	
	vector<TimeSlot> tslot;
	tslot.push_back(TimeSlot(1));
	tslot.push_back(TimeSlot(2));
	
	unsigned nSlots = tslot.size();
	if (nSlots != numSlots)
		cerr << "n slots is not " << numSlots <<  std::endl;
	
	SCIP * scip;
	

	try
	{
		// create scip instance
		SCIP_CALL_EXC(SCIPcreate( & scip));
		// default output and other plugins
		SCIP_CALL_EXC(SCIPincludeDefaultPlugins(scip));

		// begin the problem with the name, default sense is minimization 
		// as we would like
		SCIP_CALL_EXC(SCIPcreateProb(scip, "testProblem", NULL, NULL, NULL,
			 NULL, NULL, NULL, NULL));

		// add binary examIsAt variables
		
		double lBound = 0.0;
		double uBound = 1.0;
		
		// coefficient in the objective 
		// CHANGE LATER TO DEPEND ON TIME RESTRICTIONS/PREFERENCES
		double objectiveFunctionValue = 0.0;

		bool isInitial = TRUE;
		bool canRemoveInAging = FALSE;
			
		SCIP_VAR * examIsAt[numExams][numSlots];
		
		string names[numExams][numSlots];
		names[0][0] = "examIsAt_0_0";
		names[0][1] = "examIsAt_0_1";
		names[1][0] = "examIsAt_1_0";
		names[1][1] = "examIsAt_1_1";
		names[2][0] = "examIsAt_2_0";
		names[2][1] = "examIsAt_2_1";
		names[3][0] = "examIsAt_3_0";
		names[3][1] = "examIsAt_3_1";
		
		for (int exam = 0; exam < numExams; exam++)
		{
			for (int ts = 0; ts < numSlots; ts++)
			{
				SCIP_VAR* var;

				std::string vString = names[exam][ts];
				const char * vName = vString.c_str();
			
				SCIP_CALL_EXC(SCIPcreateVar(scip, & var, vName, lBound, uBound, 
					objectiveFunctionValue, SCIP_VARTYPE_BINARY, 
					isInitial, canRemoveInAging,
					NULL, NULL, NULL, NULL, NULL));
			

			 	// storing the SCIP_VAR pointer for later access
		 		examIsAt[exam][ts] = var;

				// add the scip var pointer to the scip object
				SCIP_CALL_EXC(SCIPaddVar(scip, var));
			}
		}
	

		
		// CONSTRAINTS
		// each exam must use exactly one time slot
		//
		// subto time: 
		//	forall <e> in ALL_EXAMS do
		//		sum<t> in TSLOT : examIsAt [e,t] == 1;     
	


		SCIP_CONS* timeConstraint[numExams];

		// lower and upper limits are both one since sum needs to be exactly one
		double lowerLimit = 1.0;
		double upperLimit = 1.0;
		isInitial = TRUE;
		
		double coeff = 1.0;

		std::string timeConsNames[numExams];
		timeConsNames[0] = "exam1";
		timeConsNames[1] = "exam2";
		timeConsNames[2] = "exam3";
		timeConsNames[3] = "exam4";
	
		// these constraints are not quite right yet
		for (int exam = 0; exam < numExams; exam++)
		{
			std::string vString = timeConsNames[exam];
			const char * vName = vString.c_str();

			SCIP_CALL_EXC(SCIPcreateConsLinear(scip, & timeConstraint[exam], vName,
				0, NULL, NULL, lowerLimit, upperLimit, isInitial,
				TRUE, TRUE, TRUE, TRUE, FALSE,
				FALSE, FALSE, FALSE, FALSE));

			// add variables to the constraint
			for (int ts = 0; ts < numSlots; ts++)
			{
				SCIP_CALL_EXC(SCIPaddCoefLinear(scip, timeConstraint[exam], examIsAt[exam][ts], coeff));
			}
		}


		// EXTRA STUFF TO CHECK
		SCIP_VAR * extraVar;
		std::string ExtravString = "extra";
		const char * ExtravName = ExtravString.c_str();
		SCIP_CALL_EXC(SCIPcreateVar(scip, & extraVar, ExtravName, 0.0, 1.0, 
					-10.0, SCIP_VARTYPE_BINARY, 
					isInitial, canRemoveInAging,
					NULL, NULL, NULL, NULL, NULL));
		// dont forget to add variable to scip object or you will get segfaults later!
		SCIP_CALL_EXC(SCIPaddVar(scip, extraVar));
		
		SCIP_CONS * extraCon;
		SCIP_CALL_EXC(SCIPcreateConsLinear(scip, & extraCon, "extraCon",
				0, NULL, NULL, 0.0, 1.0, isInitial,
				TRUE, TRUE, TRUE, TRUE, FALSE,
				FALSE, FALSE, FALSE, FALSE));
		SCIP_CALL_EXC(SCIPaddCoefLinear(scip, extraCon, extraVar, 1.0));
			
		//solve the problem!		
		SCIP_CALL_EXC(SCIPsolve(scip));

		SCIP_SOL* sol = SCIPgetBestSol(scip);

		SCIP_VAR * variable;

		if (sol == NULL)
		{
			cerr << "no solution!" << endl;
		}
		else 
		{
			std::cout << "Solution found!" << std::endl;
			for (unsigned exam = 0; exam < numExams; exam++)
			{
				for (unsigned ts = 0; ts < numSlots; ts++)
				{
					variable =  examIsAt[exam][ts];
					
					std::cout << "\t" << names[exam][ts] << " : ";
					std::cout << SCIPgetSolVal(scip, sol, variable) << std::endl;
				}
			}
			
			std::cout << "\t" << "extra variable : " << SCIPgetSolVal(scip, sol, extraVar) << std::endl; 
			
		}

		// Close things up
		for (int exam = 0; exam < numExams; exam ++)
		{
			for (int ts = 0; ts < numSlots; ts ++)
			{
				//std::cout << "release variable for exam " << exam << " and time " << ts << std::endl;
				SCIP_CALL_EXC(SCIPreleaseVar(scip, & examIsAt[exam][ts])) ;
			}
			std::cout << "NOT WORKING: release constraint for exam " << exam << std::endl;
			//SCIP_CALL_EXC(SCIPreleaseCons(scip, & timeConstraint[exam]));

		}
		
		//std::cout << " releasing extra variable" << std::endl;
		SCIP_CALL_EXC(SCIPreleaseVar(scip, & extraVar));
		std::cout << "NOT WORKING releasing extra constraint" << std::endl;
		//SCIP_CALL_EXC(SCIPreleaseCons(scip, & extraCon));

		//Then we close the SCIP environment:
		SCIP_CALL_EXC(SCIPfree(& scip));

	} catch(SCIPException & exec)
	{
		cerr<<"oops"<<endl;
		exit(exec.getRetcode());
	} 
	std::cout << "success!" << endl;
}
