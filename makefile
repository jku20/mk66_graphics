CC = gcc
CMPNTS = main.o io.o drawer.o matrix.o

all: main #run

main: $(CMPNTS)
	$(CC) -o main $(CMPNTS)


main.o: main.c drawer.h io.h matrix.h

io.o: io.c io.h
drawer.o: drawer.c drawer.h
matrix.o: matrix.c matrix.h

.PHONY: all run clean

#requres imagemagick
run: main
	./main
	convert test.ppm test.png
	rm test.ppm
	display test.png

clean:
	rm *.o
