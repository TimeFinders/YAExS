/*
  -----YAExS Main Program-----
*/

#include <map>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>

#include "scheduler.h"

int main(int argc, char* argv[])
{
        //Check arguments
        if (argc != 5)
        {
                std::cerr << "Usage: " << argv[0] << "days slots-per-day registrations-file pid-file" << std::endl;
                return 1;
        }

        //Parse inputs
        int examDays = atoi(argv[1]);
        int slotsPerDay = atoi(argv[2]);

        //Load configuration file
        std::map<std::string,std::string> settings;
        std::ifstream config("config.txt");
        if (!config)
        {
                std::cerr << "Error opening configuration file config.txt" << std::endl;
                return 1;
        }

        //Read the file
        while (!config.eof())
        {
                //Get the next line
                std::string line;
                getline(config, line);

                //Remove all spaces
                size_t space;
                space = line.find(' ');
                while (space != line.npos)
                {
                        line.erase(space,1);
                        space = line.find(' ');
                }

                //Skip comments
                if (line[0] == '#') continue;

                //Parse
                std::string set, value;
                size_t equals = line.find('=');
                settings[line.substr(0, equals)] = line.substr(equals+1, line.size()-1);
        }
        config.close();

        //Make sure some essential settings are defined
        if (settings.find("db_username") == settings.end())
        {
                std::cerr << "db_username must be defined." << std::endl;
                return 1;
        }
        if (settings.find("db_password") == settings.end())
        {
                std::cerr << "db_password must be defined." << std::endl;
                return 1;
        }
        if (settings.find("db_host") == settings.end())
        {
                std::cerr << "db_host must be defined." << std::endl;
                return 1;
        }
        if (settings.find("db_name") == settings.end())
        {
                std::cerr << "db_name must be defined." << std::endl;
                return 1;
        }
        if (settings.find("db_port") == settings.end())
        {
                std::cerr << "db_port must be defined." << std::endl;
                return 1;
        }
        if (settings.find("roomfile") == settings.end())
        {
                std::cerr << "roomfile must be defined." << std::endl;
                return 1;
        }
        if (settings.find("roomgroupfile") == settings.end())
        {
                std::cerr << "roomgroupfile must be defined." << std::endl;
                return 1;
        }
        
        //Create a DBManager and Optimizer
        std::string dbSetupString = "user=" + settings["db_username"];
        dbSetupString += " password=" + settings["db_password"];
        dbSetupString += " host=" + settings["db_host"];
        dbSetupString += " dbname=" + settings["db_name"];
        dbSetupString += " port=" + settings["db_port"];
        DBManager dbManager(dbSetupString);
        Optimizer opt(false);

        //Create a Scheduler
        Scheduler sched(&dbManager, &opt);

        //Truncate the output table in case something goes wrong partway through
        sched.clearDBSchedule();

        //Try to load exams and students
        sched.loadExams();
        if (!sched.loadStudents(argv[3]))
        {
                std::cerr << "Could not read " << argv[3] << std::endl;
                return 1;
        }

        //Deactivate the database before forking
        sched.deactivateDB();
        
        //Fork to let the parent return
        pid_t pid = fork();
        if (pid < 0) //Error
        {
                std::cerr << "Error forking." << std::endl;
                return 1;
        }
        else if (pid > 0) //Parent
        {
                //Parent needs to create a pid file before returning
                std::ofstream pidfile(argv[4], std::ios::out | std::ios::trunc);
                pidfile << pid;
                pidfile.close();
        }
        else //Child
        {
                //Reactivate the database connection
                sched.reactivateDB();
                
                //Run the scheduler
                sched.startScheduling(examDays, slotsPerDay);

                //Load locations and assign rooms
                sched.loadLocations(settings["roomfile"], settings["roomgroupfile"]);

                std::cout << "Load locations complete" << std::endl;
                
                sched.assignRooms();

                std::cout << "Assign rooms complete" << std::endl;

                //Load results into database
                sched.writeScheduleToDB();

                //Delete the pid file to let the website know it's done
                remove(argv[4]);

                std::cout << "Child done" << std::endl;
        }

        //Finish up
        return 0;
}
