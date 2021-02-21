CC = gcc
CMPNTS = main.o io.o drawer.o

all: main run

main: $(CMPNTS)
	$(CC) -o main $(CMPNTS)


main.o: main.c drawer.h io.h
io.o: io.c io.h
drawer.o: drawer.c drawer.h

.PHONY: all run clean

#requres imagemagick
run: main
	./main
	convert test.ppm test.png
	rm test.ppm
	display test.png

clean:
	rm *.o
