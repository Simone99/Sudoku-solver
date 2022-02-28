#ifndef __FUNCTIONS__
#define __FUNCTIONS__
#define N 3
#define SUDOKU_DIM (N*N)

#include <stdlib.h>
#include <stdint.h>

int** malloc2D(int rows, int cols);
void free2D(int** m, int rows);

#endif