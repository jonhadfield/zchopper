P=chopper
OBJECTS=
CFLAGS=-g -static-libgcc -Wall
LDLIBS=
CC=gcc

all: 
	$(CC) $(CFLAGS) -o chopper src/chopper.c
clean:
	rm -f chopper
