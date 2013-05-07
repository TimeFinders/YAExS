/*
  Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
  All rights reserved.
  
  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
  Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  
  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  
  The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
  
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
  -----Database Manager Class Implementation-----
  A class to manage the connection to the database for course, exam and student info.
*/

#include "dbmanager.h"

//Default constructor, does not set up a connection
DBManager::DBManager() {}

//Constructor that takes a string of database settings to connect
DBManager::DBManager(const std::string& settings) : connect_(settings) {}

//Destructor, ensures that the connection is closed
DBManager::~DBManager()
{
  connect_.disconnect();
}

//Executes the SQL query passed in as a string and returns the result
pqxx::result DBManager::execute(const std::string& query)
{
  //Create some work on the database
  pqxx::work w(connect_);

  //Return the result of the query
  pqxx::result ret =  w.exec(query);
  w.commit();
  return ret;
}

//Deactiavtes the database connection
void DBManager::deactivate()
{
  connect_.deactivate();
}

//Reactiavtes the database connection after deactivation
void DBManager::reactivate()
{
  connect_.activate();
}
