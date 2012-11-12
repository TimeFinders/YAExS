/*
  -----Optimizer Class Declaration-----
  A class to manage SCIP's variables, constraints, and solving for exam scheduling.
*/

#ifndef _optimizer_h_
#define _optimizer_h_

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include "exam.h"


#include <objscip/objscip.h>
#include <objscip/objscipdefplugins.h>
//#include <scip/reader_zpl.h>

class TimeSlot;

class Optimizer
{
public:
    //Constructors
    Optimizer();

    //Destructor
    ~Optimizer();

    //Setup function
    void loadModel(const std::vector<Exam> & exams, const std::vector<TimeSlot> & slots);

    //Runs the solver
    void schedule();

    // print the solution and the values of the variables
    void printSolutionAndValues();

    //Returns the generated schedules
    std::string getBestSolution();
    //Currently not sure how to get the number of solutions...
    //std::vector<SCIP_SOL*> getSolutions();

private:
    //SCIP problem information
    SCIP* scip_;

    // just for testing:
    SCIP_CONS * extraCon;

    // exam is at time variables
    std::unordered_map< Exam::EXAM_ID, std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR *> > examIsAt;

    // exam can meet at exactly one time constraint
    std::unordered_map< Exam::EXAM_ID, SCIP_CONS *> onceCon;

    void loadExamIsAtVariables(const std::vector<Exam> & exams, const std::vector<TimeSlot> & slots);

    void loadOnceConstraints(const std::vector<Exam> & exams, const std::vector<TimeSlot> & slots);

    static const char* examAtVariableName(const Exam & exam, const TimeSlot & timeslot);
};

#endif
