// checa para saber se é windows OS
// _WIN32 macro
#ifdef _WIN32
    #define limpar "cls"
  
// checa para saber se é linux OS 
// __linux__ macro
#elif __linux__
    #define limpar "clear"
  
#endif

#define TRUE 1
#define MAX 100
#define MAXLIN 10000
#define MAXOP 8

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include "avl.h"

void to_lower(char*);

int main()
{
  int op = 0, i = 0, linha = 1, criado = 0;
  char diretorio[MAX], *palavra = malloc(MAX), *conteudo_linha = malloc(MAXLIN);
  FILE* fp = NULL;
  Arv ir = NULL;
  struct item item;
  clock_t inicio, fim;

  setlocale(LC_ALL, "Portuguese_Brasil");

  do{
      do{
          printf("LISTA DE OPCOES:\n\n");
          printf("[1] Criar índice nulo.\n");
          printf("[2] Criar índice por arquivo.\n");
          printf("[3] Inserir item (palavra e página).\n");
          printf("[4] Imprimir índice.\n");
          printf("[5] Descarregar índice em arquivo.\n");
          printf("[6] Esvaziar índice (deixar na condição de índice vazio).\n");
          printf("[7] Destruir índice.\n");
          printf("[8] Sair do sistema.\n");
          printf("\nDigite uma das opcoes: ");
          
          scanf("%d", &op);
          system(limpar);
          setbuf(stdin, NULL);
          
          if(op < 1 || op > MAXOP){
              printf("Opcao invalida! Digite novamente!\n\n");
          }
      } while(op < 1 || op > MAXOP);
      
      if(op == MAXOP){
          break;
      }
      
      switch(op){
          case 1:
            if(criado){
              printf("Destrua o índice atual primeiro!\n\n");
              break;
            }
            ir = cria_arv_vazia();
            criado = 1;
            printf("Índice nulo criado com sucesso.\n\n");
            break;

          case 2:
            if(criado){
              printf("Destrua o índice atual primeiro!\n\n");
              break;
            }

            printf("Entre com o diretório do texto: ");
            scanf("%s", diretorio);
            fp = fopen(diretorio, "r");
            if(fp == NULL){
              printf("Erro ao abrir arquivo!\n\n");
              break;
            }

            ir = cria_arv_vazia();
            criado = 1;
            linha = 1;
            inicio = clock();

            while(fscanf(fp, "%[^\n] ", conteudo_linha) != EOF && linha) {
              palavra = strtok(conteudo_linha, " ,.;!?/\n—-");
              while(palavra != NULL){
                to_lower(palavra);
                strcpy(item.pal, palavra);
                item.linha = linha;
                if(!insere_arv(&ir, &item)){
                  printf("Houve algum erro ao inserir alguma palavra!\n\n");
                  apaga_arv(&ir);
                  linha = 0;
                  break;
                }
                palavra = strtok(NULL, " ,.;!?/\n—-");
              }
              linha++;
            }
            fim = clock();
            fim = (double)(fim - inicio)/CLOCKS_PER_SEC;
            fclose(fp);

            printf("Índice criado com sucesso!\n\n");
            break;

          case 3:
            if(!criado){
              printf("Crie um índice primeiro!\n\n");
              break;
            }
            printf("Entre com a palavra do item: ");
            scanf("%s", item.pal);
            printf("Entre com a página do item: ");
            scanf("%d", &item.linha);
            to_lower(item.pal);
            if(!insere_arv(&ir, &item)){
              printf("Houve algum erro ao inserir a palavra!\n\n");
              apaga_arv(&ir);
              break;
            }
            printf("Palavra inserida com sucesso!\n\n");
            break;

          case 4:
            if(!criado){
              printf("Crie um índice primeiro!\n\n");
              break;
            }

            imprimir_indice(ir, stdout, fim);
            printf("\n\n");
            break;

          case 5:
            if(!criado){
              printf("Crie um índice primeiro!\n\n");
              break;
            }

            fp = fopen("indice.txt", "w");
            if(!imprimir_indice(ir, fp, fim))
              printf("Erro ao abrir arquivo!\n\n");
            else
              printf("Índice guardado no arquivo com sucesso!\n\n");

            fclose(fp);
            break;

          case 6:
            if(!criado){
              printf("Crie um índice primeiro!\n\n");
              break;
            }

            esvazia_arv(&ir);
            printf("Índice atual esvaziado com sucesso!\n\n");
            break;

          case 7:
            if(!criado){
              printf("Crie um índice primeiro!\n\n");
              break;
            }

            criado = 0;
            apaga_arv(&ir);
            printf("Índice atual apagado com sucesso!\n\n");
            break;
      }  

  } while(op != MAXOP);
  
  system(limpar);
  printf("\nFIM DO PROGRAMA\n");
  return 0;
}

void to_lower(char* str){
  for(int i = 0; str[i]; i++)
    str[i] = tolower(str[i]);
}