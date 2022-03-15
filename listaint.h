#include <stdio.h>

typedef struct noLista* Lista;

int cria_lista(Lista*);
int lista_vazia(Lista);
int tamanho_lista(Lista, int*);
int insere_elem(Lista, int);
int pertence(Lista, int);
int imprimir_lista(Lista, FILE*);
int apaga_lista(Lista*);