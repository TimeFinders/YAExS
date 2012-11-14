CC = g++ -std=c++0x 
CFLAGS = -g -Wall -Wextra -c
SCIP_ZIMPL = 
SCIP_LFLAGS = -L/usr/lib -lscip -lreadline -lgmp $(SCIP_ZIMPL) -lz -lsoplex -llpispx -lobjscip -lnlpi.cppad
PQXX_LFLAGS = -lpqxx

TESTOPT_OBJ = testOpt.cpp optimizer.o exam.o
TESTDB_OBJ = testDB.cpp scheduler.o optimizer.o dbreader.o

test: testOpt testDB

testOpt: $(TESTOPT_OBJ)
	$(CC) $(TESTOPT_OBJ) $(SCIP_LFLAGS) -o testOpt

testDB: $(TESTDB_OBJ)
	$(CC) $(TESTDB_OBJ) -o testDB $(PQXX_LFLAGS) $(SCIP_LFLAGS)

scheduler.o: scheduler.cpp scheduler.h optimizer.o dbreader.o
	$(CC) scheduler.cpp $(CFLAGS)

optimizer.o: optimizer.cpp optimizer.h
	$(CC) optimizer.cpp $(CFLAGS)

dbreader.o: dbreader.cpp dbreader.h
	$(CC) dbreader.cpp $(CFLAGS)

exam.o: exams/exam.cpp exams/exam.h
	$(CC) exams/exam.cpp exams/exam.h $(CFLAGS)