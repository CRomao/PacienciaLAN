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
int validarMovimento(int valorCartaMovida, int valorCartaAcima, char corCartaMovida, char corCartaAcima);
struct carta{
    int valor;
    char naipe;
    char visivel;
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
    int pilhaOrigem; // vai servir para saber em qual pilha estava, caso a jogada seja desfeita
    //ver se pode ser um inteiro ou um "TPPilhaCarta *pilhaOrigem"
    int pilhaDestino;
    TPCarta *carta;
    TPCarta * cartaAnterior;
};
typedef struct historico TPHistorico;

int main(int argc, char** argv) {
    TPCarta cartas[52];
    TPPilhaCarta PilhasC[7], estoqueC, descarteC, montanteC[4];
    TPHistorico movimento;
    printf("MAIN LISTAMOV: %p\n", &movimento);
    int pOrigem, pDestino, valorC, contMovimentos = 0, opc;
    iniciarCartas(&cartas);
    embaralharCartas(&cartas);
    inicializarCabecaPilhas(&PilhasC);
    inicializarHistorico(&movimento);
    distribuicaoInicial(&PilhasC, &cartas);
    PilhasC[0].carta->prox->valor = 11;
    PilhasC[0].carta->prox->cor = 'P';
    PilhasC[0].carta->prox->naipe = 'E';
    PilhasC[4].carta->prox->prox->prox->prox->prox->valor = 12;
    PilhasC[4].carta->prox->prox->prox->prox->prox->cor = 'V';
    PilhasC[4].carta->prox->prox->prox->prox->prox->naipe = 'O';
    
    PilhasC[4].carta->prox->prox->prox->prox->valor = 4;
    PilhasC[4].carta->prox->prox->prox->prox->cor = 'P';
    PilhasC[4].carta->prox->prox->prox->prox->naipe = 'E';
    
    PilhasC[3].carta->prox->prox->prox->prox->valor = 13;
    PilhasC[3].carta->prox->prox->prox->prox->cor = 'P';
    PilhasC[3].carta->prox->prox->prox->prox->naipe = 'E';
    
    PilhasC[6].carta->prox->prox->prox->prox->prox->prox->prox->valor = 3;
    PilhasC[6].carta->prox->prox->prox->prox->prox->prox->prox->cor = 'P';
    PilhasC[6].carta->prox->prox->prox->prox->prox->prox->prox->naipe = 'P';
    PilhasC[6].carta->prox->prox->prox->prox->prox->valor = 3;
    PilhasC[6].carta->prox->prox->prox->prox->prox->cor = 'V';
    PilhasC[6].carta->prox->prox->prox->prox->prox->naipe = 'C';
    
    PilhasC[2].carta->prox->prox->prox->valor = 4;
    PilhasC[2].carta->prox->prox->prox->cor = 'V';
    PilhasC[2].carta->prox->prox->prox->naipe = 'C';
    
    
    imprimirTemp(&PilhasC);
    imprimir(&PilhasC);
    
    while(1){
        printf("Escolha uma opção: \n");
        printf("1 - Fazer Movimento Pilha para Pilha\n");
        printf("2 - Desfazer movimento\n");
        scanf("%d", &opc);
        switch(opc){
            case 1:
                printf("\nInforme a pilha de origem: ");
                scanf("%d", &pOrigem);
                printf("\nInforme o valor da carta: ");
                scanf("%d", &valorC);
                printf("\nInforme a pilha de destino: ");
                scanf("%d", &pDestino);
                fazerMovimentoPP(pOrigem, valorC, pDestino, &PilhasC, &movimento, &contMovimentos);
                imprimir(&PilhasC);
                break;
            case 2:
                desfazerMovimento(&PilhasC, &movimento, &contMovimentos);
                imprimir(&PilhasC);
                break;
        }
        printf("\nCONTMOV: %d\n", contMovimentos);
        
    }
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
        pilhasCarta[i].carta->valor = -1;
    }
}

void distribuicaoInicial(TPPilhaCarta *pilhasCarta, TPCarta *cartas){
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
void distribuicaoInicialEstoque(TPPilhaCarta *estoque, TPCarta *cartas){
    
    
}


void imprimir(TPPilhaCarta *pilhasCarta){
    TPCarta *aux;
    int lim = 7, i;
    aux = malloc(sizeof(TPCarta));
    for(i=0; i<lim; i++){
        aux = pilhasCarta[i].carta->prox;
        printf("%d - ",i+1);
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

void fazerMovimentoPP(int pilhaOrigem, int valorCarta, int pilhaDestino, TPPilhaCarta *pilhasCartas, TPHistorico *movimento, int *contMovimentos){
    pilhaOrigem--;
    pilhaDestino--;
    TPCarta *aux, *aux2, *aux3;
    TPHistorico *newMovimento;
    int validarMov;
    newMovimento = malloc(sizeof(TPHistorico));
    aux = malloc(sizeof(TPCarta));
    aux = pilhasCartas[pilhaOrigem].carta;
    //buscar carta da pilha de origem
    while(aux->prox != NULL){
        if(valorCarta == aux->valor && aux->visivel == 'S'){
            break;
        }
        aux = aux->prox;
    }
    //Buscar carta da pilha de destino
    aux3 = pilhasCartas[pilhaDestino].carta;
    while(aux3->prox != NULL){
        aux3 = aux3->prox;
    }
    aux2 = aux->ant; // anterior da pilha de origem
    newMovimento->cartaAnterior = aux2;
    validarMov = validarMovimento(aux->valor, aux3->valor, aux->cor, aux3->cor);
    if(validarMov == 0 || validarMov == 1){    
        aux2->prox = NULL;
        aux2->visivel = 'S';
        aux->ant = NULL;
        //atribuição a nova pilha
        aux3->prox = aux;
        aux->ant = aux3;
        //Pegar dados desse movimento para jogar na função de registrar no histórico
        newMovimento->pilhaOrigem = pilhaOrigem;
        newMovimento->pilhaDestino = pilhaDestino;
        newMovimento->carta = aux;
        newMovimento->prox = NULL;
        newMovimento->ant = NULL;
        printf("LISTAMOV: %p\n", movimento);
        printf("*LISTAMOV: %p\n", *movimento);
        printf("NEWMOV: %p\n", newMovimento);
        registrarMovimento(newMovimento, movimento);
        (*contMovimentos)++;
    }else{
        printf("\nMovimento inválido\n");
    }
}

void desfazerMovimento(TPPilhaCarta *pilhasCartas, TPHistorico *historico, int *contMovimentos){
    TPHistorico *aux, *anteriorAux;
    TPCarta *destino, *auxDestino;
    aux = historico;
    while(aux->prox != NULL){ // Buscar o historico em cima da pilha (o ultimo movimento feito)
        aux = aux->prox;
    }
    
    destino =  pilhasCartas[aux->pilhaDestino].carta;
    while(destino != aux->carta){ // Vai pegar a carta que tinha sido movinmentada nesse movimento
        destino = destino->prox;
    }
    auxDestino = destino->ant; // a carta que tinha recebido a carta movimentada
    auxDestino->prox = NULL; // o proximo da carta que recebeu, recebe NULO;
    destino->ant = NULL; // o anterior da carta movimentada recebe NULO;
    
    aux->cartaAnterior->prox = destino; // Vai colocar a carta na carta em que ela estava abaixo;
    destino->ant = aux->cartaAnterior; // Vai apontar o anterior da carta com a carta anterior
    
    // Após isso, retirar da pilha de movimentos, esse movimento que foi desfeito
    anteriorAux = aux->ant;
    anteriorAux->prox = NULL;
    free(aux);
    (*contMovimentos)++;
    
}

void fazerMovimentoPM(){
    
}

void fazerMovimentoDP(){
    
}

void fazerMovimentoDM(){
    
}

void registrarMovimento(TPHistorico *newMovimento, TPHistorico *movimento){
    TPHistorico *aux, *aux2;
    aux = malloc(sizeof(TPHistorico));
    aux = movimento;
    aux2 = malloc(sizeof(TPHistorico)); // anterior
    while(aux->prox != NULL) aux = aux->prox;
    
    aux->prox = newMovimento;
    newMovimento->ant = aux;
}

void inicializarHistorico(TPHistorico *historico){
    historico->ant = NULL;
    historico->carta = NULL;
    historico->pilhaOrigem = -1;
    historico->pilhaDestino = -1;
    historico->prox = NULL;
    historico->cartaAnterior = NULL;
}




void regrasJogo(){
    
}

void sairJogo(){
    
}


void rankJogo(){
    
}

void validarcampo(int campo, char tipo){
    if(tipo = 'P'){
        if(campo <= 7){
            return 0;
        }else{
            printf("Valor inválido!\n");
            return -1;
        }
    }else if(tipo == 'C'){
        if(campo <= 13){
            return 0;
        }else{
            printf("Vslor inválido!\n");
            return -1;
        }
    }
}





void imprimirTemp(TPPilhaCarta *pilhasCarta){
    TPCarta *aux;
    int lim = 7, i;
    aux = malloc(sizeof(TPCarta));
    for(i=0; i<lim; i++){
        aux = pilhasCarta[i].carta->prox;
        while(aux != NULL){
                printf("%d%c  ", aux->valor, aux->naipe);
            aux = aux->prox;
        }
        printf("\n");
    }
}


int validarMovimento(int valorCartaMovida, int valorCartaAcima, char corCartaMovida, char corCartaAcima){
    if(valorCartaMovida == 13 && valorCartaAcima == -1){ // de 13 para espaço vazio
        return 0; //  movimento válido
    }else if(((valorCartaMovida +1) == valorCartaAcima) && (corCartaMovida != corCartaAcima)){
        return 1; // movimento válido
    }else{
        return 2;
    }
    return 3;
}