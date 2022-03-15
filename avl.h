#include <stdio.h>
#include <time.h>
#define MAX 100

typedef struct noArv* Arv;

struct item{
    char pal[MAX];
    int linha;
};
typedef struct item* Item;

Arv cria_arv_vazia(void);
int cria_arv(Arv*, Arv, Arv, Item);
int arv_vazia(Arv);
int arv_altura(Arv);
int arv_tamanho(Arv);
int arv_palavra_pertence(Arv, char*, Arv*);
int arv_item_pertence(Arv, Item, Arv*);
int insere_arv(Arv*, Item);
void esvazia_arv(Arv*);
void apaga_arv(Arv*);
int imprimir_indice(Arv, FILE*, clock_t);