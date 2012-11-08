CC = g++
CFLAGS = -g -Wall -Wextra -c
SCIP_LFLAGS = -L/usr/lib -lscip -lreadline -lgmp -lz -lsoplex -llpispx -lobjscip -lnlpi.cppad 
PQXX_LFLAGS = -lpqxx

TESTOPT_OBJ = testOpt.cpp optimizer.o
TESTDB_OBJ = testDB.cpp dbreader.o

test: testOpt testDB

testOpt: $(TESTOPT_OBJ)
	$(CC) $(SCIP_INCLUDE) $(TESTOPT_OBJ)  $(SCIP_LFLAGS) -o testOpt

testDB: $(TESTDB_OBJ)
	$(CC) $(SCIP_INCLUDE) $(TESTDB_OBJ) -o testDB $(PQXX_LFLAGS) 

optimizer.o: optimizer.cpp optimizer.h
	$(CC) $(SCIP_INCLUDE) optimizer.cpp optimizer.h $(CFLAGS)

dbreader.o: dbreader.cpp dbreader.h
	$(CC) $(SCIP_INCLUDE) dbreader.cpp dbreader.h $(CFLAGS)