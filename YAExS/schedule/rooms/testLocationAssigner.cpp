/*
Test location assignment
*/

#include "locationassigner.h"
#include "room.h"
#include "roomgroup.h"

//#include <list>

int main()
{
	std::list<Exam> exams;

	Exam cla(25);
	cla.assignTime(1);
	exams.push_front(cla);

	Exam sdd(80);
	sdd.assignTime(2);
	exams.push_front(sdd);

	Exam bio(200);
	bio.assignTime(1);
	exams.push_front(bio);

	Exam ccn(15);
	ccn.assignTime(1);
	exams.push_front(ccn);

	Exam ml(55);
	ml.assignTime(2);
	exams.push_front(ml);



	std::list<ExamLocation*> locs;

	Room ae214("AmosEaton","214",162);
	Room * ae215 = new Room("AmosEaton", "215", 50);
	Room ae216("AmosEaton", "216", 40);

	locs.push_back(&ae214);
	locs.push_back(ae215);	
	locs.push_back(&ae216);
	
	ExamLocation * West = new Room("West", "Auditorium", 250);
	locs.push_back(West);

	std::vector<Room> ae2all;
	ae2all.push_back(ae214);
	ae2all.push_back(*ae215);
	ae2all.push_back( ae216) ;

	RoomGroup ae2All(ae2all);
	locs.push_back(&ae2All);

	std::vector<Room> ae214_215vec;
	ae214_215vec = ae2all;
	ae214_215vec.pop_back();
	
	RoomGroup * ae214_215 = new RoomGroup(ae214_215vec);
	locs.push_back(ae214_215);
	
	Room * VCCNorth = new Room("VCC", "North", 24);
	Room * VCCSouth = new Room("VCC", "South", 30);
	locs.push_back(VCCNorth);
	locs.push_back(VCCSouth);

	std::vector<Room> vcc;
	vcc.push_back(*VCCNorth);
	vcc.push_back(*VCCSouth);

	ExamLocation * VCC = new RoomGroup(vcc);
	locs.push_back(VCC);


	return 0;
}
