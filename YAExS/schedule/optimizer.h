/*
  -----Optimizer Class Declaration-----
  A class to manage SCIP's variables, constraints, and solving for exam scheduling.
*/

#ifndef _optimizer_h_
#define _optimizer_h_

#include <vector>

extern "C"
{
#include <scip/scip.h>
#include <scip/scipdefplugins.h>
}

class Optimizer
{
 public:
  //Constructors
  Optimizer();

  //Destructor
  ~Optimizer();

  //Setup functions. Pretty much all placeholders while we figure out what's needed
  void setupTimeSlots(int days, int slotsPerDay);
  void addStudent(int id, std::vector<int> classid);
  void addClass(int id, int meetingTime);

  //Runs the solver
  void schedule();

  //Returns the generated schedules
  std::vector<SCIP_SOL*> getSolutions();

 private:
  //SCIP problem information
  SCIP* scip_;

  //SCIP_VARs in whatever structures are needed
  std::vector<SCIP_VAR*> vars_;

  //SCIP_CONSs in whatever structures are needed
  std::vector<SCIP_CONS*> cons_;
};

#endif
