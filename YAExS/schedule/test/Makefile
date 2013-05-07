# we need -std=c++0x for unordered_map (used in optimizer)
CC = g++ -std=c++0x 
INCLUDE = -I../exams -I../people -I../time -I../locations -I..
CFLAGS = $(INCLUDE) -g -Wall -Wextra
SCIP_LFLAGS =  -L/usr/lib  -lscip -lreadline  -lgmp -lz  -llpispx  -lsoplex -lobjscip -lnlpi.cppad

# if you change anything in these files, it will re-make I think
OTHER_FILES = ../people/person.h ../people/student.h ../people/instructor.h ../exams/exam.h ../exams/exam.cpp getsome.h ../time/day.h ../time/timeslot.h

TESTOPT_OBJ = ../testOpt.cpp ../exams/exam.cpp optimizer.o

test: testOpt

testOpt: $(TESTOPT_OBJ)
	$(CC) $(CFLAGS) $(SCIP_INCLUDE) $(TESTOPT_OBJ)  $(SCIP_LFLAGS) -o testOpt

optimizer.o: ../optimizer.cpp ../optimizer.h $(OTHER_FILES)
	$(CC) $(CFLAGS) $(SCIP_INCLUDE) ../optimizer.cpp ../optimizer.h 

