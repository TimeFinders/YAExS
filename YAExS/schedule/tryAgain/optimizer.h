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
#include "day.h"

#include <objscip/objscip.h>
#include <objscip/objscipdefplugins.h>


class Optimizer
{
public:
    //Constructors
    Optimizer();

    Optimizer(bool enablePrinting);

    //Destructor
    ~Optimizer();

    //Setup function
    void loadModel(const std::vector<Exam> & exams, 
            const std::vector<Person*> & people,
            int numDays, int slotsPerDay);

    //Runs the solver
    void schedule();

    // print the solution and the values of the variables where they are nonzero
    void printSolutionAndNonzeroValues();

    //Returns the generated schedules
    std::string getBestSolution();
    //Currently not sure how to get the number of solutions...
    //std::vector<SCIP_SOL*> getSolutions();

private:
    // if true lots of debugging info will print to standard out
    bool shouldPrint_;

    //SCIP problem information
    SCIP* scip_;


    // VARIABLES
    // exam is at time variables
    std::unordered_map< Exam::EXAM_ID, std::unordered_map<TimeSlot::TIMESLOT_ID, SCIP_VAR *> > examIsAt_;

    // person has two or more exams in one day variables
    std::unordered_map <Person::PERSON_ID, SCIP_VAR * > twoPlus_;

    // person has three or more exams in one dat variables
    std::unordered_map <Person::PERSON_ID, SCIP_VAR * > threePlus_;

    // CONSTRAINTS
    // exam can meet at exactly one time constraint
    std::unordered_map< Exam::EXAM_ID, SCIP_CONS *> onceCon_;

    // set the two plus and three plus variables constraint
    std::unordered_map< Person::PERSON_ID, std::unordered_map<Day::DAY_ID, SCIP_CONS *> >overloadCon_;


    // OTHER DATA
    // holds the days you can schedule exams over. 
    std::vector<Day> days_;

    // keep track of pointers to people objects so we can figure out what exams they have
    std::unordered_map< Person::PERSON_ID, Person* > allPeople_;


    // LOADING SCIP INFO
    void loadExamIsAtVariables(const std::vector<Exam> & exams);
    void loadTwoPlusVariables(const std::vector<Person* > & people);
    void loadThreePlusVariables(const std::vector<Person* > & people);

    void loadOnceConstraints();
    void loadOverloadConstraints();


    // RELEASING SCIP INFO
    void releaseExamIsAtVariables();
    void releaseTwoPlusVariables();
    void releaseThreePlusVariables();

    void releaseOnceConstraints();
    void releaseOverloadConstraints();
    
    // NAME SCIP VARIABLES
    // used for naming variables and constraints for SCIP. const char *'s are needed for SCIP
    static const char* examAtVariableName(const Exam & exam, const TimeSlot & timeslot);
    static const char* onceConName( const Exam::EXAM_ID & eid);
    static const char* onceConName( const Exam & exam);
    static const char* twoPlusVariableName(Person * person);
    static const char* threePlusVariableName(Person * person);
    static const char* overloadConName( Person::PERSON_ID pID, const Day & day );

    // OTHER HELPER FUNCTIONS
    void loadDays( int numDays, int slotsPerDay);
    void loadAllPeople( const std::vector<Person*> & people);

    // true if the person must take the exam
    bool personHasExam(Person::PERSON_ID personID, Exam::EXAM_ID examID);

    // true if the day contains the time slot
    bool dayHasSlot(Day::DAY_ID dayID, TimeSlot::TIMESLOT_ID tsID);

};

#endif
