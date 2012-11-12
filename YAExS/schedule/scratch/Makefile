CC = g++ 
CFLAGS = -g -std=gnu++0x -Wall -Wextra
SCIP_LFLAGS =  -L/usr/lib  -lscip -lreadline  -lgmp -lz  -llpispx  -lsoplex -lobjscip -lnlpi.cppad

TESTOPT_OBJ = testOpt.cpp exam.cpp optimizer.o

test: testOpt

testOpt: $(TESTOPT_OBJ)
	$(CC) $(CFLAGS) $(SCIP_INCLUDE) $(TESTOPT_OBJ)  $(SCIP_LFLAGS) -o testOpt.out

optimizer.o:  optimizer.cpp optimizer.h
	$(CC) $(CFLAGS) $(SCIP_INCLUDE) testOpt.cpp exam.cpp optimizer.cpp optimizer.h

