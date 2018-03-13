#include <stdio.h>
#include <stdlib.h>

//AND SO IT BEGINS!!!

typedef struct node{
    int id;
    struct node* next;
} vertice;

int main() {
    int nvertices, narcos, i;
    vertice **tabela;
    vertice *manel;

    scanf("%d", &nvertices);
    scanf("%d", &narcos);

    tabela = (vertice**) malloc(nvertices * sizeof(vertice*));

    for (i = 0; i < nvertices; i++) {
        tabela[i] = NULL;
    }

    for (i = 0; i < narcos; i++) {
        int origem, destino;
        vertice* dest;

        scanf("%d %d", &origem, &destino);

        dest = (vertice*) malloc(sizeof(vertice));

        dest->id = destino;
        dest->next = tabela[origem - 1];

        tabela[origem - 1] = dest;
    }

    for(i = 0; i < nvertices; i++) {
        for(manel = tabela[i]; manel != NULL; manel = manel->next) {
            printf("%d %d\n", i+1, manel->id);
        }
    }

    return 0;
}