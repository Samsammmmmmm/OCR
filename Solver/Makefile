# Makefile

all:
	gcc -Wall -Wextra   -c -o sudokuSolver.o sudokuSolver.c
	gcc -Wall -Wextra   -c -o load_and_save.o load_and_save.c
	gcc -Wall -Wextra   -c -o main.o main.c
	gcc   sudokuSolver.o load_and_save.o main.o  -o solver

clean :
	rm -f sudokuSolver.o main.o load_and_save.o
	rm -f solver *.result
#END
