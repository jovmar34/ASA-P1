#include <stdio.h>
#include <stdlib.h>


/* ------------------------------ Estruturas ------------------------------ */

typedef struct node{
    int id;
    struct node* next;
} Vertice;

typedef struct scc_lst {
    int id;
    Vertice* novasAdjs;
    struct scc_lst *next;
} SCC_lst;

/* ------------------------------------------------------------------------- */


/* --------------------------- Variaveis Globais --------------------------- */

SCC_lst *sccs_list;
int scc_comps, scc_arestas;
Vertice* pontes = NULL;

int *stack;
int indstack = 0;

int visited, *d, *h;

Vertice **tabela;
int nvertices, narcos;

/* ------------------------------------------------------------------------- */


/* ------------------------- Biblioteca de funcoes ------------------------- */

void addSCC(int id) {
    SCC_lst *nova_componente = (SCC_lst*) malloc(sizeof(SCC_lst));
    nova_componente->id = id;
    nova_componente->novasAdjs = pontes;
    nova_componente->next = sccs_list;
    sccs_list = nova_componente;

    scc_comps++;
    pontes = NULL;
}


void addPonte() {
    Vertice *novaPonte = (Vertice*) malloc(sizeof(Vertice));
    novaPonte->id = sccs_list->id;
    novaPonte->next = pontes;
    pontes = novaPonte;

    scc_arestas++;
}


void push(int id_vert) {
    stack[indstack] = id_vert;
    indstack++;
}


int pop() {
    indstack--;
    return stack[indstack];
}


int inStack(int ind) {
    int i;
    for (i = 0; i < indstack; i++) {
        if (stack[i] == ind) {
            return 1;
        }
    }
    return 0;
}


int min(int a, int b) {
    return (a < b ? a : b);
}


int Tarjan_visit(int i) {
    Vertice *v;

    h[i] = d[i] = visited;
    visited++;

    push(i+1);

    for (v = tabela[i]; v != NULL; v = v->next) {
        int indv = v->id - 1;

        if (d[indv] == -1 || inStack(indv + 1)) {
            if (d[indv] == -1) {
                if (Tarjan_visit(indv)) {
                    addPonte();
                }
            }

            h[i] = min(h[i],h[indv]);
        }
    }

    if (d[i] == h[i]) {
        int x = pop();
        int temp;

        while (x != i + 1) {
            temp = pop();

            if(temp < x) {
                x = temp;
            }
        }

        addSCC(x);
        return 1;
    }

    return 0;
}


void Tarjan() {
    int i;
    
    visited = 0;
    stack = (int*) malloc(nvertices*sizeof(int));

    for (i = 0; i < nvertices; i++) {
        d[i] = -1;
    }

    for (i = 0; i < nvertices; i++) {
        if (d[i] == -1) {
            Tarjan_visit(i);
        }
    }
}


void freeVertices(Vertice *ptr_vert) {
    if (ptr_vert != NULL) {
        freeVertices(ptr_vert->next);
        free(ptr_vert);
    }
}


void freeSCC_lst(SCC_lst *ptr_scc) {
    if (ptr_scc != NULL) {
        freeVertices(ptr_scc->novasAdjs);
        freeSCC_lst(ptr_scc->next);
        free(ptr_scc);
    }
}

/* ------------------------------------------------------------------------- */


int main() {
    int i;
    SCC_lst *lol;
    Vertice *manel;

    scanf("%d", &nvertices);
    scanf("%d", &narcos);

    tabela = (Vertice**) malloc(nvertices * sizeof(Vertice*));

    d = (int*) malloc(nvertices * sizeof(int));
    h = (int*) malloc(nvertices * sizeof(int));
    sccs_list = NULL;

    for (i = 0; i < nvertices; i++) {
        tabela[i] = NULL;
    }

    for (i = 0; i < narcos; i++) {
        int origem, destino;
        Vertice* dest;

        scanf("%d %d", &origem, &destino);

        dest = (Vertice*) malloc(sizeof(Vertice));

        dest->id = destino;
        dest->next = tabela[origem - 1];

        tabela[origem - 1] = dest;
    }

    Tarjan();

    printf("%d\n%d\n", scc_comps, scc_arestas);
    for(lol = sccs_list; lol != NULL; lol = lol->next) {
        for(manel = lol->novasAdjs; manel != NULL; manel = manel->next) {
            printf("%d %d\n", lol->id, manel->id);
        }
    }

    for (i = 0; i < nvertices; i++) {
        freeVertices(tabela[i]);
    }
    free(tabela);

    free(d);
    free(h);

    freeSCC_lst(sccs_list);

    return 0;
}

