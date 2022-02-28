#ifndef __GRAPH__
#define __GRAPH__
#include <stdint.h>
#include <stdio.h>
#include "List.h"
#include "Functions.h"

typedef struct graph_ *G;

G GraphInit(FILE *fp);
void PreColoringExtToListColoring(G graph);
void ListColoring(G graph);
void printSolution(G graph, FILE *fp);
void freeGraph(G graph);

#endif