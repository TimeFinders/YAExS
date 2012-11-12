/*
  -----PQXX Database Test-----
  Quick test main to ensure complation and linking work.
*/

#include <iostream>

#include "dbreader.h"

int main(int argc, char* argv[])
{
  //Create a DBReader
  DBReader dbReader("user=andrew password=andrew host=ec2-23-20-128-235.compute-1.amazonaws.com dbname=django port=5432");

  //Try to run "SELECT 1" on the database
  pqxx::result result = dbReader.execute("SELECT * FROM courses_course WHERE status = 'hasexam'");

  for (size_t i = 0; i < result.size(); i++)
    {
      for (size_t j = 0; j < result.columns(); j++)
        {
          std::cout << result[i][j].c_str() << " ";
        }
      std::cout << std::endl;
    }
}
