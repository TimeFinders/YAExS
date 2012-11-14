/*
  -----Optimizer Class Declaration-----
  A class to manage SCIP's variables, constraints, and solving for exam scheduling.
*/

#ifndef _optimizer_h_
#define _optimizer_h_

#include <vector>
#include <list>
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

    //print each exam with its assigned time slot;
    void printExamSchedule();

    // print the SCIP solution information
    // and the values of the variables where they are nonzero
    void printSolutionAndNonzeroValues();


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

    // conflict variables
    std::unordered_map <Person::PERSON_ID, std::unordered_map <TimeSlot::TIMESLOT_ID, SCIP_VAR *> > conflictAt_;


    // CONSTRAINTS
    // exam can meet at exactly one time constraint
    std::unordered_map< Exam::EXAM_ID, SCIP_CONS *> onceCon_;

    // set the two plus and three plus variables constraint
    std::unordered_map< Person::PERSON_ID, std::unordered_map<Day::DAY_ID, SCIP_CONS *> >overloadCon_;

    // conflict constraints
    std::unordered_map <Person::PERSON_ID, std::unordered_map <TimeSlot::TIMESLOT_ID, SCIP_CONS *> > conflictCon_;


    // OTHER DATA
    // holds the days you can schedule exams over. 
    std::vector<Day> days_;

    // holds all the time slots over all the days
    std::vector<TimeSlot> allTimeSlots_;

    // keep track of pointers to people objects so we can figure out what exams they have
    std::unordered_map< Person::PERSON_ID, Person* > allPeople_;


    // LOADING SCIP INFO
    void loadExamIsAtVariables(const std::vector<Exam> & exams);
    void loadTwoPlusVariables(const std::vector<Person*> & people);
    void loadThreePlusVariables(const std::vector<Person*> & people);
    void loadConflictAtVariables(const std::vector<Person*> & people);

    void loadOnceConstraints();
    void loadOverloadConstraints();
    void loadConflictConstraints(); 
    // load the conflict constraints for a single person (one for each time slot)
    void loadConflictConstraints(Person * p);


    // RELEASING SCIP INFO
    void releaseExamIsAtVariables();
    void releaseTwoPlusVariables();
    void releaseThreePlusVariables();
    void releaseConflictAtVariables();

    void releaseOnceConstraints();
    void releaseOverloadConstraints();
    void releaseConflictConstraints();


    // PRINT VALUES OF (NONZERO) VARIABLES in a solution
    void printExamIsAtVariables(SCIP_SOL* sol);
    void printTwoPlusVariables(SCIP_SOL* sol);
    void printThreePlusVariables(SCIP_SOL* sol);
    void printConflictVariables(SCIP_SOL * sol);

    
    // NAME SCIP VARIABLES AND CONSTRAINTS
    // used for naming variables and constraints for SCIP. const char *'s are needed for SCIP
    static const char* examAtVariableName( const Exam & exam, const TimeSlot & timeslot );
    static const char* conflictAtVariableName( Person::PERSON_ID personID, TimeSlot::TIMESLOT_ID tsID );
    static const char* twoPlusVariableName( Person * person );
    static const char* threePlusVariableName( Person * person );
    static const char* onceConName( Exam::EXAM_ID eid );
    static const char* onceConName( const Exam & exam );
    static const char* overloadConName( Person::PERSON_ID personID, Day::DAY_ID dayID );
    static const char* conflictConName( Person::PERSON_ID personID, TimeSlot::TIMESLOT_ID tsID ); // WRITE


    // OTHER HELPER FUNCTIONS
    void loadDays( int numDays, int slotsPerDay);
    void loadAllPeople( const std::vector<Person*> & people);

    // true if the day contains the time slot
    bool dayHasSlot(Day::DAY_ID dayID, TimeSlot::TIMESLOT_ID tsID);

    // the exam is at variables for this person, keyed by their time slot id
    std::unordered_map<TimeSlot::TIMESLOT_ID, std::list<SCIP_VAR *> > personalExamIsAtVariables(Person * person);

    // create an overload constraint for an individual person on one day
    SCIP_CONS * createPersonalOverloadConstraint(Person::PERSON_ID personID, const Day & day);

};

#endif
