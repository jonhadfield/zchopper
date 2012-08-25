P=chopper
OBJECTS=
CFLAGS=-static-libgcc -O2 -Wall --std=c99 -lmongoc
LDLIBS=
CC=gcc

all: 
	$(CC) $(CFLAGS) -o chopper src/chopper.c
clean:
	rm -f chopper
