#ifndef GRAPH_H
#define GRAPH_H
#include <stdio.h>

typedef struct edge { int v; int w; } Edge;

typedef struct graph *Graph;

Graph GRAPHload(FILE *fin);
void  GRAPHinsertE(Graph G, int id1, int id2);
void  GRAPHedges(Graph G, Edge *a);

// Funzioni leggermente modificate
Graph GRAPHinit(int V);
void  GRAPHfree(Graph G);
int GRAPHcc(Graph G);

// Funzioni create in sede d'esame
int GRAPHsizeG(Graph G);
void GRAPHcalculateDegree(Graph G, int *D);
void GRAPHk_core(Graph G, int *D);
void GRAPHj_edge_connected(Graph G);

#endif
