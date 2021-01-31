#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

int menu(Graph G, int *degree);

int main(int argc, char **argv) {
    if(argc != 2) {
        printf("Errore passaggio parametri.");
        exit(-1);
    }

    FILE *fp;
    fp = fopen(argv[1], "r");
    Graph G = GRAPHload(fp);
    fclose(fp);

    int *degree = malloc(GRAPHsizeG(G)*sizeof(int));
    for (int i = 0; i < GRAPHsizeG(G); i++)
        degree[i] = 0;

    GRAPHcalculateDegree(G, degree);

    while(menu(G, degree));
    printf("\nFine programma.");

    GRAPHfree(G);
    free(degree);
    return 0;
}

int scelta() {
    int s;
    printf("\nOpzioni disponibili:\n1 Calcola k-core di G dando k.\n"
           "2 Calcola j-edge-connected dando j\n"
           "Inserire qualsiasi altro intero per terminare il programma.\n");
    scanf("%d\n", &s);
    return s;
}

int menu(Graph G, int *degree) {
    int s = scelta();
    switch(s) {
        case 1:
            GRAPHk_core(G, degree);
            return 1;
        case 2:
            GRAPHj_edge_connected(G);
            return 1;
        default:
            return 0;
    }
}