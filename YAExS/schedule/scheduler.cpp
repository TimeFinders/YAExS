/*
  -----Scheduler Class Implementation-----
  A class to act as a mediator betweeen an Optimizer and a DBReader.
*/

#include "scheduler.h"

//Constructors
Scheduler::Scheduler(DBReader* dbIn, Optimizer* optIn) : db_(dbIn), optimizer_(optIn)
{}

//Load classes which have exams into the vector
//Returns true as long as any exams were put in the vector
bool Scheduler::loadExams()
{
        //Clear any old exams
        exams_.clear();
  
        //Get the list of exams marked as 'hasexam'
        pqxx::result dbresult = db_->execute("SELECT name FROM courses_course WHERE status = 'hasexam'");
  
        //Store them
        for (size_t i = 0; i < dbresult.size(); i++)
        {
                exams_.push_back(Exam(0,dbresult[i][0].c_str()));
        }
  
        return true;
}
