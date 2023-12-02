CC=gcc
CFLAGS=-I.

gen: gen.o
	$(CC) -o gen gen.o

gen.o: gen.c gen.h
	$(CC) -c gen.c $(CFLAGS)

run: gen
	./gen

clean:
	rm -f *.o gen
