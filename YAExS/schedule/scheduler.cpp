/*
  -----Scheduler Class Implementation-----
  A class to act as a mediator betweeen an Optimizer, a DBManager, 
  a LocationReader and a LocationAssigner
  Scheduler coordinates all scheduling activities. 
*/

#include <algorithm>
#include <fstream>
#include <map>
#include <sstream>

#include "scheduler.h"

//Constructors
Scheduler::Scheduler(DBManager* dbIn, Optimizer* optIn) : db_(dbIn), optimizer_(optIn)
{}

//Load classes which have exams into the vector
//Returns true as long as any exams were put in the vector
bool Scheduler::loadExams()
{
        //Clear any old data
        data_.clearExams();
        
        //Get the list of exam ID's marked as 'hasexam'
        pqxx::result dbresult = db_->execute("SELECT DISTINCT \"examID_id\" FROM courses_course, courses_exammapping, courses_section WHERE courses_course.id = courses_section.course_id AND courses_section.crn = courses_exammapping.crn AND courses_course.status='hasexam'");

        //Check size
        if (dbresult.size() == 0) return false;
  
        //Store them
        for (size_t i = 0; i < dbresult.size(); i++)
        {
            const char * examName = dbresult[i][0].c_str();
            data_.addExam( Exam(0, examName) );
        }
  
        return true;
}

//Adds an entry in the Registrations for the line
void Scheduler::parseLine(Registrations & reg, const std::string & line, std::map<CRN, Exam::EXAM_ID> & match)
{
        //Split up the string into Student ID and CRN
        std::string studentID, crn;
        size_t comma = line.find(',');
        studentID = line.substr(0, comma);
        crn = line.substr(comma+1, line.size()-1);

        //Add it if it's not already there
        for (size_t i = 0; i < reg[studentID].size(); i++)
        {
                if (reg[studentID][i] == match[crn]) return;
        }
        reg[studentID].push_back(match[crn]);
}

//Returns how many students are enrolled in the given exam
int Scheduler::studentsInExam(const Registrations & reg, const Exam::EXAM_ID & examID)
{
        int count = 0;
        for (Registrations::const_iterator i = reg.begin(); i != reg.end(); i++)
        {
                const std::vector<Exam::EXAM_ID> examIds = i->second;
                if (std::find(examIds.begin(), examIds.end(), examID) != examIds.end()) count++;
        }

        return count;
}

//Converts a vector of Exam ID's into a list of Exams
std::list<Exam> Scheduler::convertToExam(const Registrations & reg, const std::vector<Exam::EXAM_ID> & input)
{
        //Create the output list
        std::list<Exam> out;
        
        //Cycle over each entry
        for (size_t i = 0; i < input.size(); i++)
        {
                if (input[i] == "") continue;
                out.push_back(Exam(studentsInExam(reg, input[i]), input[i]));
        }

        return out;
}

//Updates the number of students in each exam in data_
void Scheduler::updateNumStudents(const Registrations & reg)
{
        for (std::list<Exam>::const_iterator i = data_.exams().begin(); i != data_.exams().end(); i++)
        {
                Exam::EXAM_ID examId = i->getId();
                data_.updateNumStudents(examId, studentsInExam(reg, examId));
                
                DEBUG_PRINT(examId << " has " << studentsInExam(reg, examId) << " students.");
        }
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
        //Should be Student ID -> vector<Exam ID>
        Registrations reg;

        //Get the conversions from crn to exam id
        pqxx::result dbresult = db_->execute("SELECT courses_section.crn, \"examID_id\" FROM courses_course, courses_exammapping, courses_section WHERE courses_course.id = courses_section.course_id AND courses_section.crn = courses_exammapping.crn AND courses_course.status='hasexam'");
        std::map<std::string, std::string> match;
        for (size_t i = 0; i < dbresult.size(); i++)
        {
                match[dbresult[i][0].c_str()] = dbresult[i][1].c_str();
        }

        //Read every line
        DEBUG_PRINT("Parsing lines");
        while (!fin.eof())
        {
                std::string line;
                fin >> line;
                parseLine(reg, line, match);
        }

        //Close the file stream
        fin.close();

        //Clear the people first
        data_.clearPeople();
        
        //Add each student to the ScheduleData
        DEBUG_PRINT("Adding students to data.");
        for (Registrations::iterator i = reg.begin(); i != reg.end(); i++)
        {
                Person::PERSON_ID personId = i->first;
                Student studentToAdd(personId, convertToExam(reg, i->second));
                data_.addPerson(&studentToAdd);
        }

        //Update the ScheduleData's list of exams with the proper number of students
        updateNumStudents(reg);

        //TEST: Print out all the information
        for(int i = 0; i < data_.numPeople(); i++)
        {
                //Get the exams
                Person * person = data_.people()[i];
                std::list<Exam> exams = person->getExams();

                //Print out information
                DEBUG_PRINT("Student ID: " << person->getId());

                for (std::list<Exam>::iterator it = exams.begin(); it != exams.end(); it++)
                {
                        DEBUG_PRINT("  " << it->getId());
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
bool Scheduler::startScheduling(int numExamDays, int numSlotsPerDay)
{
        //Load the model
        const std::vector<Person *> people = data_.people();
        std::list<Exam> * examListPointer = &data_.exams();
        optimizer_->loadModel(*examListPointer, people, numExamDays, numSlotsPerDay);

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

bool Scheduler::assignRooms()
{
        DEBUG_PRINT("Assigning rooms");
        int status = LocationAssigner::assignLocations(data_.exams(), locations_);

        return (status == 0);
}

void Scheduler::printRooms()
{
        std::cout << "\n_________________________\n" << std::endl;
        for (std::list<Exam>::iterator it = (data_.exams()).begin(); it != (data_.exams()).end(); it++)
        {
                std::cout << "exam: " << it->getId() << " with " << it->size() << " students";
                std::cout << " scheduled at time " << (it->getTime()).getId();

                if (it->hasLocation())
                {
                        std::cout << " in ";
                        ExamLocation * l = it->getLocation();
                        std::cout << l->getId();
                }
                else
                {
                        std::cout << " has no location assigned";
                }
                std::cout << std::endl;
        }
        std::cout << "\n_________________________\n" << std::endl;

}

void Scheduler::writeScheduleToDB()
{
        std::list<Exam> * examListPointer = &data_.exams();
        for (std::list<Exam>::iterator it = examListPointer->begin();
             it != examListPointer->end(); it++)
        {
                writeExamToDB(*it);
        }

}

void Scheduler::clearDBSchedule()
{
        pqxx::result dbresult = db_->execute("TRUNCATE TABLE \"Accounts_schedule\"");
}

void Scheduler::writeExamToDB( Exam & exam )
{
        Exam::EXAM_ID examID = exam.getId();
        TimeSlot::TIMESLOT_ID timeslotID = (exam.getTime()).getId();
        ExamLocation::LOCATION_ID locationID = (exam.getLocation())->getId();

        DEBUG_PRINT("inserting exam " << examID << " at time " << timeslotID << " in " << locationID << " into the database");

        std::string dbCall;
        std::ostringstream callStream;
    
        callStream << "INSERT INTO \"Accounts_schedule\" (section_id, timeslot, location) ";
        callStream << "VALUES (";
        callStream << examID;
        callStream << ", ";
        callStream << timeslotID;
        callStream << ", ";
        callStream << "'";
        callStream << locationID;
        callStream << "'";
        callStream << ")";
    
        dbCall = callStream.str();
        DEBUG_PRINT(dbCall);


        pqxx::result dbresult = db_->execute(dbCall);
}

//Deactivates the database in case of fork or other issues
void Scheduler::deactivateDB()
{
        db_->deactivate();
}

//reactivates the database after a deactivation
void Scheduler::reactivateDB()
{
        db_->reactivate();
}
