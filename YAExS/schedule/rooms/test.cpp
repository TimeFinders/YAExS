/*
  Test instructor, student, exam, exam location, room, and room group classes
*/


#include "instructor.h"
#include "student.h"
#include "room.h"
#include "roomgroup.h"

#include <iostream>
using namespace std;

int main() 
{
	std::cout << "I'm going to create 4 exams" << std::endl;

	int claStudents = 1;
	Exam cla(claStudents);
	std::cout << "Size of cla: " << cla.size() << std::endl;	

	Exam sdd(2);
	std::cout << "Size of sdd: " << sdd.size() << std::endl;
	Exam ccn(1);
	Exam bio(1);

	std::cout << "4 exams created" << std::endl;
	std::cout << "assigning time 'T1' to cla" << std::endl;
	cla.assignTime("T1");

	vector<Exam> johnExams;
	johnExams.push_back(sdd);
	johnExams.push_back(bio);

	Instructor john("john", johnExams);
	Instructor fengyan("fengyan", vector<Exam>(1, cla) );
	Instructor sikdar("sikdar", vector<Exam>(1,ccn) );

	std::cout << "3 instructors created" << std::endl;

	vector<Exam> andrewExams;
	andrewExams.push_back(ccn);
	andrewExams.push_back(sdd);

	Student andrew("andrew", andrewExams);

	vector<Exam> veraExams;
	veraExams.push_back(cla);
	veraExams.push_back(bio);
	veraExams.push_back(sdd);

	Student vera("vera", veraExams);
	
	std::cout << "two students created" << std::endl;


	Room ae214("AmosEaton", "214", 162);
	Room ae215("AmosEaton", "215", 50);
	Room ae216("AmosEaton", "216", 40);
	Room tiny("Fictional","t",3);
	
	std::cout << "three rooms created" << std::endl;	
	std::cout << "will CLA fit into room ae214?\t" << ae214.willExamFit(cla) << endl;
	std::cout << "will CLA fit into room tiny?\t" << tiny.willExamFit(cla) << endl;
	std::cout << "will SD&D fit into room tiny?\t" << tiny.willExamFit(sdd) << endl;

	ExamLocation * AE2 = &ae214;
	std::cout << "will SDD fit into exam location AE2 (room 214)?\t" << AE2->willExamFit(sdd) << endl;
	
	if (AE2->willExamFit(sdd))
	{	
		sdd.assignLocation(AE2);
	}
	std::cout << "Does sdd have an location? " << sdd.hasLocation() << std::endl;


	std::vector<Room> ae2rooms;
	ae2rooms.push_back(ae214);
	ae2rooms.push_back(ae215);
	ae2rooms.push_back(ae216);
	RoomGroup ae2(ae2rooms);

	std::cout << "one room group created" << std::endl;

	AE2 = &ae2;
	std::cout << "now changing ExamLocation AE2 to be RoomGroup ae2. Will SDD still fit? " << AE2->willExamFit(sdd) << endl;

	AE2 = &tiny;
	std::cout << "now changing ExamLocation AE2 to be tiny. Will SDD still fit? " << AE2->willExamFit(sdd) << endl;


	return 0;

}

