/*
  -----Scheduler Class Implementation-----
  A class to act as a mediator betweeen an Optimizer and a DBReader.
*/

#include <algorithm>

#include "scheduler.h"


//TESTING
#include <random>
#include <chrono>
#define TIMESEED std::chrono::system_clock::now().time_since_epoch().count()

//Constructors
Scheduler::Scheduler(DBReader* dbIn, Optimizer* optIn) : db_(dbIn), optimizer_(optIn)
{}

//Destructor, cleans up People vector
static bool deleteAll( Person * p ) { delete p; return true; }

Scheduler::~Scheduler()
{
            std::remove_if( people_.begin(), people_.end(), deleteAll );
}

//Load classes which have exams into the vector
//Returns true as long as any exams were put in the vector
bool Scheduler::loadExams()
{
        //Clear any old data
        exams_.clear();
        
        //Get the list of exam ID's marked as 'hasexam'
        pqxx::result dbresult = db_->execute("SELECT DISTINCT \"examID\" FROM courses_course, courses_exammapping, courses_section WHERE courses_course.id = courses_section.course_id AND courses_section.crn = courses_exammapping.crn AND courses_course.status='hasexam'");

        //Check size
        if (dbresult.size() == 0) return false;
  
        //Store them
        for (size_t i = 0; i < dbresult.size(); i++)
        {
                exams_.push_back(Exam(0,dbresult[i][0].c_str()));
        }
  
        return true;
}

//Loads students from the database
//For now, they are just randomly generated from the exams
//
bool Scheduler::loadStudents()
{
        //Set up random number generation
        std::default_random_engine gen(TIMESEED);
        std::uniform_int_distribution<int> rand_exam(0,exams_.size()-1);
        std::uniform_int_distribution<int> rand_courseload(1,5);

        //Create 30 students
        std::remove_if( people_.begin(), people_.end(), deleteAll );
        people_.clear();
        
        for (int i = 0; i < 30; i++)
        {
                //Create a vector of exams for this person
                std::vector<Exam> exams;
                int num_exams = rand_courseload(gen);
                for (int j = 0; j < num_exams; j++)
                {
                        exams.push_back(Exam(1, exams_[rand_exam(gen)].getId()));
                }

                char id[3];
                id[0] = i/10 + '0';
                id[1] = i%10 + '0';
                id[2] = '\0';
                Student* add = new Student(id, exams);
                people_.push_back(add);
        }

        return true;
}

//Loads the exams and students into SCIP and begins the scheduling
bool Scheduler::startScheduling()
{
        //Load the model
        optimizer_->loadModel(exams_, people_, 5, 4);

        //Run it
        optimizer_->schedule();

        return true;
}

//Prints the schedule
void Scheduler::printSchedule()
{
        //Print output
        optimizer_->printSolutionAndNonzeroValues();
        std::cout << "\n_________________________\n" << std::endl;
        optimizer_->printExamSchedule();
        std::cout << "\n_________________________\n" << std::endl;
}
