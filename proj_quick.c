#include <stdio.h>
#include <stdlib.h>


/* ------------------------------ Estruturas ------------------------------ */

typedef struct node{
    int id;
    struct node* next;
} Vertice;

typedef struct ponte {
    int origem;
    int destino;
} Ponte;

typedef struct ponte_lst {
    Ponte *ponte;
    struct ponte_lst *next;
} Ponte_LST;

/* ------------------------------------------------------------------------- */


/* --------------------------- Variaveis Globais --------------------------- */

int scc_comps;

int npontes;
int *componentes;
Ponte_LST *scc_pontes;
Ponte **pontes_vetor;

int *stack;
int indstack;

int visited;
int *d, *h;

Vertice **tabela;
int nvertices, narcos;

/* ------------------------------------------------------------------------- */


/* ------------------------- Biblioteca de funcoes ------------------------- */

void addPonte(int origem, int destino) {
    Ponte *nova_ponte = (Ponte*) malloc(sizeof(Ponte));
    Ponte_LST *ponte_nova = (Ponte_LST*) malloc(sizeof(Ponte_LST));

    nova_ponte->origem = origem;
    nova_ponte->destino = destino;

    ponte_nova->ponte = nova_ponte;
    ponte_nova->next = scc_pontes;
    scc_pontes = ponte_nova;

    npontes++;
}


int ponteMenorIgual(Ponte *p1, Ponte* p2) {
    return ((p1->origem <= p2->origem) && (p1->destino <= p2->destino));
}


int pontesIguais(Ponte *p1, Ponte *p2) {
    return ((p1->destino == p2->destino) && (p1->origem == p2->origem));
}


int contaArestas() {
    int i;
    int arestas = 1;
    Ponte *ultima = pontes_vetor[0];

    for (i = 1; i < npontes; i++) {
        if (!pontesIguais(ultima, pontes_vetor[i])) {
            ultima = pontes_vetor[i];
            arestas++;
        }
    }
    return arestas;
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


void guardaComponente(Vertice *list, int id_comp) {
    Vertice *vert_temp;
    if (list != NULL) {
        componentes[list->id - 1] = id_comp;
        vert_temp = list->next;
        free(list);
        guardaComponente(vert_temp, id_comp);
    }
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
                    addPonte(i+1, v->id);
                }
            }

            h[i] = min(h[i],h[indv]);
        }
        else {
            addPonte(i+1, v->id);
        }
    }

    if (d[i] == h[i]) {
        int x = pop();
        int temp = x;

        Vertice *list = (Vertice*) malloc(sizeof(Vertice));
        Vertice *vert_temp;
        
        list->id = x;
        list->next = NULL;


        while (temp != i + 1) {
            temp = pop();

            vert_temp = (Vertice*) malloc(sizeof(Vertice));
            vert_temp->id = temp;
            vert_temp->next = list;
            list = vert_temp;

            if(temp < x) {
                x = temp;
            }
        }

        guardaComponente(list, x);

        scc_comps++;
        return 1;
    }

    return 0;
}


void Tarjan() {
    int i;
    
    visited = 0;
    stack = (int*) malloc(nvertices*sizeof(int));
    indstack = 0;

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
    Vertice *vert_temp;
    if (ptr_vert != NULL) {
        vert_temp = ptr_vert->next;
        free(ptr_vert);
        freeVertices(vert_temp);
    }
}


void metePontesVetor() {
    int i = 0;
    Ponte_LST *lista_pontes_temp = scc_pontes;
    Ponte_LST *pontes_temp;

    while(lista_pontes_temp != NULL) {
        pontes_vetor[i] = lista_pontes_temp->ponte;

        pontes_vetor[i]->origem = componentes[pontes_vetor[i]->origem - 1];
        pontes_vetor[i]->destino = componentes[pontes_vetor[i]->destino - 1];
        
        i++;

        pontes_temp = lista_pontes_temp->next;
        free(lista_pontes_temp);
        lista_pontes_temp = pontes_temp;
    }
}


int Partition(int p, int r) {
    Ponte *x = pontes_vetor[r];
    int i = p-1;
    int j;
    Ponte *temp;

    for (j = p; j < r; j++) {
        if (ponteMenorIgual(pontes_vetor[j], x)) {
            i++;
            temp = pontes_vetor[i];
            pontes_vetor[i] = pontes_vetor[j];
            pontes_vetor[j] = temp;
        }
    }

    temp = pontes_vetor[i+1];
    pontes_vetor[i+1] = pontes_vetor[j];
    pontes_vetor[j] = temp;

    return i+1;
}


void quickSort(int p, int r) {
    if (p < r) {
        int q = Partition(p, r);
        quickSort(p, r-1);
        quickSort(q+1, r);
    }
}

/* ------------------------------------------------------------------------- */


int main() {
    int i;
    Ponte *ultima;

    scanf("%d", &nvertices);
    scanf("%d", &narcos);

    tabela = (Vertice**) malloc(nvertices * sizeof(Vertice*));

    d = (int*) malloc(nvertices * sizeof(int));
    h = (int*) malloc(nvertices * sizeof(int));
    
    scc_comps = 0;

    componentes = (int*) malloc(nvertices * sizeof(int));
    scc_pontes = NULL;

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
    

    for (i = 0; i < nvertices; i++) {
        freeVertices(tabela[i]);
    }
    free(tabela);

    free(d);
    free(h);

    free(stack);


    pontes_vetor = (Ponte**) malloc(npontes * sizeof(Ponte*));
    metePontesVetor();


    free(componentes);


    quickSort(0, npontes-1);

    printf("%d\n", scc_comps);

    if (npontes != 0) {
        printf("%d\n", contaArestas());

        printf("%d %d\n", (pontes_vetor[0])->origem, (pontes_vetor[0])->destino);
        ultima = pontes_vetor[0];

        for (i = 1; i < npontes; i++) {
            if (!pontesIguais(ultima, pontes_vetor[i])) {
                printf("%d %d\n", (pontes_vetor[i])->origem, (pontes_vetor[i])->destino);
                ultima = pontes_vetor[i];
            }
        }
    }
    else {
        printf("%d\n", 0);
    }


    for (i = 0; i < npontes; i++) {
        free(pontes_vetor[i]);
    }
    free(pontes_vetor);

    return 0;
}

