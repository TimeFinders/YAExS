/*
  Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
  All rights reserved.
  
  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
  Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  
  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  
  The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
  
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
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
                std::cerr << "Usage: " << argv[0] << " days slots-per-day registrations-file pid-file" << std::endl;
                return 1;
        }
        
        //Parse inputs
        int examDays = atoi(argv[1]);
        // require at least 2 exam days because otherwise
        // the chance of infeasibility is huge.
        if (examDays < 2)
        {
            std::cerr << "Number of exam days must be at least two to avoid problems" << std::endl;
            return 1;
        }

        int slotsPerDay = atoi(argv[2]);
        if (slotsPerDay < 1)
        {
            std::cerr << "Number of exam slots per day must be at least one" << std::endl;
            return 1;
        }

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
        Optimizer opt;

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

                std::cout << "done scheduling, loading the locations..." << std::endl;

                //Load locations and assign rooms
                sched.loadLocations(settings["roomfile"], settings["roomgroupfile"]);

                 std::cout << "done loading locations, assign the locations..." << std::endl;

                sched.assignRooms();

                std::cout << "done assigning locations,  writing to database..." << std::endl;

                //Load results into database
                sched.writeScheduleToDB();

                //Delete the pid file to let the website know it's done
                remove(argv[4]);

                std::cout << "all done." << std::endl;
        }

        //Finish up
        return 0;
}
