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
int scc_arestas;
int flag_scc = 0;

int *stack;
int indstack = 0;

int visited;
int *d, *h;

Vertice **tabela;
int nvertices, narcos;

Vertice *manel;

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
    indstack++;
}

int pop() {
    indstack--;
    return stack[indstack];
}

int inStack(int ind) {
    for (int i = 0; i < indstack; i++) {
        if (stack[i] == ind) return 1;
    }
    return 0;
}

int min(int a, int b) {
    return (a < b ? a : b);
}

void Tarjan_visit(int i) {
    Vertice *v;

    h[i] = d[i] = visited;

    visited++;

    push(i+1);

    for (v = tabela[i]; v != NULL; v = v->next) {
        int indv = v->id - 1;
        if (d[indv] == -1 || inStack(indv + 1)) {
            if (d[indv] == -1) Tarjan_visit(indv);
            if(foi criada scc?) guarda ultimo vertice
            h[i] = min(h[i],h[indv]);
        }
    }

    if (d[i] == h[i]) {
        int x = pop();
        Vertice *nova_componente = (Vertice*) malloc(sizeof(Vertice));

        nova_componente->id = x;
        nova_componente->next = NULL;

        while (x != i + 1) {
            Vertice *atual = (Vertice*) malloc(sizeof(Vertice));

            x = pop();

            atual->id = x;

            if(x < nova_componente->id) {
                free(nova_componente);
                nova_componente = atual;
            }
            else {
                free(atual);
            }
        }

        addSCC(nova_componente);
    }
}

void Tarjan() {
    visited = 0;
    stack = (int*) malloc(nvertices*sizeof(int));
    int i;

    for (i = 0; i < nvertices; i++) {
        d[i] = -1;
    }

    for (i = 0; i < nvertices; i++) {
        if (d[i] == -1) {
            Tarjan_visit(i);
        }
    }
}

/* ------------------------------------------------------------------------- */


int main() {
    int i;

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

/*    printf("\n");
    for(i = 0; i < nvertices; i++) {
        printf("%d", i + 1);
        for(manel = tabela[i]; manel != NULL; manel = manel->next) {
            printf("\t%d ", manel->id);
        }
        printf("\n");
    } */

    Tarjan();

    for(SCC_lst *lol = sccs_list; lol != NULL; lol = lol->next) {
        for(manel = lol->componente; manel != NULL; manel = manel->next) {
            printf("%d ", manel->id);
        }
        printf("\n");
    }

    printf("%d\n", scc_comps);

    return 0;
}

