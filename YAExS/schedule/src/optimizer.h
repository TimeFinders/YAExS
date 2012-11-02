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

#include <scip/scip.h>
#include <scip/scipdefplugins.h>
#include <scip/reader_zpl.h>

class Optimizer
{
public:
    //Constructors
    Optimizer();

    //Destructor
    ~Optimizer();

    //Setup functions. Pretty much all placeholders while we figure out what's needed
    //void setupTimeSlots(int days, int slotsPerDay);
    //void addStudent(int id, std::vector<int> classid);
    //void addInstructor(int id, std::vector<int> classid);
    //void addClass(int id, int meetingTime);
    //void addPreference();
    void loadZIMPLModel(std::string filename);

    //Runs the solver
    void schedule();

    //Returns the generated schedules
    std::string getBestSolution();
    //Currently not sure how to get the number of solutions...
    //std::vector<SCIP_SOL*> getSolutions();

private:
    //SCIP problem information
    SCIP* scip_;
};

#endif
