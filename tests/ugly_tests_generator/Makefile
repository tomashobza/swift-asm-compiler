CC=gcc
CFLAGS=-I.

gen: gen.o
	$(CC) -o gen gen.o

gen.o: gen.c gen.h
	$(CC) -c gen.c $(CFLAGS)

run: gen
ifdef DEBUG
	./gen
else
	./gen 2>/dev/null
endif

clean:
	rm -f *.o gen
