CC = g++
CFLAGS = -g -Wall -Wextra -c
LFLAGS = -lreadline -lgmp -lz -lzimpl -lsoplex -llpispx -lobjscip -lnlpi.cppad -lscip

OBJECTS = testOpt.cpp optimizer.o

testOpt: $(OBJECTS)
	$(CC) $(OBJECTS) $(LFLAGS) -o testOpt

optimizer.o: optimizer.cpp optimizer.h
	$(CC) optimizer.cpp optimizer.h $(CFLAGS)