/*
  Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
  All rights reserved.
  
  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
  Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  
  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  
  The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
  
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
  ----- Exam Location Class Declaration-----
  An abstract class that represents a location that can 
  hold an exam. This could be a single room or a group 
  of rooms. 
*/

#ifndef _examlocation_h_
#define _examlocation_h_

#include<vector>

// for debugging
#include <iostream>
#include <string>
#include <string>

#ifndef DEBUG_PRINT
#define DEBUG_PRINT(x)
#ifdef debugMode
#undef DEBUG_PRINT
#define DEBUG_PRINT(x) std::cout << x << std::endl;
#endif
#endif

class Exam;
class Room;

#define unassignedID "unassigned";

class ExamLocation 
{		
	public:
		typedef std::string LOCATION_ID;

		// Return true if the given exam can fit in this location, false otherwise.
		virtual bool willExamFit(Exam exam) const = 0;

		static bool isLarger(ExamLocation * e1, ExamLocation * e2)
			{ return e1->getCapacity() > e2->getCapacity(); };

		// Returns a vector of the constituent Room(s)
		virtual std::vector<Room> contains() const = 0;

		//for debugging
		virtual void print() const = 0;
	
		virtual std::string getId() const = 0;
		
	protected:
		virtual int getCapacity() const = 0;
		
};
#endif
