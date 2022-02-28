#include "Functions.h"

int** malloc2D(int rows, int cols){
    int **tmp = malloc(rows * sizeof(*tmp));
    int i;
    for(i = 0; i < rows; i++){
        tmp[i] = calloc(cols, sizeof(*(tmp[i])));
    }
    return tmp;
}

void free2D(int** m, int rows){
    int i;
    for(i = 0; i < rows; i++){
        free(m[i]);
    }
    free(m);
}
