PROG_NAME=zchopper
OBJECTS=flush.o zchopper.o validation.o
CFLAGS=-static-libgcc -O2 -Wall --std=c99 -pedantic
LDFLAGS=
LDLIBS=-Lext/mongo/lib -Iext/mongo/include -lz -lmongoc
CC=gcc -g

$(PROG_NAME) : $(OBJECTS)
	$(CC) $(OBJECTS) -o $(PROG_NAME) $(LDLIBS)

zchopper.o : src/zchopper.c src/zchopper.h 
	$(CC) $(CFLAGS) $(LDLIBS) -c ./src/zchopper.c 

flush.o : src/flush.c src/zchopper.h
	$(CC) $(CFLAGS) $(LDLIBS) -c ./src/flush.c 

validation.o : src/validation.c src/zchopper.h
	$(CC) $(CFLAGS) $(LDLIBS) -c ./src/validation.c 

clean:
	@- $(RM) $(PROG_NAME)
	@- $(RM) $(OBJECTS)
