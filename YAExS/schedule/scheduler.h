/*
  -----Scheduler Class Declaration-----
  A class to act as a mediator betweeen an Optimizer and a DBReader.
*/

#ifndef _scheduler_h_
#define _scheduler_h_

#include "dbreader.h"
#include "optimizer.h"

class Scheduler
{
 public:
  //Constructors
  Scheduler();
  Scheduler(DBReader* dbIn, Optimizer* optIn);

  //Destructor
  ~Scheduler();

  //Setup functions
  bool loadClasses();
  bool loadStudents();

  //Scheduling functions
  bool startScheduling();
  std::string status();
  bool stopScheduling();
 private:
  //Pointer to a DBReader
  DBReader* db_;

  //Pointer to an Optimizer
  Optimizer* optimizer_;

  //Current state of the scheduling process
  std::string state_;
}

#endif
