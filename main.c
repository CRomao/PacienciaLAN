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
    int oculta;
    char cor;
    struct carta *ant;
    struct carta *prox;
};
typedef struct carta TPCarta;

struct pilhaCarta{
    TPCarta carta;
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
    TPCarta cartas[52];
    TPPilhaCarta PilhasC[7];
    iniciarCartas(&cartas);
    
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

void embaralharCartas(){
    
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
            cont++;
        }
    }
}
 
