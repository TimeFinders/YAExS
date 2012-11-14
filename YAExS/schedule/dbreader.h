/*
  -----Database Reader Class Declaration-----
  A class to manage the connection to the database for course, exam and student info.
*/

#ifndef _dbreader_h_
#define _dbreader_h_

#include <pqxx/pqxx>

#include <string>

class DBReader
{
public:
    //Constructors
    DBReader();
    DBReader(const std::string& settings);

    //Destructor
    ~DBReader();

    //General use functions
    pqxx::result execute(const std::string& query);

private:
    //Objects for the database connection itself
    pqxx::connection connect_;
};

#endif
