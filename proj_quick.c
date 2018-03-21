#include <stdio.h>
#include <stdlib.h>


/* ------------------------------ Estruturas ------------------------------ */

typedef struct node{
    int id;
    struct node* next;
} Vertice;

/* ------------------------------------------------------------------------- */


/* --------------------------- Variaveis Globais --------------------------- */

Vertice **tabela;
int nvertices, narcos;

Vertice **scc;
int comps, arestas, scc_comps, scc_arestas;

int *componentes;

int *stack;
int indstack;
int *inStack;

int visited;
int *d, *h;

/* ------------------------------------------------------------------------- */


/* ------------------------- Biblioteca de funcoes ------------------------- */

void push(int id_vert) {
    stack[indstack] = id_vert;
    indstack++;
    inStack[id_vert - 1] = 1;
}


int pop() {
    int res;
    indstack--;
    res = stack[indstack];
    inStack[res - 1] = 0;
    return res;
}


void tira_stack(int id) {
    int i = indstack-1;
    int x = pop();
    int temp = x;

    while (stack[i] != id) {
        if (stack[i] < x) {
            x = stack[i];
        }
        i--;
    }
    
    if (stack[i] < x) {
        x = stack[i];
    }

    componentes[temp-1] = x;

    while (indstack != i) {
        temp = pop();

        componentes[temp-1] = x;
    }
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

        if (d[indv] == -1 || inStack[indv]) {
            if (d[indv] == -1) {
                if (Tarjan_visit(indv)) {
                    scc_arestas++;
                }
            }

            h[i] = min(h[i],h[indv]);
        }
        else {
            scc_arestas++;
        }
    }

    if (d[i] == h[i]) {
        tira_stack(i+1);
        scc_comps++;
        return 1;
    }

    return 0;
}


void Tarjan() {
    int i;
    
    d = (int*) malloc(nvertices * sizeof(int));
    h = (int*) malloc(nvertices * sizeof(int));
    
    visited = 0;

    scc_comps = 0;
    scc_arestas = 0;

    componentes = (int*) malloc(nvertices * sizeof(int));

    stack = (int*) malloc(nvertices*sizeof(int));
    inStack = (int*) malloc(nvertices*sizeof(int));
    indstack = 0;

    for (i = 0; i < nvertices; i++) {
        d[i] = -1;
        inStack[i] = 0;
    }

    for (i = 0; i < nvertices; i++) {
        if (d[i] == -1) {
            Tarjan_visit(i);
        }
    }
}


int inSCC(int id_comp) {
    int i;

    for (i = 0; i < comps; i++) {
        if (scc[i]->id == id_comp) {
            return 1;
        }
    }
    return 0;
}


void criaSCC() {
    int i;
    Vertice *temp;

    scc = (Vertice**) malloc(scc_comps * sizeof(Vertice*));
    comps = 0;

    for (i = 0; comps != scc_comps; i++) {
        if (!inSCC(componentes[i])) {
            temp = (Vertice*) malloc(sizeof(Vertice));

            temp->id = componentes[i];
            temp->next = NULL;
            
            scc[comps] = temp;
            comps++;
        }
    }
}


void meteNaSCC(int origem, int destino) {
    int i;
    Vertice *temp;
    Vertice *novo;

    for (i = 0; scc[i]->id != origem; i++);


    for (temp = scc[i]; temp->next != NULL; temp = temp->next) {

        if (destino < temp->next->id) {
            novo = (Vertice*) malloc(sizeof(Vertice));

            novo->id = destino;
            novo->next = temp->next;
            temp->next = novo;
            arestas++;
            return;
        }
        else if (destino == temp->next->id) {
            scc_arestas--;
            return;
        }
    }
    
    novo = (Vertice*) malloc(sizeof(Vertice));

    novo->id = destino;
    novo->next = NULL;
    temp->next = novo;
    arestas++;
}


void metePontesSCC() {
    int i;
    Vertice *temp;

    for (i = 0; arestas < scc_arestas; i++) {
        for (temp = tabela[i]; temp != NULL; temp = temp->next) {
            if (componentes[i] != componentes[temp->id - 1]) {
                meteNaSCC(componentes[i], componentes[temp->id -1]);
            }
        }
    }
}


void freeVertices(Vertice *ptr_vert) {
    Vertice *vert_temp;
    if (ptr_vert != NULL) {
        vert_temp = ptr_vert->next;
        free(ptr_vert);
        freeVertices(vert_temp);
    }
}



/* ------------------------------------------------------------------------- */


int main() {
    int i;
    Vertice *temp;

    scanf("%d", &nvertices);
    scanf("%d", &narcos);

    tabela = (Vertice**) malloc(nvertices * sizeof(Vertice*));

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

    criaSCC();

    metePontesSCC();

    printf("%d\n%d\n", scc_comps, scc_arestas);
    for (i=0; i < scc_comps; i++) {
        for(temp = scc[i]->next; temp != NULL; temp = temp->next) {
            printf("%d %d\n", scc[i]->id, temp->id);
        }
    }

    free(d);
    free(h);
    free(componentes);
    free(stack);
    free(inStack);

    for (i = 0; i < nvertices; i++) {
        freeVertices(tabela[i]);
    }
    free(tabela);

    for (i = 0; i < scc_comps; i++) {
        freeVertices(scc[i]);
    }
    free(scc);

    return 0;
}

