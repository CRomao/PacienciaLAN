/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Romão
 *
 * Created on 25 de Maio de 2019, 19:36
 */

#include <stdio.h>
#include <stdlib.h>
#define LIN 19
#define COL 7

struct carta{
    int valor;
    char naipe;
    int oculta; //1 - oculta 0 - visivel
    char cor;
    struct carta *ant;
    struct carta *prox;
    int indice;
};
typedef struct carta TPCarta;

struct pilhaCarta{
    TPCarta *carta;
};
typedef struct  pilhaCarta TPPilhaCarta;

struct jogador{
    char nome[100];
    int tempo;
    int pont;
    int mov;
};
typedef struct jogador TPJogador;

struct historico{
    TPCarta carta;
    struct historico *ant;
    struct historico *prox;
};
typedef struct historico TPHistorico;

int main(int argc, char** argv) {
    TPCarta cartas[52], cartasEmb[52];
    
    TPPilhaCarta PilhasC[7];
    iniciarCartas(&cartas);
    //imp(&cartas);
   // printf("\nDEPOIS\n");
    embaralharCartas(&cartas);
   // imp(&cartas);
    //printf("1: %p\n", &PilhasC);
    inicializarCabecaPilhas(&PilhasC);
    distribuicaoInicial(&PilhasC, &cartas);
    imprimir(&PilhasC);
    int i,j;
    
 

    return (EXIT_SUCCESS);
}

void embaralharCartas(TPCarta *cartas){
    srand(time(NULL));
    int i, pos;
    TPCarta aux;
     for(i=0;i<51;i++) {
      pos = i + rand()%(52 - i);
      aux = cartas[i];
      cartas[i] = cartas[pos];
      cartas[pos] = aux;
   }
}

void iniciarCartas(TPCarta *cartas){
    int i,j, cont = 0;
    char naipes[4] = {'O', 'C', 'P', 'E'};
    char cor[4] = {'V', 'V', 'P', 'P'};
    for(i=0; i<4; i++){
        for(j=0; j<13; j++){
            cartas[cont].ant = NULL;
            cartas[cont].prox = NULL;
            cartas[cont].naipe = naipes[i];
            cartas[cont].oculta = 1;
            cartas[cont].valor = (j+1);
            cartas[cont].cor = cor[i];
            cartas[cont].indice = cont;
            cont++;
        }
    }
}
 
void inicializarCabecaPilhas(TPPilhaCarta *pilhasCarta){
    int i;
    TPCarta *aux;
    aux = malloc(sizeof(TPCarta));
    for(i=0; i<7; i++){
        pilhasCarta[i].carta = malloc(sizeof(TPCarta));
        pilhasCarta[i].carta->ant = NULL;
        pilhasCarta[i].carta->prox = aux;
        pilhasCarta[i].carta->cor = 'N';
        pilhasCarta[i].carta->naipe = 'N';
        pilhasCarta[i].carta->oculta = -1;
        pilhasCarta[i].carta->valor = 0;
        pilhasCarta[i].carta->indice = -1;
    }
}

void distribuicaoInicial(TPPilhaCarta *pilhasCarta, TPCarta *cartas){
    int tam = 51, i, lim = 7, j=0, cont=1;
    TPCarta *aux;
    aux = malloc(sizeof(TPCarta));
    
    for(i=0; i<lim; i++){
        aux = pilhasCarta[i].carta;
        while(j < cont){
            aux->prox = &cartas[tam];
            aux = aux->prox;
            tam--;
            j++;
        }
        cont++;
        j=0;
        aux->oculta = 0;
    }
}


void imprimir(TPPilhaCarta *pilhasCarta){
    TPCarta *aux;
    int lim = 7, i;
    aux = malloc(sizeof(TPCarta));
    for(i=0; i<lim; i++){
        aux = pilhasCarta[i].carta->prox;
        while(aux != NULL){
             if(aux->oculta == 0){
                printf("%d%c  ", aux->valor, aux->naipe);
            }else{
                printf("* ");
            }
            aux = aux->prox;
        }
        printf("\n");
    }
}



void imp(TPCarta *cartas){
    for(int i=0; i<52; i++){
        if((i+1) % 13 == 0){
            if(cartas[i].oculta == 0){
                printf("%d%c ", cartas[i].valor, cartas[i].naipe);
            }else{
                printf("* ");
            }
            
        }else{
            if(cartas[i].oculta == 0){
                printf("%d%c ", cartas[i].valor, cartas[i].naipe);
            }else{
                printf("* ");
            }
            
        }
    }
}