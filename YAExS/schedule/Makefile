CC = g++ -std=c++0x -g
CFLAGS = -Wall -Wextra -c
OUTPUT = -o main

SCIP_ZIMPL =
SCIP_LFLAGS = -L/usr/lib -lscip $(SCIP_ZIMPL) -lreadline  -lgmp -lz -llpispx  -lsoplex  -lobjscip -lnlpi.cppad
PQXX_LFLAGS = -lpqxx

MAIN_OBJ = main.cpp locations/locationreader.cpp locations/locationassigner.cpp scheduler.o optimizer.o dbmanager.o scheduleData.o exam.o
TESTOPT_OBJ = testOpt.cpp locations/locationreader.cpp locations/locationassigner.cpp optimizer.o exam.o
TESTDB_OBJ = testDB.cpp locations/locationreader.cpp locations/locationassigner.cpp scheduler.o optimizer.o dbmanager.o scheduleData.o exam.o

all: main test

main: $(MAIN_OBJ)
	$(CC) $(MAIN_OBJ) $(OUTPUT) $(PQXX_LFLAGS) $(SCIP_LFLAGS)

test: testOpt testDB

testOpt: $(TESTOPT_OBJ)
	$(CC) $(TESTOPT_OBJ) $(SCIP_LFLAGS) -o testOpt

testDB: $(TESTDB_OBJ)
	$(CC) $(TESTDB_OBJ) -o testDB $(PQXX_LFLAGS) $(SCIP_LFLAGS)

scheduler.o: scheduler.cpp scheduler.h optimizer.o dbmanager.o
	$(CC) scheduler.cpp $(CFLAGS)

optimizer.o: optimizer.cpp optimizer.h
	$(CC) optimizer.cpp optimizer.h $(CFLAGS)

dbmanager.o: dbmanager.cpp dbmanager.h
	$(CC) dbmanager.cpp $(CFLAGS)

exam.o: exams/exam.cpp exams/exam.h
	$(CC) exams/exam.cpp exams/exam.h $(CFLAGS)

scheduleData.o: scheduleData/scheduleData.cpp scheduleData/scheduleData.h
	$(CC) scheduleData/scheduleData.cpp scheduleData/scheduleData.h $(CFLAGS)

clean:
	rm -frv *.o *.gch testDB testOpt
