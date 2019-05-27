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
    printf("1: %p\n", &PilhasC);
    inicializarCabecaPilhas(&PilhasC);
    distribuicaoInicial(&PilhasC, &cartas);
    imprimir(&PilhasC);
    int i,j;
    
        /*for(j=0; j<LIN; j++){
            PilhasC[0].carta[j].valor = 1;
            PilhasC[1].carta[j].valor = 2;
            PilhasC[2].carta[j].valor = 3;
            PilhasC[3].carta[j].valor = 4;
            PilhasC[4].carta[j].valor = 5;
            PilhasC[5].carta[j].valor = 6;
            PilhasC[6].carta[j].valor = 7;
            printf("%d\t", PilhasC[0].carta[j].valor);
            printf("%d\t", PilhasC[1].carta[j].valor);
            printf("%d\t", PilhasC[2].carta[j].valor);
            printf("%d\t", PilhasC[3].carta[j].valor);
            printf("%d\t", PilhasC[4].carta[j].valor);
            printf("%d\t", PilhasC[5].carta[j].valor);
            printf("%d\t", PilhasC[6].carta[j].valor);
            printf("\n");
        }*/
    
    return (EXIT_SUCCESS);
}

void embaralharCartas(TPCarta *cartas, TPCarta *cartasEmb){
    int aux[52], cont = 0;
    for(int i=0; i<52; i++){
        aux[i] = -1;
    }
    while(cont != 52){
        
        
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
    printf("2: %p\n", pilhasCarta);
    printf("3: %p\n", *(pilhasCarta));
//    printf("4: %p\n", *(*(pilhasCarta)));
    int i;
    for(i=0; i<7; i++){
        pilhasCarta[i].carta = malloc(sizeof(TPCarta));
        pilhasCarta[i].carta->ant = NULL;
        pilhasCarta[i].carta->prox = NULL;
        pilhasCarta[i].carta->cor = 'N';
        pilhasCarta[i].carta->naipe = 'N';
        pilhasCarta[i].carta->oculta = -1;
        pilhasCarta[i].carta->valor = 0;
        pilhasCarta[i].carta->indice = -1;
    }
}

void distribuicaoInicial(TPPilhaCarta *pilhasCarta, TPCarta *cartas){
    int i= 1, tam = 51;
    TPCarta *aux;
    aux = malloc(sizeof(TPCarta));
    //Pilha 1
    aux = pilhasCarta[0].carta;
    while(i < 2){
        aux->prox = &cartas[tam];
        aux = aux->prox;
        tam--;
        i++;
    }
    i = 1;
    //Pilha 2
    aux = pilhasCarta[1].carta;
    while(i < 3){
        aux->prox = &cartas[tam];
        aux = aux->prox;
        tam--;
        i++;
    }
    
    //Pilha 3
    i = 1;
    aux = pilhasCarta[2].carta;
    while(i < 4){
        aux->prox = &cartas[tam];
        aux = aux->prox;
        tam--;
        i++;
    }
    
    //Pilha 4
    i = 1;
    aux = pilhasCarta[3].carta;
    while(i < 5){
        aux->prox = &cartas[tam];
        aux = aux->prox;
        tam--;
        i++;
    }
    //Pilha 5
    i = 1;
    aux = pilhasCarta[4].carta;
    while(i < 6){
        aux->prox = &cartas[tam];
        aux = aux->prox;
        tam--;
        i++;
    }
    //Pilha 6
    i = 1;
    aux = pilhasCarta[5].carta;
    while(i < 7){
        aux->prox = &cartas[tam];
        aux = aux->prox;
        tam--;
        i++;
    }
    //Pilha 7
    i = 1;
    aux = pilhasCarta[6].carta;
    while(i < 8){
        aux->prox = &cartas[tam];
        aux = aux->prox;
        tam--;
        i++;
    }
}


void imprimir(TPPilhaCarta *pilhasCarta){
    TPCarta *aux;
    aux = malloc(sizeof(TPCarta));
    aux = pilhasCarta[0].carta->prox;
    //Pilha 1
    while(aux != NULL){
        printf("%d%c  ", aux->valor, aux->naipe);
        aux = aux->prox;
    }
    printf("\n");
    
    //Pilha 2
    aux = pilhasCarta[1].carta->prox;
    while(aux != NULL){
        printf("%d%c  ", aux->valor, aux->naipe);
        aux = aux->prox;
    }
    printf("\n");
    
    //Pilha 3
    aux = pilhasCarta[2].carta->prox;
    while(aux != NULL){
        printf("%d%c  ", aux->valor, aux->naipe);
        aux = aux->prox;
    }
    printf("\n");
    
    //Pilha 4
    aux = pilhasCarta[3].carta->prox;
    while(aux != NULL){
        printf("%d%c  ", aux->valor, aux->naipe);
        aux = aux->prox;
    }
    printf("\n");
    
    //Pilha 5
    aux = pilhasCarta[4].carta->prox;
    while(aux != NULL){
        printf("%d%c  ", aux->valor, aux->naipe);
        aux = aux->prox;
    }
    printf("\n");
    
    //Pilha 6
    aux = pilhasCarta[5].carta->prox;
    while(aux != NULL){
        printf("%d%c  ", aux->valor, aux->naipe);
        aux = aux->prox;
    }
    printf("\n");
    
    //Pilha 7
    aux = pilhasCarta[6].carta->prox;
    while(aux != NULL){
        printf("%d%c  ", aux->valor, aux->naipe);
        aux = aux->prox;
    }
    printf("\n");
}