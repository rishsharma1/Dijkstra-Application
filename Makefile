# Makefile  
# Andrew Turpin
# March 2015

OBJ     = graph.o main.o set.o heap.o
SRC     = graph.c main.c set.c heap.c
EXE     = main

CC    = g++
CDEFS = -Wall # -O2 -m32

input:   $(OBJ) Makefile
	$(CC) $(CDEFS) -o $(EXE) $(OBJ)

clean:
	rm -f $(OBJ) $(EXE)

clobber: clean
	rm -f $(EXE)

usage: $(EXE)
	./$(EXE)

test: $(EXE)
	./$(EXE) 10
