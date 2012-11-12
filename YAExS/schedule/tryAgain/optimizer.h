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
#include "person.h"

#include <objscip/objscip.h>
#include <objscip/objscipdefplugins.h>


class Optimizer
{
public:
    //Constructors
    Optimizer();

    //Destructor
    ~Optimizer();

    //Setup function
    void loadModel(const std::vector<Exam> & exams, 
            const std::vector<TimeSlot> & slots, 
            const std::vector<Person*> & people);

    //Runs the solver
    void schedule();

    // print the solution and the values of the variables where they are nonzero
    void printSolutionAndNonzeroValues();

    //Returns the generated schedules
    std::string getBestSolution();
    //Currently not sure how to get the number of solutions...
    //std::vector<SCIP_SOL*> getSolutions();

private:
    //SCIP problem information
    SCIP* scip_;

    // exam is at time variables
    std::unordered_map< Exam::EXAM_ID, std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR *> > examIsAt;

    // person has two or more exams in one day variables
    std::unordered_map <Person::PERSON_ID, SCIP_VAR * > twoPlus;

    // person has three or more exams in one dat variables
    //std::unordered_map <Person::PERSON_ID, SCIP_VAR * > threePlus;

    // exam can meet at exactly one time constraint
    std::unordered_map< Exam::EXAM_ID, SCIP_CONS *> onceCon;

    void loadExamIsAtVariables(const std::vector<Exam> & exams, const std::vector<TimeSlot> & slots);
    void loadTwoPlusVariables(const std::vector<Person* > & people);
    //void loadThreePlusVariables(const std::vector<Person* > & people);

    // doesn't need parameters because just uses examIsAtVariables
    void loadOnceConstraints();

    // used for naming variables and constraints. const char *'s are needed for SCIP
    static const char* examAtVariableName(const Exam & exam, const TimeSlot & timeslot);
    static const char* onceConName( const Exam::EXAM_ID & eid);
    static const char* onceConName( const Exam & exam);
    static const char* twoPlusVariableName(Person* person);
};

#endif
