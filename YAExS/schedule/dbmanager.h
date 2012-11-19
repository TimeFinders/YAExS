/*
  -----Database Manage Class Declaration-----
  A class to manage the connection to the database for course, exam and student info.
*/

#ifndef _dbmanager_h_
#define _dbmanager_h_

#include <pqxx/pqxx>

#include <string>

class DBManager
{
public:
    //Constructors
    DBManager();
    DBManager(const std::string& settings);

    //Destructor
    ~DBManager();

    //General use functions
    pqxx::result execute(const std::string& query);
    void deactivate();
    void reactivate();

private:
    //Objects for the database connection itself
    pqxx::connection connect_;
};

#endif
