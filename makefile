ARGS = -lm -Wall -pedantic
CC = gcc $(ARGS)
CMPNTS = main.o io.o drawer.o matrix.o parser.o

#all: main mk_s gen run
all: main run

main: $(CMPNTS)
	$(CC) -o main $(CMPNTS) $(ARGS)

#mk_s: mk_script.c
	#$(CC) -o mk_s mk_script.c
#gen:
	#./mk_s

main.o: main.c drawer.h io.h matrix.h parser.o

io.o: io.c io.h
drawer.o: drawer.c drawer.h
matrix.o: matrix.c matrix.h
parser.o: parser.c parser.h

.PHONY: all run clean gen

#requres imagemagick
run: main
	./main < script

clean:
	rm *.o
