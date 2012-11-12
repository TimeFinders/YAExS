/*
  -----Optimizer Class Declaration-----
  A class to manage SCIP's variables, constraints, and solving for exam scheduling.
*/

#ifndef _optimizer_h_
#define _optimizer_h_

#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <iostream>
#include "exam.h"
#include "person.h"
#include "timeslot.h"


#include <objscip/objscip.h>
#include <objscip/objscipdefplugins.h>

//extern "C"
//{
//#include <scip/scip.h>
//}

//class Student;
//class Instructor;

class Optimizer
{
public:
    //Constructors
    Optimizer();

    //Destructor
    ~Optimizer();

    void loadModel( const std::vector<Exam> & exams  
                   // const std::vector<Person> & people,
                    //int numberOfDays, int numSlotsPerDay
                  );

    //Runs the solver
    void schedule();

    //Returns the generated schedules
    std::string getBestSolution();
    //Currently not sure how to get the number of solutions...
    //std::vector<SCIP_SOL*> getSolutions();

    // for debugging
    static const char * testExamVariableName( Exam & exam, TimeSlot & timeslot)
    {return examAtVariableName(exam, timeslot);};


private:
    //SCIP problem information
    SCIP* scip_;

    // SCIP variables
    std::unordered_map <Exam::EXAM_ID, std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR * > > examIsAt;
    //std::unordered_map <PERSON_ID, SCIP_VAR * > twoPlus;
    //std::unordered_map <PERSON_ID, SCIP_VAR * > threePlus;
    //std::unordered_map <PERSON_ID, std::unordered_map<TS_ID, SCIP_VAR * > > conflict;
    

    // SCIP constraints
    std::vector<SCIP_CONS *> onceCon; // each exam meets exactly once
   // std::vector<SCIP_CONS *> conflictCon; // set up conflict indicator variables
    // std::vector<SCIP_CONS *> overloadCon; // set up overload variables
    
    // In the future we can try to include time preferences and requirements
    //std::vector<SICP_CONS *> negativeTimeCon;
    //std::vector<SCIP_CONS *> postitiveTimeCon:

    void loadExamIsAtVariables(const std::vector<Exam> & exams, int numDays, int timeSlotsPerDay);
    //void loadTwoPlusVariables(const std::vector<PEOPLE> & people);
    //void loadThreePlusVariables(const std::vector<PEOPLE> & people);
    //void loadConflictVariables(const std::vector<PEOPLE> & people, int numDays, int timeSlotsPerDay);


    void loadOnceConstraints(const std::vector<Exam> & exams);
  //  void loadConflictConstraints(const std::vector<Person> & people);
    //void loadOverloadConstraints(const std::vector<Person> & people);

    static const char* examAtVariableName(Exam & exam, TimeSlot & timeslot);
};
#endif




