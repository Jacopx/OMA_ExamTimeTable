/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *          OMA Project of Jacopo Maggio, Stefano Munna, Jacopo Nasi, Andrea Santu and Marco Torlaschi               *
 *                      Repository available on https://github.com/Jacopx/OMA_ExamTimeTable                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdlib.h>
#include <stdio.h>
#include "ADTgraph.h"

struct graph {
    int V, E; // V number of vertices, E number of edges
    int **adj; // adjacency matrix
    // @TODO evaluate if add a structure to remember exam id or not
};

int **MatrixInit (int rc, int initVal) {
    int i, j;
    int **table = malloc(rc * sizeof(int *));

    for (i = 0; i < rc; ++i) table[i] = malloc(rc * sizeof(int));
    for (i = 0; i < rc; ++i)
        for (j = 0; j < rc; ++j)
            table[i][j] = initVal;

    return table;
}

Graph GraphInit (int V) {
    Graph g = malloc(sizeof(*g));
    if (g == NULL) return NULL;

    g->E = 0;
    g->V = V;
    g->adj = MatrixInit(V, 0);

    return g;
}

void GraphInsertE (Graph g, edge e) {
    int prev = g->adj[e.v][e.w];

    if (e.v == e.w) {
        printf("Warning, edge between same vertex");
        exit(1);
    }
    if (prev == 0) g->E++;

    // it's useful to increment the previous value for the Conflict graph
    g->adj[e.v][e.w] = prev + 1;
    g->adj[e.w][e.v] = prev + 1;
}

void GraphRemoveE (Graph g, edge e) {
    if (g->adj[e.v][e.w] != 0) g->E--;
    g->adj[e.v][e.w] = 0;
    g->adj[e.w][e.v] = 0;
}

void GraphFree (Graph g) {
    int i;
    for (i = 0; i < g->V; ++i) free(g->adj[i]);
    free(g->adj);
    free(g);
}