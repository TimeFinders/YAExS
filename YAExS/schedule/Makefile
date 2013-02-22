# Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
#
# The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

CC = g++ -std=c++0x
CFLAGS = -Wall -Wextra -c
OUTPUT = -o main

ifdef DEBUG
CFLAGS += -DdebugMode
CC += -g
else
CC += -O3
endif
SCIP_ZIMPL =
SCIP_LFLAGS = -L/usr/lib -lscip $(SCIP_ZIMPL) -lreadline  -lgmp -lz -llpispx  -lsoplex  -lobjscip -lnlpi.cppad -lzimpl
PQXX_LFLAGS = -lpqxx -L/usr/pgsql-9.2/lib -lpq

MAIN_OBJ = main.cpp locations/locationreader.cpp locations/locationassigner.cpp scheduler.o optimizer.o dbmanager.o scheduleData.o exam.o
TESTOPT_OBJ = testOpt.cpp locations/locationreader.cpp locations/locationassigner.cpp optimizer.o exam.o
TESTDB_OBJ = testDB.cpp locations/locationreader.cpp locations/locationassigner.cpp scheduler.o optimizer.o dbmanager.o scheduleData.o exam.o

all: main test

main: $(MAIN_OBJ)
	$(CC) $(MAIN_OBJ) $(OUTPUT) $(PQXX_LFLAGS) $(SCIP_LFLAGS)

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
	rm -frv *.o *.gch
