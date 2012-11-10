/*
  -----Optimizer Test Main-----
*/

#include <iostream>
#include <objscip/objscip.h>
#include <objscip/objscipdefplugins.h>
#include "scip_exception.hpp"
#include <string>
#include <vector>

using namespace std;

int main()
{
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

		// add some binary variables
		
		double lBound = 0.0;
		double uBound = 1.0;
		
		// coefficient in the objective
		double objectiveFunctionValue = 1.0;
		bool isInitial = TRUE;
		bool canRemoveInAging = FALSE;
			
		SCIP_VAR * vars[4][2];
		
		string names[4][2];
		names[0][0] = "x_0_0";
		names[0][1] = "x_0_1";
		names[1][0] = "x_1_0";
		names[1][1] = "x_1_1";
		names[2][0] = "x_2_0";
		names[2][1] = "x_2_1";
		names[3][0] = "x_3_0";
		names[3][1] = "x_3_1";
		
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 2; j++)
			{
			SCIP_VAR* var;

			std::string vString = names[i][j];
			const char * vName = vString.c_str();
			
			SCIP_CALL_EXC(SCIPcreateVar(scip, & var, vName, lBound, uBound, 
				objectiveFunctionValue, SCIP_VARTYPE_BINARY, 
				isInitial, canRemoveInAging,
				NULL, NULL, NULL, NULL, NULL));
			

		 	// storing the SCIP_VAR pointer for later access
	 		vars[i][j] = var;

			// add the scip var pointer to the scip object
			SCIP_CALL_EXC(SCIPaddVar(scip, var));
			}
		}
	
		

	} catch(SCIPException & exec)
	{
		cerr<<"oops"<<endl;
		exit(exec.getRetcode());
	}
	std::cout << "success!" << endl;
}
