/*
  -----Scheduler Class Implementation-----
  A class to act as a mediator betweeen an Optimizer and a DBReader.
*/

#include <algorithm>
#include <fstream>
#include <map>

#include "scheduler.h"

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

//Adds an entry in the Registrations for the line
void Scheduler::parseLine(Registrations & reg, const std::string & line)
{
        //Split up the string into Student ID and CRN
        std::string studentID, crn;
        size_t comma = line.find(',');
        studentID = line.substr(0, comma);
        crn = line.substr(comma+1, line.size()-1);

        //Add it
        reg[studentID].push_back(crn);
}

//Returns how many students are enrolled in the given class
int Scheduler::studentsInClass(const Scheduler::Registrations & reg, const std::string & crn)
{
        int count = 0;
        for (Scheduler::Registrations::const_iterator i = reg.begin(); i != reg.end(); i++)
        {
                if (std::find(i->second.begin(), i->second.end(), crn) != i->second.end()) count++;
        }

        return count;
}

//Converts a vector of strings into a vector of Exams
//Queries the database to use the exam id's instead of CRN's and
//to reference exams
std::vector<Exam> Scheduler::convertToExam(const Scheduler::Registrations & reg, const std::vector<std::string> & input, std::map<std::string,std::string> & match)
{
        //Create the output vector
        std::vector<Exam> out;
        
        //Cycle over each entry
        for (size_t i = 0; i < input.size(); i++)
        {
                if (match[input[i]] == "") continue;
                out.push_back(Exam(studentsInClass(reg, input[i]), match[input[i]]));
        }

        return out;
}

//Loads students from a file
//Each row should be StudentID,CRN
//Returns true if everything worked okay
bool Scheduler::loadStudents(std::string filename)
{
        //Create a file stream
        std::ifstream fin(filename);

        //Make sure it opened
        if (!fin) return false;

        //Set up storage for information
        Scheduler::Registrations reg;

        //Read every line
        std::cout << "Parsing lines" << std::endl;
        while (!fin.eof())
        {
                std::string line;
                fin >> line;
                parseLine(reg, line);
        }

        //Get the conversions from crn to exam id
        pqxx::result dbresult = db_->execute("SELECT courses_section.crn, \"examID\" FROM courses_course, courses_exammapping, courses_section WHERE courses_course.id = courses_section.course_id AND courses_section.crn = courses_exammapping.crn AND courses_course.status='hasexam'");
        std::map<std::string, std::string> match;
        for (size_t i = 0; i < dbresult.size(); i++)
        {
                match[dbresult[i][0].c_str()] = dbresult[i][1].c_str();
        }

        //Clear the people first
        data_.clearPeople();
        
        //Add each student to the ScheduleData
        std::cout << "Adding students to data." << std::endl;
        for (Scheduler::Registrations::iterator i = reg.begin(); i != reg.end(); i++)
        {
                Student add(i->first, convertToExam(reg, i->second, match));
                data_.addPerson(&add);
        }

        //TEST: Print out all the information
        for(int i = 0; i < data_.numPeople(); i++)
        {
                //Get the exams
                std::vector<Exam> exams = data_.people()[i]->getExams();

                //Print out information
                std::cout << "Student ID: " << data_.people()[i]->getId() << std::endl;

                for (size_t j = 0; j < exams.size(); j++)
                {
                        std::cout << "  " << exams[j].getId() << std::endl;
                }
        }

        return true;
}

bool Scheduler::loadLocations(std::string roomFilePath, std::string roomGroupFilePath)
{
    if (!locations_.empty())
    {
        std::cerr << "Locations list is not empty. Only loadLocations once" << std::endl;
        return false;   
    }
    
   LocationReader lr;
   locations_  = lr.readLocations(roomFilePath, roomGroupFilePath);
   
    if(locations_.empty())
    {
        std::cerr << "no locations were added" << std::endl;
        return false;
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
