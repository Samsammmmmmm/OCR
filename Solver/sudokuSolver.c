# include <stdio.h>
# include <stdlib.h>
#include <string.h>
#include "sudokuSolver.h"

// function checking if putting a number k would be valid
// grid[][]: the sudoku grid we are trying to solve
// int r: row position of the number we are testing
// int c: column position of the number we are testing
// char n: number we are testing (1-9)
int is_valid(char grid[9][9], int r, int c, char n)
{
    for (size_t i = 0; i<9; i++)
    {
        if (grid[r][i] == n) // check if n is in the ligne
            return 0;
        if (grid[i][c] == n) // check if n is in the colomn
            return 0;
    }
    size_t i = r-r%3;
    size_t j  = c-c%3;
    for (size_t k = i; k < i+3; k++) // check the case 3x3
    {
        for (size_t l = j; l < j+3; l++)
        {
            if (grid[k][l] == n)
                return 0;
        }
    }
    return 1;
}
// function which calls the rec solver
void solver(char grid[9][9])
{
    solver_rec(grid,0,0);
}
// recursive function which implement a backtracking algo
int solver_rec(char grid[9][9], int r, int c)
{
    if (r == 9) //stop case
        return 1;
    else if (c == 9) //au bout de la ligne passe a la suivante
        return solver_rec(grid, r+1, 0);
    else if (grid[r][c] != 0) //case where it is a already placed number
        return solver_rec(grid, r, c+1);
    else
    {
        for (char n = 1; n<10; n++)
        {
            if (is_valid(grid, r, c, n))
            {
                grid[r][c] = n;
                if (solver_rec(grid, r, c+1))
                    return 1;
                grid[r][c] = 0;
            }
        }
        return 0;
    }
}

// function print grid for tests
void print_grid(char grid[9][9])
{
    for (size_t i = 0; i<9; i++)
    {
        for (size_t j = 0; j<9; j++)
            printf("%d ", grid[i][j]);
        printf("\n");
    }
}

void solverM(char* path)
{
    char grid[9][9] = {{0}}; // initialize the grid we are going to solve
    load_txtfile(path, grid);
    //call the solver function on the grid
    solver(grid);
    //open a new file and save the grid in it
    save_fileresult(path, grid);
}
