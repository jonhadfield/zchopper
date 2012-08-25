OBJECTS=ext/mongo/lib/libmongoc.a
CFLAGS=-static-libgcc -O2 -Wall --std=c99 
LDFLAGS=
LDLIBS=
CC=gcc

all: 
	$(CC) -o chopper src/chopper.c $(CFLAGS) $(OBJECTS) 
clean:
	rm -f chopper
