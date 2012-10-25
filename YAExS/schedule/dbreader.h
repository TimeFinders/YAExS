/*
  -----Database Reader Class Declaration-----
  A class to manage the connection to the database for class and student info.
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
  DBReader(std::string settings);

  //Destructor
  ~DBReader();

  //General use functions
  //Should use specialized versions instead depending on what exactly SCIP needs
  result execute(std::string command);

 private:
  //Objects for the database connection itself
  pqxx::connection connect_;
}

#endif
