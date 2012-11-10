CC = g++
CFLAGS = -g -Wall -Wextra -c
SCIP_LFLAGS =  -L/usr/lib  -lscip -lreadline  -lgmp -lz  -llpispx  -lsoplex -lobjscip -lnlpi.cppad

TESTOPT_OBJ = testOpt.cpp exam.cpp optimizer.o

test: testOpt

testOpt: $(TESTOPT_OBJ)
	$(CC) $(SCIP_INCLUDE) $(TESTOPT_OBJ)  $(SCIP_LFLAGS) -o testOpt.out

optimizer.o: optimizer.cpp optimizer.h
	$(CC) $(SCIP_INCLUDE) optimizer.cpp optimizer.h $(CFLAGS)

