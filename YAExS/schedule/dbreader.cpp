/*
  -----Database Reader Class Implementation-----
  A class to manage the connection to the database for course, exam and student info.
*/

#include "dbreader.h"

//Default constructor, does not set up a connection
DBReader::DBReader() {}

//Constructor that takes a string of database settings to connect
DBReader::DBReader(const std::string& settings) : connect_(settings) {}

//Destructor, ensures that the connection is closed
DBReader::~DBReader()
{
  connect_.disconnect();
}

//Executes the SQL query passed in as a string and returns the result
pqxx::result DBReader::execute(const std::string& query)
{
  //Create some work on the database
  pqxx::work w(connect_);

  //Return the result of the query
  pqxx::result ret =  w.exec(query);
  w.commit();
  return ret;
}
