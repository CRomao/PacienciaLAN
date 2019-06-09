#include <stdio.h>
#include <stdlib.h>
// Impressao das Pilhas de 1 a 7
void imprimir(TPPilhaCarta *pilhasCarta){
    TPCarta *aux;
    int lim = 7, i;
    for(i=0; i<lim; i++){
        aux = pilhasCarta[i].carta->prox;
        if(aux == NULL){
            printf("   ___\n" );
            printf("  | K |\n");
            printf("%d |___|\n", i+1);
        }else{
          while(aux != NULL){
              printf("   ___ " );
              aux = aux->prox;
          }
          printf("\n");
          //imprimir os naipes das cartas das pilhas
          aux = pilhasCarta[i].carta->prox;
          while(aux != NULL){
              if(aux->visivel == 'S'){
                  if(aux->naipe == 'O' || aux->naipe == 'C'){
                      printf("  |\033[31m %c\033[39m |", aux->naipe);
                  }else{
                      printf("  | %c |", aux->naipe);
                  }
              }else{
                  printf("  | * |");
              }

              aux = aux->prox;
          }
          printf("\n");

          printf("%d", (i+1));
          //imprimir os valores da pilhas
          aux = pilhasCarta[i].carta->prox;
          while(aux != NULL){
              if(qtdDigitos(aux->valor) == 1){
                  if(aux->visivel == 'S'){
                      if(aux->naipe == 'O' || aux->naipe == 'C'){
                        printf(" |_\033[31m%d\033[39m_| ", (char)aux->valor);
                      }else{
                        printf(" |_%d_| ", aux->valor);  
                      }
                  }else{
                      printf(" |_*_| ");
                  }
              }else{
                  if(aux->visivel == 'S'){
                      if(aux->naipe == 'O' || aux->naipe == 'C'){
                            printf(" |_\033[31m%d\033[39m| ", (char)aux->valor);
                      }else{
                            printf(" |_%d| ", aux->valor);
                      }
                  }else{
                      printf(" |_*_| ");
                  }
              }
              aux = aux->prox;
          }
          printf("\n");
      }
    }
} 