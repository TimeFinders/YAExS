CC = g++
CFLAGS = -g -Wall -Wextra -c
SCIP_LFLAGS = -lreadline -lgmp -lz -lzimpl -lsoplex -llpispx -lobjscip -lnlpi.cppad -lscip
PQXX_LFLAGS = -lpqxx

TESTOPT_OBJ = testOpt.cpp optimizer.o
TESTDB_OBJ = testDB.cpp dbreader.o

test: testOpt testDB

testOpt: $(TESTOPT_OBJ)
	$(CC) $(TESTOPT_OBJ) $(SCIP_LFLAGS) -o testOpt

testDB: $(TESTDB_OBJ)
	$(CC) $(TESTDB_OBJ) $(PQXX_LFLAGS) -o testDB

optimizer.o: optimizer.cpp optimizer.h
	$(CC) optimizer.cpp optimizer.h $(CFLAGS)

dbreader.o: dbreader.cpp dbreader.h
	$(CC) dbreader.cpp dbreader.h $(CFLAGS)