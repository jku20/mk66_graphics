CC = gcc
CMPNTS = main.o io.o

all: main

main: $(CMPNTS)
	$(CC) -o main $(CMPNTS)


main.o: main.c
io.o: io.c io.h

.PHONY: all run

run:
	./main

clean:
	rm *.o
