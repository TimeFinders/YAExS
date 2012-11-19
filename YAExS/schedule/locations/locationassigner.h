/*
  Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
  All rights reserved.
  
  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
  Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  
  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  
  The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
  
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
  ----- Location Assigner Class Declaration-----
  A class to assign locations to exams after they have been
  scheduled into time slots. Locations could be single rooms
  or a group of rooms.
*/

#ifndef _locationassigner_h_
#define _locationassigner_h_

#include <list>

#include "../exams/exam.h"
//#include "examlocation.h"
#include "room.h"

#ifndef DEBUG_PRINT
#define DEBUG_PRINT(x)
#ifdef debugMode
#undef DEBUG_PRINT
#define DEBUG_PRINT(x) std::cout << x << std::endl;
#endif
#endif

class LocationAssigner 
{
public:
	// Assign exam locations to exams that have had times assigned. 
	// After call, exam objects will have ExamLocation assigned.
	//Returns 0 if all goes well	
	static int assignLocations( std::list<Exam> & exams, std::list<ExamLocation *> examLocations);

private:
	// Return the "best" location for exam e from the available list.
	// Returns NULL if no location is suitable.
	static ExamLocation* bestLocation(Exam e, std::list<ExamLocation*> available);

	// remove from locList all locations that overlap with loc. E.g. if loc is AE214
	// and there is a RoomGroup[AE214, AE215, AE216] it will be removed.
	static std::list<ExamLocation *> removeOverlappingLocations( const ExamLocation* loc, const std::list<ExamLocation*>  & locList);


};

#endif
