#ifndef __LIST__
#define __LIST__
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct LIST_ *LIST;

typedef struct node *link;

struct LIST_{
    link head;
    link tail;
    int n;
};

typedef struct {
    int vertex_number;
    int color;
    int *available_colors;
    int removed;
} Content;

struct node{
    Content* c;
    link next;
    link prev;
};


LIST ListInit();
void insert(Content* c, LIST l);
Content* getAtIndex(int index, LIST l);
Content* getLast(LIST l);
Content* pop(LIST l);
Content* deleteAtIndex(int index, LIST l);
void deleteContent(Content* c, LIST l);
void freeList(LIST l);

#endif