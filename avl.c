#ifndef AVL_H
#define AVL_H
#include "avl.h"
#endif

#ifndef LISTA_H
#define LISTA_H
#include "listaint.h"
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

struct noArv{
    char pal[MAX];
    Lista linhas;
    int altura;
    struct noArv *esq, *dir;
};

Arv cria_arv_vazia(void){
    return NULL;
}

int max(int a, int b){
    return (a > b ? a : b);
}

int cria_arv(Arv* arv, Arv subesq, Arv subdir, Item item){
    if(arv == NULL) return 0;

    Arv a = (Arv)malloc(sizeof(struct noArv));
    if(a == NULL) return 0;

    strcpy(a->pal, item->pal);
    if(!cria_lista(&a->linhas) || !insere_elem(a->linhas, item->linha)){
        free(a);
        return 0;
    }

    if(subesq == NULL && subdir == NULL) a->altura = 0;
    else if(subesq == NULL) a->altura = 1 + subdir->altura;
    else if(subdir == NULL) a->altura = 1 + subesq->altura;
    else a->altura = 1 + max(subesq->altura, subdir->altura);

    a->esq = subesq;
    a->dir = subdir;
    *arv = a;

    return 1;
}

int arv_vazia(Arv arv){
    return (arv == NULL);
}

int arv_altura(Arv arv){
    if(arv_vazia(arv)) return 0;
    else return arv->altura;
}

int arv_tamanho(Arv arv){
    if(arv == NULL) return 0;
    return(1 + arv_tamanho(arv->esq) + arv_tamanho(arv->dir));
}

int arv_palavra_pertence(Arv arv, char* p, Arv* no){
    if(arv_vazia(arv)) return 0;
    else
        if(!strcoll(p, arv->pal)){
            *no = arv;
            return 1;
        }else return (arv_palavra_pertence(arv->esq, p, no) || arv_palavra_pertence(arv->dir, p, no));
}

int arv_item_pertence(Arv arv, Item item, Arv* no){
    if(arv_vazia(arv)) return 0;
    else
        if(!strcoll(arv->pal, item->pal) && pertence(arv->linhas, item->linha)){
            *no = arv;
            return 1;
        }else return (arv_item_pertence(arv->esq, item, no) || arv_item_pertence(arv->dir, item, no));
}

int fator_balanceamento(Arv arv){
    if(arv == NULL) return 0;
    return(arv_altura(arv->esq) - arv_altura(arv->dir));
}

Arv rotacao_esquerda(Arv arv) {
    Arv x = arv->dir;
    Arv y = x->esq;

    x->esq = arv;
    arv->dir = y;

    arv->altura = max(arv_altura(arv->esq), arv_altura(arv->dir)) + 1;
    x->altura = max(arv_altura(x->esq), arv_altura(x->dir)) + 1;

    return x;
}

Arv rotacao_direita(Arv arv) {
    Arv x = arv->esq;
    Arv y = x->dir;

    x->dir = arv;
    arv->esq = y;

    arv->altura = max(arv_altura(arv->esq), arv_altura(arv->dir)) + 1;
    x->altura = max(arv_altura(x->esq), arv_altura(x->dir)) + 1;

    return x;
}

Arv insere_arv_(Arv, Item);

int insere_arv(Arv* arv, Item item){
    if(arv == NULL || item == NULL) return 0;

    *arv = insere_arv_(*arv, item);
    if(*arv == NULL) return 0;
    return 1;
}

Arv insere_arv_(Arv arv, Item item) {
    if (arv == NULL){
        Arv novo = (Arv)malloc(sizeof(struct noArv));
        if(novo == NULL) return novo;

        strcpy(novo->pal, item->pal);
        if(!cria_lista(&novo->linhas) || !insere_elem(novo->linhas, item->linha)){
            free(novo);
            return NULL;
        }
        novo->esq = NULL;
        novo->dir = NULL;
        novo->altura = 1;
        return novo;
    }
    
    if(strcoll(item->pal, arv->pal) < 0)
        arv->esq = insere_arv_(arv->esq, item);
    else if(strcoll(item->pal, arv->pal) > 0)
        arv->dir = insere_arv_(arv->dir, item);
    else{
        insere_elem(arv->linhas, item->linha);
        return arv;
    }

    arv->altura = 1 + max(arv_altura(arv->esq), arv_altura(arv->dir));

    int balanceamento = fator_balanceamento(arv);
    if (balanceamento > 1 && strcoll(item->pal, arv->esq->pal) < 0)
        return rotacao_direita(arv);

    if (balanceamento < -1 && strcoll(item->pal, arv->dir->pal) > 0)
        return rotacao_esquerda(arv);

    if (balanceamento > 1 && strcoll(item->pal, arv->esq->pal) > 0) {
        arv->esq = rotacao_esquerda(arv->esq);
        return rotacao_direita(arv);
    }

    if (balanceamento < -1 && strcoll(item->pal, arv->dir->pal) < 0) {
        arv->dir = rotacao_direita(arv->dir);
        return rotacao_esquerda(arv);
    }

    return arv;
}

void esvazia_arv(Arv* arv){
    if(arv == NULL || *arv == NULL) return;

    esvazia_arv(&(*arv)->esq);
    esvazia_arv(&(*arv)->dir);
    free(*arv);
    *arv = NULL;
}

void apaga_arv(Arv* arv){
    esvazia_arv(arv);
}

void imprimir_indice_(Arv, FILE*, int*, int*);

int imprimir_indice(Arv arv, FILE* fp, clock_t tempo){
    if(fp == NULL) return 0;

    fprintf(fp, "Índice:\n\n");
    int p = 0, pd = 0;
    imprimir_indice_(arv, fp, &p, &pd);
    fprintf(fp, "\nNúmero total de palavras: %d.\n", p);
    fprintf(fp, "Numero de palavras distintas: %d.\n", pd);
    if(tempo >= 0)
        fprintf(fp, "Tempo de construção do índice usando árvore AVL: %lds.", tempo);
}

void imprimir_indice_(Arv arv, FILE* fp, int* p, int* pd){
    if(!(arv_vazia(arv))){
        int t, *li, i;
        setlocale(LC_ALL, "Portuguese_Brasil");
        
        (*pd)++;
        tamanho_lista(arv->linhas, &t);
        (*p) += t;

        imprimir_indice_(arv->esq, fp, p, pd);
        fprintf(fp, "%s ", arv->pal);
        imprimir_lista(arv->linhas, fp);
        imprimir_indice_(arv->dir, fp, p, pd);
    }
}

