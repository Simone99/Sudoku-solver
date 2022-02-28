#include "Graph.h"

struct graph_{
    LIST *adj_list;
    Content *vertices;
    int V, E;
};

G GraphInit(FILE *fp){
    int i, a, b;
    G return_graph = malloc(sizeof(*return_graph));
    fscanf(fp, "%d", &(return_graph -> V));
    return_graph -> vertices = malloc((return_graph -> V)*sizeof(*(return_graph -> vertices)));
    return_graph -> adj_list = malloc((return_graph -> V)*sizeof(*(return_graph -> adj_list)));
    return_graph -> E = 0;
    for (i = 0; i < return_graph -> V; i++){
        fscanf(fp, "%d %d", &a, &b);
        return_graph -> adj_list[a] = ListInit();
        return_graph -> vertices[a] = (Content){a, b, NULL, 0};
    }
    while((fscanf(fp, "%d %d", &a, &b)) != EOF){
        insert(&(return_graph -> vertices[b]), return_graph -> adj_list[a]);
        insert(&(return_graph -> vertices[a]), return_graph -> adj_list[b]);
        (return_graph -> E)++;
    }
    return return_graph;
}

void PreColoringExtToListColoring(G graph){
    int i, j;
    link p;
    for(i = 0; i < graph -> V; i++){
        if(graph -> vertices[i].color == 0){
            /*
            
            Available_colors vector uses indexes in order to retrive colors, so if availble_colors[i] == 0 it means that color i+1 is not available for that
            vertex, given that one neighbor has that color

            */

            graph -> vertices[i].available_colors = malloc(SUDOKU_DIM * sizeof(*(graph -> vertices[i].available_colors)));
            for(j = 0; j < SUDOKU_DIM; j++){
                graph -> vertices[i].available_colors[j] = j + 1;
            }
            //Look at vertex neighbors in order to set vertex colors as not available
            for(p = graph -> adj_list[i] -> head; p != NULL; p = p -> next){
                if(p -> c -> color != 0){
                    //Set color as not available
                    graph -> vertices[i].available_colors[p -> c -> color - 1] = 0;
                }
            }
        }
    }
    for(i = 0; i < graph -> V; i++){
        if(graph -> vertices[i].color != 0){
            graph -> vertices[i].removed = 1;
        }
    }
}

void ListColoringRecursive(G graph, int vertex_number, int *found){
    int i, j, *vertices_to_restore, n_vertices;
    link p;
    if(vertex_number >= 81){
        (*found) = 1;
        return;
    }
    if(!graph -> vertices[vertex_number].removed){
        for(i = 0; i < SUDOKU_DIM && !(*found); i++){
            if(graph -> vertices[vertex_number].available_colors[i] != 0){
                graph ->  vertices[vertex_number].color = graph ->  vertices[vertex_number].available_colors[i];
                vertices_to_restore = malloc((3 * SUDOKU_DIM - 2 * N - 1)*sizeof(*vertices_to_restore));
                n_vertices = 0;
                for (p = graph->adj_list[vertex_number]->head; p != NULL; p = p->next) {
                    if (!p->c->removed && p->c->available_colors[i] != 0) {
                        p->c->available_colors[i] = 0;
                        vertices_to_restore[n_vertices++] = p->c->vertex_number;
                    }
                }
                ListColoringRecursive(graph, vertex_number + 1, found);
                if(!(*found)){
                    graph ->  vertices[vertex_number].color = 0;
                    for (j = 0; j < n_vertices; j++) {
                        graph->vertices[vertices_to_restore[j]].available_colors[i] = i + 1;
                    }
                    n_vertices = 0;
                }
                free(vertices_to_restore);
            }
        }
    }else{
        ListColoringRecursive(graph, vertex_number + 1, found);
    }
    return;
}

void ListColoring(G graph){
    int found = 0;
    ListColoringRecursive(graph, 0, &found);
}

void printSolution(G graph, FILE *fp){
    int i, j;
    for(i = 0; i < SUDOKU_DIM; i++){
        for(j = 0; j < SUDOKU_DIM - 1; j++){
            fprintf(fp, "%d ", graph -> vertices[i * SUDOKU_DIM + j].color);
        }
        fprintf(fp, "%d\n", graph -> vertices[i * SUDOKU_DIM + j].color);
    }
}

void freeGraph(G graph) {
    int i;
    for (i = 0; i < graph->V; i++) {
        freeList(graph->adj_list[i]);
        if (graph->vertices[i].available_colors != NULL) {
            free(graph->vertices[i].available_colors);
        }
    }
    free(graph->adj_list);
    free(graph->vertices);
    free(graph);
}