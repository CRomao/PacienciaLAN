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
    char visivel; //1 - oculta 0 - visivel
    char cor;
    struct carta *ant;
    struct carta *prox;
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
    struct historico *ant;
    struct historico *prox;
    int pilhaOrigem;
    TPCarta *carta;
};
typedef struct historico TPHistorico;

int main(int argc, char** argv) {
    TPCarta cartas[52];
    TPPilhaCarta PilhasC[7];
    
    iniciarCartas(&cartas);
    embaralharCartas(&cartas);
    inicializarCabecaPilhas(&PilhasC);
    distribuicaoInicial(&PilhasC, &cartas);
    imprimir(&PilhasC);
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
            cartas[cont].visivel = 'N';
            cartas[cont].valor = (j+1);
            cartas[cont].cor = cor[i];
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
        pilhasCarta[i].carta->visivel = 'N';
        pilhasCarta[i].carta->valor = 0;
    }
}

void distribuicaoInicial(TPPilhaCarta *pilhasCarta, TPCarta *cartas){
    //falta fazer o apontamento dos anteriores - apontamentos feitos :) 
    int tam = 51, i, lim = 7, j=0, cont=1;
    TPCarta *aux, *aux2;
    aux = malloc(sizeof(TPCarta));
    aux2 = malloc(sizeof(TPCarta));
    for(i=0; i<lim; i++){
        aux = pilhasCarta[i].carta;
        while(j < cont){
            aux2 = aux;
            aux->prox = &cartas[tam];
            aux = aux->prox;
            aux->ant = aux2;
            tam--;
            j++;
        }
        cont++;
        j=0;
        aux->visivel = 'S';
    }
}

void imprimir(TPPilhaCarta *pilhasCarta){
    TPCarta *aux;
    int lim = 7, i;
    aux = malloc(sizeof(TPCarta));
    for(i=0; i<lim; i++){
        aux = pilhasCarta[i].carta->prox;
        while(aux != NULL){
             if(aux->visivel == 'S'){
                printf("%d%c  ", aux->valor, aux->naipe);
            }else{
                printf("* ");
            }
            aux = aux->prox;
        }
        printf("\n");
    }
}