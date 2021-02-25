CFLAGS=-O --pedantic
.PHONY: all
all: sud 

.PHONY:clean
clean: 
	rm -f sud.o board.o sud testsud

sud: sud.c board.o
	gcc $(CFLAGS) sud.c board.o -o sud 

board.o: board.c board.h
	gcc $(CFLAGS) -c board.c 

test: testsud
	./testsud

testsud: testsud.c board.o
	gcc $(CFLAGS) testsud.c board.o -o testsud 

.PHONY: examples
examples:
	./sud example.txt
	./sud example2.txt
	./sud june62018.txt

.PHONY: doxygen
doxygen:
	doxygen
