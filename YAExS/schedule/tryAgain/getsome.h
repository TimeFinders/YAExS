/*
Just a helper class for testing SCIP
*/

#include "exam.h"
#include "student.h"
#include <vector>

class Person;

class GetSome
{

public:


	// EXAMS
	static std::vector<Exam> getFourExams()
	{
		std::vector<Exam> exams;

		Exam cla(25, "CLA");
		cla.assignTime(1);
		exams.push_back(cla);

		Exam sdd(80, "SDD");
		sdd.assignTime(2);
		exams.push_back(sdd);

		Exam bio(126, "BIO");
		bio.assignTime(1);
		exams.push_back(bio);

		Exam ccn(15, "CCN");
		ccn.assignTime(1);
		exams.push_back(ccn);



		return exams;
	}

	static std::vector<Exam> getSomeExams()
	{
		return getFourExams();
	}

	static std::vector<TimeSlot> getSomeTimeSlots()
	{
		std::vector<TimeSlot> ts;
		ts.push_back(TimeSlot(1));
		ts.push_back(TimeSlot(2));
		return ts;
	}

	
	static Person * getVera()
	{
		Person * veraPtr = new Student("VERA");
		return veraPtr;
	}

	static std::vector<Person*> getSomePeople()
	{
   	 	std::vector<Person *> people;
    	Person * veraPtr = new Student("VERA");
    	people.push_back(veraPtr);

		return people;
	}
	

	static std::vector<Student> getSomeStudents()
	{
		std::vector<Student> students;
		Student s("Vera");
		students.push_back(s);
		return students;
	}
	//static std::vector<Instructor> getSomeInstructors();
};
