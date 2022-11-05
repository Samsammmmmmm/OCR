# include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sudokuSolver.h"
#include "load_and_save.h"
int main(int argc, char** argv)
{
    if (argc != 2)
    {
        return 0;
    }
    char grid[9][9] = {{0}}; // initialize the grid we are going to solve
    load_txtfile(argv[1], grid);
    //call the solver function on the grid
    solver(grid);
    //open a new file and save the grid in it
    save_fileresult(argv[1], grid);
    return 0;
}
