# include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sudokuSolver.h"
//function which open the txt file and translate it in a char[][]
void load_txtfile(char file_name[], char grid[9][9])
{
    //open the txt file and get each number in the grid
    FILE*file;
    char c;
    file  = fopen(file_name, "r");
    size_t row = 0;
    size_t col = 0;
    do {
        c = fgetc(file);
        if (col == 9){
            row++;
            col = 0;
        }
        if (row == 9)
            break;
        if (c == '.')
            col ++;
        if (c >='1' && c <='9'){
            grid[row][col] = c-'0';
            col ++;
        }
    } while (c != EOF);
    fclose(file);
}

// function which open a new file with file name + .result and save the solved grid
void save_fileresult(char file_name[], char grid[9][9])
{
    //file pointer
    FILE*resultfile;
    // concat the file name with .result
    size_t s;
    for (s=0; file_name[s]!=0; s++);
    s+=7;
    char nameresult[s];
    for (size_t i = 0; i<= s; i++)
        nameresult[i] = file_name[i];
    strcat(nameresult, ".result");
    //open a new file with writing rights
    resultfile = fopen(nameresult,"w");
    for (size_t i = 0;i < 9; i++)
    {
        for (size_t j = 0; j<9; j++){
            if (j == 3 || j == 6)
                fprintf(resultfile, " ");
            fprintf(resultfile,"%d", grid[i][j]);
        }
        if (i == 2 || i == 5)
            fprintf(resultfile,"\n");
        fprintf(resultfile,"\n");
    }
    fclose(resultfile);
}
