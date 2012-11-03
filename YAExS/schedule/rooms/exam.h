/*
  ----- Exam Class Declaration-----
  A class that represents an exam.
*/

#ifndef _exam_h_
#define _exam_h_

class Exam
{
	private:
		int numStudents;	
		Room room;
		vector<Instructor> instructors;
		vector<Student> students;
		TimeSlot time;

	public:
		Exam();
		Exam(Instructor instructor);
		Exam(vector<Instructor> instructors);

		// the number of students taking this exam
		int size();

		void addStudent(Student student);
		void addStudents(Vector<Student> students);

		void assignTime(TimeSlot time);
		void assignRoom(Room room);
}