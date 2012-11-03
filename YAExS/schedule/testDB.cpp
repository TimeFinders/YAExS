/*
  -----PQXX Database Test-----
  Quick test main to ensure complation and linking work.
*/

#include <iostream>

#include "dbreader.h"

int main(int argc, char* argv[])
{
  //Create a DBReader
  DBReader dbReader;

  //Try to run "SELECT 1" on the nonexistent database
  pqxx::result result = dbReader.execute("SELECT 1");

  std::cout << result[0][0].c_str() << std::endl;
}
