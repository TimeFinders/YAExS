/*
  -----Scheduler Class Implementation-----
  A class to act as a mediator betweeen an Optimizer and a DBReader.
*/

#include <algorithm>

#include "scheduler.h"


//TESTING
#include <random>
#include <chrono>
#include <set>
#define TIMESEED std::chrono::system_clock::now().time_since_epoch().count()

//Constructors
Scheduler::Scheduler(DBReader* dbIn, Optimizer* optIn) : db_(dbIn), optimizer_(optIn)
{}

//Load classes which have exams into the vector
//Returns true as long as any exams were put in the vector
bool Scheduler::loadExams()
{
        //Clear any old data
        data_.clearExams();
        
        //Get the list of exam ID's marked as 'hasexam'
        pqxx::result dbresult = db_->execute("SELECT DISTINCT \"examID\" FROM courses_course, courses_exammapping, courses_section WHERE courses_course.id = courses_section.course_id AND courses_section.crn = courses_exammapping.crn AND courses_course.status='hasexam'");

        //Check size
        if (dbresult.size() == 0) return false;
  
        //Store them
        for (size_t i = 0; i < dbresult.size(); i++)
        {
                data_.addExam(Exam(0,dbresult[i][0].c_str()));
        }
  
        return true;
}

//Adds the exam in data_ at the given index to the inputted exams
//as long as it's not already there
void Scheduler::addExamIfUnique(std::vector<Exam>& exams, int index)
{
        bool skip = false;
        for (size_t j = 0; j < exams.size(); j++)
        {
                if (exams[j].getId() == data_.examID(index)) skip=true;
        }
        if (skip) return;
        exams.push_back(Exam(1, data_.examID(index)));
}

//Creates a random student, adding it to data
void Scheduler::createRandomStudent(int id)
{
        //Set up random number generation
        std::default_random_engine gen(TIMESEED);
        std::uniform_int_distribution<int> rand_exam(0,data_.numExams()-1);
        std::uniform_int_distribution<int> rand_courseload(0,data_.numExams()-1);

        //Create a vector of exams for this person
        std::vector<Exam> exams;
        size_t num_exams = rand_courseload(gen);

        while (exams.size() < num_exams)
        {
                int index = rand_exam(gen);
                addExamIfUnique(exams, index);
        }


        char idc[3];
        idc[0] = id/10 + '0';
        idc[1] = id%10 + '0';
        idc[2] = '\0';
        Student add(idc, exams);
        data_.addPerson(&add);
}

//Loads students from the database
//For now, they are just randomly generated from the exams
bool Scheduler::loadStudents()
{
        //Create 100 students
        data_.clearPeople();
        
        for (int i = 0; i < 60; i++)
        {
                createRandomStudent(i);
        }

        return true;
}

//Loads the exams and students into SCIP and begins the scheduling
bool Scheduler::startScheduling()
{
        int numDays = 3;
        int slotsPerDay = 3;

        //Load the model
        optimizer_->loadModel(data_.exams(), data_.people(), numDays, slotsPerDay);

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
