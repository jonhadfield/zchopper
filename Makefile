P=chopper
OBJECTS=
CFLAGS=-g -O3 -Wall
LDLIBS=
CC=gcc

all: 
	$(CC) $(CFLAGS) -o chopper src/chopper.c
clean:
	rm -f chopper
