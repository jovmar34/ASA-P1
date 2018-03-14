#include <stdio.h>
#include <stdlib.h>


/* ------------------------------ Estruturas ------------------------------ */

typedef struct node{
    int id;
    struct node* next;
} Vertice;

typedef struct scc_lst {
    Vertice* componente;
    struct scc_lst *next;
} SCC_lst;

/* ------------------------------------------------------------------------- */


/* --------------------------- Variaveis Globais --------------------------- */

SCC_lst *sccs_list;
int scc_comps;

int *stack;
int indstack = 0;

int visited;
int *d, *h;

/* ------------------------------------------------------------------------- */


/* ------------------------- Biblioteca de funcoes ------------------------- */

void addSCC(Vertice *comp_nova) {
    SCC_lst *nova_componete = (SCC_lst*)malloc(sizeof(SCC_lst));
    nova_componete->componente = comp_nova;
    nova_componete->next = sccs_list;
    sccs_list = nova_componete;

    scc_comps++;
}

void push(int id_vert) {
    stack[indstack] = id_vert;
    id_vert++;
}

int pop() {
    indstack--;
    return stack[indstack];
}

/* ------------------------------------------------------------------------- */


int main() {
    int nvertices, narcos, i;
    Vertice **tabela;

    scanf("%d", &nvertices);
    scanf("%d", &narcos);

    tabela = (Vertice**) malloc(nvertices * sizeof(Vertice*));

    d = (int*) malloc(nvertices * sizeof(int));
    h = (int*) malloc(nvertices * sizeof(int));

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



/*    for(i = 0; i < nvertices; i++) {
        for(manel = tabela[i]; manel != NULL; manel = manel->next) {
            printf("%d %d\n", i+1, manel->id);
        }
    } */

    return 0;
}