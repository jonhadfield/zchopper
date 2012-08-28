OBJECTS=
CFLAGS=-static-libgcc -O2 -Wall --std=c99
LDFLAGS=
LDLIBS=-Lext/mongo/lib -Iext/mongo/include -lz -lmongoc
CC=gcc

all: 
	$(CC) -o chopper src/chopper.c $(CFLAGS) $(OBJECTS) $(LDLIBS)
clean:
	rm -f chopper
