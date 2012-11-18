CC = g++ -std=c++0x 
CFLAGS = -g -Wall -Wextra -c
SCIP_ZIMPL = 
SCIP_LFLAGS = -L/usr/lib -lscip $(SCIP_ZIMPL) -lreadline  -lgmp -lz -llpispx  -lsoplex  -lobjscip -lnlpi.cppad
PQXX_LFLAGS = -lpqxx

TESTOPT_OBJ = testOpt.cpp locations/locationreader.cpp locations/locationassigner.cpp optimizer.o exam.o
TESTDB_OBJ = testDB.cpp locations/locationreader.cpp locations/locationassigner.cpp scheduler.o optimizer.o dbreader.o scheduleData.o exam.o

test: testOpt testDB

testOpt: $(TESTOPT_OBJ)
	$(CC) $(TESTOPT_OBJ) $(SCIP_LFLAGS) -o testOpt

testDB: $(TESTDB_OBJ)
	$(CC) $(TESTDB_OBJ) -o testDB $(PQXX_LFLAGS) $(SCIP_LFLAGS)

scheduler.o: scheduler.cpp scheduler.h optimizer.o dbreader.o
	$(CC) scheduler.cpp $(CFLAGS)

optimizer.o: optimizer.cpp optimizer.h
	$(CC) optimizer.cpp optimizer.h $(CFLAGS)

dbreader.o: dbreader.cpp dbreader.h
	$(CC) dbreader.cpp $(CFLAGS)

exam.o: exams/exam.cpp exams/exam.h
	$(CC)exams/exam.cpp exams/exam.h $(CFLAGS)

scheduleData.o: scheduleData/scheduleData.cpp scheduleData/scheduleData.h
	$(CC) scheduleData/scheduleData.cpp scheduleData/scheduleData.h $(CFLAGS)
