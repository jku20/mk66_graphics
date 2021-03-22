CC = gcc
CMPNTS = main.o io.o drawer.o matrix.o parser.o
ARGS = -lm -g

all: main run

main: $(CMPNTS)
	$(CC) -o main $(CMPNTS) $(ARGS)


main.o: main.c drawer.h io.h matrix.h parser.o

io.o: io.c io.h
drawer.o: drawer.c drawer.h
matrix.o: matrix.c matrix.h
parser.o: parser.c parser.h

.PHONY: all run clean

#requres imagemagick
run: main
	./main < script

clean:
	rm *.o
