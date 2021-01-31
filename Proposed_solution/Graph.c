#include <stdlib.h>
#include <stdio.h>
#include "ST.h"
#include "Graph.h"

#define MAXC 11

// MODIFICHE A STRUTTURA DATI E GRAPHinit e GRAPHfree PER ADATTARE ALLA MATRICE eliminati.
// FUNZIONI SPECIFICHE INSERITE AL FONDO.

struct graph {int V; int E; int **madj; ST tab; int **eliminati; };
static Edge  EDGEcreate(int v, int w);
static int **MATRIXint(int r, int c, int val);
static void  insertE(Graph G, Edge e);

static Edge EDGEcreate(int v, int w) {
  Edge e;
  e.v = v;
  e.w = w;
  return e;
}

static int **MATRIXint(int r, int c, int val) {
  int i, j;
  int **t = malloc(r * sizeof(int *));
  if (t==NULL)
    return NULL;

  for (i=0; i < r; i++) {
    t[i] = malloc(c * sizeof(int));
    if (t[i]==NULL)
      return NULL;
  }
  for (i=0; i < r; i++)
    for (j=0; j < c; j++)
      t[i][j] = val;
  return t;
}

Graph GRAPHinit(int V) {
  Graph G = malloc(sizeof *G);
  if (G == NULL)
    return NULL;
  G->V = V;
  G->E = 0;
  G->madj = MATRIXint(V, V, 0);
  if (G->madj == NULL)
    return NULL;

  // Aggiunta allocazione matrice archi eliminati
  G->eliminati = MATRIXint(V, V, 0);
  if(G->eliminati == NULL)
      return NULL;

  G->tab = STinit(V);
  if (G->tab == NULL)
    return NULL;
  return G;
}

void GRAPHfree(Graph G) {
  int i;
  for (i=0; i<G->V; i++)
    free(G->madj[i]);
  free(G->eliminati);
  free(G->madj);
  STfree(G->tab);
  free(G);
}

Graph GRAPHload(FILE *fin) {
  int V, i, id1, id2;
  char label1[MAXC], label2[MAXC];
  Graph G;

  fscanf(fin, "%d", &V);
  G = GRAPHinit(V);

  for (i=0; i<V; i++) {
    fscanf(fin, "%s", label1);
    STinsert(G->tab, label1, i);
  }

  while(fscanf(fin, "%s %s", label1, label2) == 2) {
    id1 = STsearch(G->tab, label1);
    id2 = STsearch(G->tab, label2);
    if (id1 >= 0 && id2 >=0)
      GRAPHinsertE(G, id1, id2);
  }
  return G;
}

static void  insertE(Graph G, Edge e) {
  int v = e.v, w = e.w;

  if (G->madj[v][w] == 0)
    G->E++;
  G->madj[v][w] = 1;
  G->madj[w][v] = 1;
}

void GRAPHinsertE(Graph G, int id1, int id2) {
  insertE(G, EDGEcreate(id1, id2));
}

void  GRAPHedges(Graph G, Edge *a) {
  int v, w, E = 0;
  for (v=0; v < G->V; v++)
    for (w=v+1; w < G->V; w++)
      if (G->madj[v][w] == 1)
        a[E++] = EDGEcreate(v, w);
}



// Richiesta 2

int GRAPHsizeG(Graph G) {
    return G->V;
}

void GRAPHcalculateDegree(Graph G, int *D) {
    int i, degree, j;
    for(i = 0; i < G->V; i++) {
        degree = 0;
        for(j = 0; j < G->V; j++)
            if(G->madj[i][j] != 0)
                degree++;
        D[i] = degree;
    }
    // Eliminato for in cui dimezzavo il degree ai vertici.
}

void GRAPHk_core(Graph G, int *D) {
    int k, *D1, i, j, end = 1;
    scanf("%d\n", &k);
    D1 = malloc(G->V*sizeof(int));

    for(i = 0; i < G->V; i++)
        D1[i] = D[i];

    while(end) {
        end = 0;
        for(i = 0; i < G->V; i++) {
            if(D1[i] < k && D1[i] != -1) {
                D1[i] = -1;
                end = 1;
                for(j = 0; j < G->V; j++)
                    if(G->madj[j][i] != 0)
                        D1[j]--;
            }
        }
    }

    printf("Insieme k-core: ");
    for(i = 0; i < G->V; i++)
        if(D1[i] != -1)
            printf("%s\n", STsearchByIndex(G->tab, i));

    free(D1);
}



// Richiesta 3

void eliminaArchi(Graph G, Edge *sol, int n) {
    for (int i = 0; i < n; ++i) {
        G->eliminati[sol[i].v][sol[i].w] = 1;
    }
}

static void dfsRcc(Graph G, int v, int id, int *cc) {
    int w;
    cc[v] = id;
    for (w = 0; w < G->V; w++)
        if(G->madj[v][w] != 0 && G->eliminati[v][w] == 0)
            if (cc[w] == -1)
                dfsRcc(G, w, id, cc);
}

int GRAPHcc(Graph G) {
    int v, id = 0, *cc;
    cc = malloc(G->V * sizeof(int));
    if (cc == NULL)
        return -1;

    for (v = 0; v < G->V; v++)
        cc[v] = -1;

    for (v = 0; v < G->V; v++)
        if (cc[v] == -1)
            dfsRcc(G, v, id++, cc);

    printf("Connected component(s) \n");
    for (v = 0; v < G->V; v++)
        printf("node %s belongs to cc %d \n", STsearchByIndex(G->tab, v), cc[v]);
    return id;
}

int comb_semplici(Graph G, Edge *A, Edge *sol, int E, int n, int pos, int start) {
    int cc, j, i;
    if(pos == n) {
        eliminaArchi(G, sol, n);
        cc = GRAPHcc(G);
        if(cc > 1) {
            printf("Archi da eliminare: \n");
            for(i = 0; i < n; i++)
                printf("%s, %s\n", STsearchByIndex(G->tab, sol[i].v), STsearchByIndex(G->tab, sol[i].w));
            return 1;
        }
        return 0;
    }
    for(i = start; i < E; i++) {
        if(G->madj[A[i].v][A[i].w] == 1 && G->eliminati[A[i].v][A[i].w] == 0) {
            G->eliminati[A[i].v][A[i].w] = 1;
            sol[pos] = A[i];
            if(comb_semplici(G, A, sol, E, n, pos+1, i+1))
                return 1;
            G->eliminati[A[i].v][A[i].w] = 0;
        }
    }
    return 0;
}

void GRAPHj_edge_connected(Graph G) {
    int cc, j;
    cc = GRAPHcc(G);
    if(cc != 1) {
        printf("Grafo già sconnesso");
        return;
    }
    printf("Inserire j numero di archi: ");
    scanf("%d\n", &j);
    if(j < 1)
        return;

    Edge *sol = malloc(j * sizeof(Edge));
    Edge *Archi = malloc(G->E*sizeof(Edge));
    GRAPHedges(G, Archi);
    if(!comb_semplici(G, Archi, sol, G->E, j, 0, 0))
        printf("Non esiste sottoinsieme di archi j.\n");
    free(sol);
    free(Archi);
}
