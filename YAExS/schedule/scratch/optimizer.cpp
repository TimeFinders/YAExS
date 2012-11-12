/*
  -----Optimizer Class Implementation-----
  A class to manage SCIP's variables, constraints, and solving for exam scheduling.
*/


#include <sstream>
#include "optimizer.h"

//Default constructor
Optimizer::Optimizer()
{
    //Initialize SCIP
   // SCIPcreate(&scip_);

    //Load default plugins
    //SCIPincludeDefaultPlugins(scip_);

    //Create a problem
    //SCIPcreateProb(scip_, "ExamScheduling",
     //              NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

//Destructor
Optimizer::~Optimizer()
{
    //Free the SCIP structure
   // SCIPfree(&scip_);
}

//Loads a variables and constraints into SCIP
void Optimizer::loadModel(const std::vector<Exam> & exams) 
                   // const std::vector<Student> & students, 
                    // const std::vector<Instructor> & instructors,
                    //int numberOfDays, int numSlotsPerDay)
{
    // VARIABLES
    
    std::unordered_map <Exam::EXAM_ID, std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR * > > examIsAt;


    // CONSTRAINTS
    loadOnceConstraints(exams);
}



const char* Optimizer::examAtVariableName(Exam & exam, TimeSlot & timeslot)
{
    // could change to read later: http://www.cplusplus.com/reference/iostream/istream/read/

   std::stringstream ss;
   ss << exam.getId() << "_" << timeslot.getId();
   std::string str(ss.str());
   const char* name = str.c_str();

   return name;
}

void Optimizer::loadExamIsAtVariables(const std::vector<Exam> & exams, int numDays, int timeSlotsPerDay)
{
    // create a vector of the time slots;
    std::vector<TimeSlot> slots;

    /*
    for (std::vector<Exam>::const_iterator examIt = exams.begin(); examIt != exams.end(); examIt++)
    {
        std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR * > variables; 

        // iterate over the time slots
        for (std::vector<TimeSlot>::iterator tsIt = slots.begin(); tsIt != slots.end(); tsIt++)
        {
            SCIP_VAR* var;
            Exam e = *examIt;
            TimeSlot s = *tsIt;
            const char * variableName = examAtVariableName(e, s);
            
            double lBound = 1.0;
            double uBound = 1.0;

            // this may need to change later based on time preferences/requirements:
            double objectiveCoef = 0.0;

            bool isInitial = TRUE;
            bool canRemoveInAging = FALSE;

            SCIP_CALL_EXC(SCIPcreateVar(scip_, & var, variableName, lBound, uBound, 
                objectiveCoef, SCIP_VARTYPE_BINARY, 
                isInitial, canRemoveInAging,
                NULL, NULL, NULL, NULL, NULL));
        

            // store the SCIP_VAR pointer for later access
            variables[tsIt->getId()] = var;

            // add the scip variable to the scip object
            SCIP_CALL_EXC(SCIPaddVar(scip_, var));

        }

        examIsAt[examIt->getId()] = variables;
    }
    */
}

void Optimizer::loadOnceConstraints(const std::vector<Exam> & exams)
{
    //check that variables have been loaded

    //create and add constraints
}

//Run the solver
void Optimizer::schedule()
{
    //SCIPsolve(scip_);
}

//Return the best solution as a string
std::string Optimizer::getBestSolution()
{
    //Create a stringstream to redirect cout to a string
    std::stringstream redir;

    //Store the old streambuf while linking cout to the stringstream
    std::streambuf* old = std::cout.rdbuf(redir.rdbuf());

    //Print the solution to the stringstream
   // SCIPprintBestSol(scip_, NULL, false);

    //Restore the old streambuf
    std::cout.rdbuf(old);

    //Return the string stored in redir
    return redir.str();
}
