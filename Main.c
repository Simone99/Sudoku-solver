#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#include "Functions.h"

#define GRAPH_FILE "graph.txt"

void sudokuToGraph(FILE *sudoku_file, FILE *graph_file);
int** readSudoku(FILE *sudoku_file);
void solveSudoku(G graph);

int main(){
    FILE *fp, *fp2;
    G graph;

    if((fp = fopen("sudoku.txt", "r")) == NULL || (fp2 = fopen(GRAPH_FILE, "w")) == NULL){
        fprintf(stderr, "Unable to open the file!");
        exit(EXIT_FAILURE);
    }

    sudokuToGraph(fp, fp2);

    fclose(fp);
    fclose(fp2);

    if((fp2 = fopen(GRAPH_FILE, "r")) == NULL){
        fprintf(stderr, "Unable to open the file!");
        exit(EXIT_FAILURE);
    }

    graph = GraphInit(fp2);
    fclose(fp2);
    
    solveSudoku(graph);
    printSolution(graph, stdout);
    freeGraph(graph);

    return 0;
}

void sudokuToGraph(FILE *sudoku_file, FILE *graph_file){
    /*

    Number between 1 and SUDOKU_DIM. So one int each, it means that the file can have the following structure

    1_2_3_4_5_6_7_8_9
    1_2_3_4_5_6_7_8_9
    1_2_3_4_5_6_7_8_9
    1_2_3_4_5_6_7_8_9
    1_2_3_4_5_6_7_8_9
    1_2_3_4_5_6_7_8_9
    1_2_3_4_5_6_7_8_9
    1_2_3_4_5_6_7_8_9
    1_2_3_4_5_6_7_8_9
    
    Nine rows and nine colums. If a zero is located where a number could be written, it means that the number is not given at the beginning of the game.

    */

    /*

    Graph file format:
    81
    0 8 <-- number written in that vertex (color)
    ...
    80 0 <-- zero if empty
    2 11 <-- edges
    ...

    */

    /*

    Let's print out the 81 vertices 
    
    */

    int i = 0;
    int n_vertices = SUDOKU_DIM * SUDOKU_DIM;
    int** sudoku = readSudoku(sudoku_file);
    fprintf(graph_file, "%d\n", n_vertices);
    for(i = 0; i < n_vertices; i++){
        fprintf(graph_file, "%d %d\n", i, sudoku[i / SUDOKU_DIM][i % SUDOKU_DIM]);
    }

    /*

    Let's create all the edges for the sub-blocks. Remember that the graph is undirected.

    1 - 2 - 3
    | X | X |
    1 - 2 - 3
    | X | X |
    1 - 2 - 3
    
    */
    
    int j, sub, vertex_number;
    for(i = 0; i < SUDOKU_DIM; i += N){
        for(j = 0; j < SUDOKU_DIM; j += N){
            //Diagonal edges
            vertex_number = i * SUDOKU_DIM + j;
            fprintf(graph_file, "%d %d\n", vertex_number, vertex_number + SUDOKU_DIM + 1);
            fprintf(graph_file, "%d %d\n", vertex_number, vertex_number + 2 * SUDOKU_DIM + 2);
            fprintf(graph_file, "%d %d\n", vertex_number + SUDOKU_DIM + 1, vertex_number + 2 * SUDOKU_DIM + 2);
            fprintf(graph_file, "%d %d\n", vertex_number + 1, vertex_number + 2 + SUDOKU_DIM);
            fprintf(graph_file, "%d %d\n", vertex_number + SUDOKU_DIM, vertex_number + 2 * SUDOKU_DIM + 1);

            fprintf(graph_file, "%d %d\n", vertex_number + 2, vertex_number + SUDOKU_DIM + 1);
            fprintf(graph_file, "%d %d\n", vertex_number + 2, vertex_number + 2 * SUDOKU_DIM );
            fprintf(graph_file, "%d %d\n", vertex_number + SUDOKU_DIM + 1, vertex_number + 2 * SUDOKU_DIM );
            fprintf(graph_file, "%d %d\n", vertex_number + 1, vertex_number + SUDOKU_DIM);
            fprintf(graph_file, "%d %d\n", vertex_number + SUDOKU_DIM + 2, vertex_number + 2 * SUDOKU_DIM + 1);

            //Other edges
            fprintf(graph_file, "%d %d\n", vertex_number, vertex_number + SUDOKU_DIM + 2);
            fprintf(graph_file, "%d %d\n", vertex_number, vertex_number + 2 * SUDOKU_DIM + 1);
            fprintf(graph_file, "%d %d\n", vertex_number + 1, vertex_number + 2 * SUDOKU_DIM );
            fprintf(graph_file, "%d %d\n", vertex_number + 1, vertex_number + 2 * SUDOKU_DIM + 2);
            fprintf(graph_file, "%d %d\n", vertex_number + 2, vertex_number + 2 * SUDOKU_DIM + 1);
            fprintf(graph_file, "%d %d\n", vertex_number + 2, vertex_number + SUDOKU_DIM);
            fprintf(graph_file, "%d %d\n", vertex_number + SUDOKU_DIM, vertex_number + 2 * SUDOKU_DIM + 2);
            fprintf(graph_file, "%d %d\n", vertex_number + 2 * SUDOKU_DIM , vertex_number + SUDOKU_DIM + 2);
        }
    }

    for(i = 0; i < SUDOKU_DIM; i++){
        for(sub = 0; sub < SUDOKU_DIM; sub++){
            //Row edges
            vertex_number = i * SUDOKU_DIM + sub;
            for(j = 1; j < SUDOKU_DIM - sub; j++){
                fprintf(graph_file, "%d %d\n", vertex_number, vertex_number + j);
            }
            //Column edges
            vertex_number = i + sub * SUDOKU_DIM;
            for(j = SUDOKU_DIM; j < SUDOKU_DIM * SUDOKU_DIM - (sub * SUDOKU_DIM) ; j += SUDOKU_DIM){
                fprintf(graph_file, "%d %d\n", vertex_number, vertex_number + j);
            }
        }
    }

}

int** readSudoku(FILE *sudoku_file){
    int** matrix = malloc2D(SUDOKU_DIM, SUDOKU_DIM);
    int a, i, j;

    for(i = 0; i < SUDOKU_DIM; i++){
        for(j = 0; j < SUDOKU_DIM; j++){
            fscanf(sudoku_file, "%d", &a);
            matrix[i][j] = a;
        }
    }

    return matrix;
}

void solveSudoku(G graph){
    PreColoringExtToListColoring(graph);
    ListColoring(graph);
}