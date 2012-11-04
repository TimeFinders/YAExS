/*
Test location assignment
*/

#include "locationassigner.h"
#include "room.h"
#include "exam.h"
#include "roomgroup.h"
#include <typeinfo>

//#include <list>


int overlaps(Room * r1, Room * r2) { return 1;};
int overlaps(Room * r, RoomGroup * rg) {return 2; };
int overlaps(RoomGroup * rg, Room * R) {return 3;} ;
int overlaps(RoomGroup * rg1, RoomGroup * rg2) {return 4; }; 

int overlaps(Room  r1, Room  r2) { return 5;};
int overlaps(RoomGroup rg, Room  R) {return 7;} ;
int overlaps(RoomGroup  rg1, RoomGroup  rg2) {return 8; };

bool overlaps(Room r, std::vector<Room> rooms) 
{
	for(unsigned i = 0; i < rooms.size(); i++)
	{
		if (r == rooms[i])
			return true;
	}
	return false;
}; 

bool overlaps(Room r, RoomGroup rg) 
{
	std::vector<Room> rooms = rg.contains();
	for(unsigned i = 0; i < rooms.size(); i++)
	{
		if (r == rooms[i])
			return true;
	}
	return false;
};

bool overlaps(Room r, ExamLocation * el) 
{
	std::vector<Room> rooms = el->contains();
	for(unsigned i = 0; i < rooms.size(); i++)
	{
		if (r == rooms[i])
			return true;
	}
	return false;
};

bool static overlaps(ExamLocation * e1, ExamLocation * e2) 
		{
			std::vector<Room> rooms1 = e1->contains();
			std::vector<Room> rooms2 = e2->contains();
			for(unsigned i = 0; i < rooms1.size(); i++)
			{
				for (unsigned j = 0; j < rooms2.size(); j++)
				{
					if (rooms1[i] == rooms2[j])
						return true;
				}
			}
			return false;
		};


int main()
{
			

	// ROOMS
	std::list<ExamLocation*> locs;

	Room ae214("AmosEaton","214",162);
	Room * ae215 = new Room("AmosEaton", "215", 50);
	Room ae216("AmosEaton", "216", 40);

	Room ae214again("AmosEaton", "214", 100);
	std::cout << "Are ae214 and AE 216 the same? " << (ae214 == ae216) << std::endl;
	std::cout << "Are ae214 and ae214again the same? " << (ae214 == ae214again) << std::endl;

	locs.push_back(&ae214);
	locs.push_back(ae215);	
	locs.push_back(&ae216);
	
	ExamLocation * West = new Room("West", "Auditorium", 250);
	locs.push_back(West);


	std::vector<Room> ae2all;
	ae2all.push_back(ae214);
	ae2all.push_back(*ae215);
	ae2all.push_back( ae216) ;
	
	std::cout << "Do ae214 and ae2all overlap? " << overlaps(ae214, ae2all) << std::endl;

	RoomGroup ae2All(ae2all);
	locs.push_back(&ae2All);

	std::cout << "Do ae214 and ae2All room group overlap? " << overlaps(ae214, ae2All) << std::endl;

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

	std::cout << "Do VCCNorth and VCC room group overlap? " << overlaps(*VCCNorth, VCC) << std::endl;
	std::cout << "Do West room group and VCC room group overlap? " << overlaps(West, VCC) << std::endl;

	/*
	bool overlaps(Room * r1, Room * r2) { return true;};
	bool overlaps(Room * r, RoomGroup * rg) {return true; };
	bool overlaps(RoomGroup rg, Room * R) {return true;} ;
	bool overlaps(RoomGroup * rg1, RoomGroup * rg2) {return true; }; 
	*/
	//std::cout << "Do Room * VCCNorth and Room * VCCSouth overlap?: " << overlaps(VCCNorth, *VCCSouth) << std::endl;
	
	// get an error if try VCC without a cast to (Room*)
	// no known conversion from type ExamLocation* to Room*

	std::cout << " *(Room* VCCNorth) and *(Room* VCCSouth) ?: ";
	std::cout << overlaps(*VCCNorth, *VCCSouth) << std::endl;
	// VCC is a RoomGroup so casting it to a Room* is bad!
	std::cout << overlaps(*VCCNorth, *((Room*) VCC) ) << std::endl;
	std::cout << overlaps(*VCCNorth, *((RoomGroup*) VCC) ) << std::endl;

	// EXAMS
	std::list<Exam> exams;

	Exam cla(25);
	cla.assignTime(1);
	exams.push_front(cla);

	Exam sdd(80);
	sdd.assignTime(2);
	exams.push_front(sdd);

	Exam bio(100);
	bio.assignTime(1);
	exams.push_front(bio);

	Exam ccn(15);
	ccn.assignTime(1);
	exams.push_front(ccn);

	Exam ml(55);
	ml.assignTime(2);
	exams.push_front(ml);



	int status = LocationAssigner::assignLocations(exams, locs);
	std::cout << "location assigner ended with status " << status << std::endl;
	

	return 0;
}
