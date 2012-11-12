# we need -std=c++0x for unordered_map (used in optimizer)
CC = g++ -std=c++0x 
CFLAGS = -g -Wall -Wextra -c
SCIP_LFLAGS =  -L/usr/lib  -lscip -lreadline  -lgmp -lz  -llpispx  -lsoplex -lobjscip -lnlpi.cppad

# if you change anything in these files, it will re-make
OTHER_FILES = person.h student.h instructor.h exam.h exam.cpp getsome.h day.h timeslot.h

TESTOPT_OBJ = testOpt.cpp exam.cpp optimizer.o

test: testOpt

testOpt: $(TESTOPT_OBJ)
	$(CC) $(SCIP_INCLUDE) $(TESTOPT_OBJ)  $(SCIP_LFLAGS) -o testOpt

optimizer.o: optimizer.cpp optimizer.h $(OTHER_FILES)
	$(CC) $(SCIP_INCLUDE) optimizer.cpp optimizer.h $(CFLAGS)

