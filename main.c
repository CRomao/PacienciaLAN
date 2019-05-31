/* 
 * File:   main.c
 * Author: Romão
 *
 * Created on 25 de Maio de 2019, 19:36
 */

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#define LIN 19
#define COL 7

struct carta{
    int valor;
    char naipe;
    char visivel;
    char cor;
    char simbolo;
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
   // int pilhaOrigem;
   // int pilhaDestino;
    char visivelCartaAnterior;
    char movEspecial;
    TPCarta *carta;
    TPCarta * cartaAnterior;
};
typedef struct historico TPHistorico;

int validarMovimento(int valorCartaMovida, int valorCartaAcima, char corCartaMovida, char corCartaAcima, char naipeCartaMovida, char naipeCartaAcima, int opc);
int buscarCarta(TPCarta *carta);
void imprimir(TPPilhaCarta *pilhasCarta);
void embaralharCartas(TPCarta *cartas);
void iniciarCartas(TPCarta *cartas);
void inicializarCabecaPilhas(TPPilhaCarta *pilhasCarta);
void inicializarHistorico(TPHistorico *historico);
void distribuicaoInicial(TPPilhaCarta *pilhasCarta, TPCarta *cartas);
void distribuicaoInicialEstoque(TPPilhaCarta *estoque, TPCarta *cartas);
void desfazerMovimento(TPPilhaCarta *pilhasCartas, TPHistorico *historico, int *contMovimentos);
void registrarMovimento(TPHistorico *newMovimento, TPHistorico *movimento);
void fazerMovimentoEstoqueDescarte(TPPilhaCarta *estoque, TPPilhaCarta *descarte, TPHistorico *movimento);
void fazerMovimentoPP(int pilhaOrigem, int valorCarta, int pilhaDestino, TPPilhaCarta *pilhasCartas, TPHistorico *movimento, int *contMovimentos, int opc);
void reporEstoque(TPPilhaCarta *estoque, TPPilhaCarta *descarte);
void validarcampo(int campo, char tipo);
void imprimirTemp(TPPilhaCarta *pilhasCarta);
int qtdDigitos(int valor);
int condicaoVitoria(TPPilhaCarta *pilhasCartas);
int main(int argc, char** argv) {
    TPCarta cartas[52];
    TPPilhaCarta PilhasC[13];
    TPHistorico movimento;
    int pOrigem, pDestino, valorC, contMovimentos = 0, opc;
    iniciarCartas(&cartas);
    embaralharCartas(&cartas);
    inicializarCabecaPilhas(&PilhasC);
    distribuicaoInicial(&PilhasC, &cartas);
    distribuicaoInicialEstoque(&PilhasC[7], &cartas);
    inicializarHistorico(&movimento);
    
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
    
    while(1){
        //imprimirEstoque(&PilhasC[7]);
        //imprimirDescarte(&PilhasC[8]);
        imprimirTemp(&PilhasC);
        imprimir(&PilhasC);
        //imprimirMontagem(&PilhasC);
        printf("Escolha a operação: \n");
        printf("1 - Mover | ");
        printf("2 - Desfazer movimento | ");
        printf("3 - Próxima carta do Estoque |\n--> ");
        scanf("%d", &opc);
        switch(opc){
            case 1:
                printf("1 - Mover de DESCARTE para MONTAGEM | ");
                printf("2 - Mover de DESCARTE para PILHA | ");
                printf("3 - Mover de PILHA para MONTAGEM | ");
                printf("4 - Mover de PILHA para PILHA |\n-->");
                scanf("%d", &opc);
                switch(opc){
                    case 1:
                        printf("Informe para qual MONTAGEM quer mover: ");
                        scanf("%d", &pDestino);
                        valorC = buscarCarta(&PilhasC[8].carta);
                        fazerMovimentoPP(9, valorC, pDestino, &PilhasC, &movimento, &contMovimentos, 2);
                        break;
                    case 2:
                        printf("Informe para qual PILHA quer mover: ");
                        scanf("%d", &pDestino);
                        valorC = buscarCarta(&PilhasC[8].carta);
                        fazerMovimentoPP(9, valorC, pDestino, &PilhasC, &movimento, &contMovimentos, 1);
                        break;
                    case 3:
                        printf("\nInforme a PILHA que tem a carta: ");
                        scanf("%d", &pOrigem);
                        printf("\nInforme o VALOR da carta: ");
                        scanf("%d", &valorC);
                        printf("\nInforme a MONTAGEM que vai receber a carta: ");
                        scanf("%d", &pDestino);
                        fazerMovimentoPP(pOrigem, valorC, pDestino, &PilhasC, &movimento, &contMovimentos, 2);
                        break;
                    case 4:
                        printf("\nInforme a PILHA que tem a carta: ");
                        scanf("%d", &pOrigem);
                        printf("\nInforme o VALOR da carta: ");
                        scanf("%d", &valorC);
                        printf("\nInforme a PILHA que vai receber a carta: ");
                        scanf("%d", &pDestino);
                        fazerMovimentoPP(pOrigem, valorC, pDestino, &PilhasC, &movimento, &contMovimentos, 1);
                        imprimir(&PilhasC);
                }
                break;
            case 2:
                desfazerMovimento(&PilhasC, &movimento, &contMovimentos);
                break;
            case 3:
                fazerMovimentoEstoqueDescarte(&PilhasC[7], &PilhasC[8], &movimento);
                break;
        }
        //printf("\nCONTMOV: %d\n", contMovimentos);
        if(condicaoVitoria(&PilhasC) == 52){
            printf("\n\nPARABÉNS, VOCÊ TERMINOU!!!\n\n");
            break;
        }else{
            printf("\e[1;1H\e[2J");
        }
        
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
    char simbolo[4] = {'♥', '♦', '♠', '♣'};
    for(i=0; i<4; i++){
        for(j=0; j<13; j++){
            cartas[cont].ant = NULL;
            cartas[cont].prox = NULL;
            cartas[cont].naipe = naipes[i];
            cartas[cont].visivel = 'N';
            cartas[cont].valor = (j+1);
            cartas[cont].cor = cor[i];
            cartas[cont].simbolo = simbolo[i];
            cont++;
        }
    }
}
 
void inicializarCabecaPilhas(TPPilhaCarta *pilhasCarta){
    int i;
    TPCarta *aux;
    aux = malloc(sizeof(TPCarta));
    for(i=0; i<13; i++){
        pilhasCarta[i].carta = malloc(sizeof(TPCarta));
        pilhasCarta[i].carta->ant = NULL;
        pilhasCarta[i].carta->prox = NULL;
        pilhasCarta[i].carta->cor = 'N';
        pilhasCarta[i].carta->naipe = 'N';
        pilhasCarta[i].carta->visivel = 'N';
        pilhasCarta[i].carta->valor = -1;
    }
}

void distribuicaoInicial(TPPilhaCarta *pilhasCarta, TPCarta *cartas){
    int tam = 51, i, lim = 7, j=0, cont=1;
    TPCarta *aux, *aux2;
    for(i=0; i<lim; i++){
        aux = pilhasCarta[i].carta;
        while(j < cont){ // controlar a quantidade de cartas por pilhas
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
    int tam = 24;
    TPCarta *auxEstoque, *anterior;

    for(int i=0; i<tam; i++){
        auxEstoque = estoque->carta;
        while(auxEstoque->prox != NULL){
            auxEstoque = auxEstoque->prox;
        }
        anterior = auxEstoque; // fazer o apontamento dos anteriores
        auxEstoque->prox = &cartas[i];
        auxEstoque->prox->ant = anterior;
        
    }
}


void imprimir(TPPilhaCarta *pilhasCarta){
    TPCarta *aux;
    int lim = 7, i;
    for(i=0; i<lim; i++){
        aux = pilhasCarta[i].carta->prox;
        if(aux == NULL){
            printf("  ___\n" );
            printf(" | K |\n");
            printf(" |___|\n");
        }else{
        //  printf("%d - \n",i+1);
          while(aux != NULL){
              printf("  ___ " );
              aux = aux->prox;
          }
          printf("\n");


          aux = pilhasCarta[i].carta->prox;
          while(aux != NULL){
              if(aux->visivel == 'S'){
                  if(aux->naipe == 'O' || aux->naipe == 'C'){
                      printf(" |\033[31m %c\033[39m |", aux->naipe);
                  }else{
                      printf(" | %c |", aux->naipe);
                  }
              }else{
                  printf(" | * |");
              }

              aux = aux->prox;
          }
          printf("\n");


          aux = pilhasCarta[i].carta->prox;
          while(aux != NULL){
              if(qtdDigitos(aux->valor) == 1){
                  if(aux->visivel == 'S'){
                      if(aux->naipe == 'O' || aux->naipe == 'C'){
                        printf(" |_\033[31m%d\033[39m_|", (char)aux->valor);
                      }else{
                        printf(" |_%d_|", aux->valor);  
                      }
                  }else{
                      printf(" |_*_|");
                  }
              }else{
                  if(aux->visivel == 'S'){
                      if(aux->naipe == 'O' || aux->naipe == 'C'){
                            printf(" |_\033[31m%d\033[39m|", (char)aux->valor);
                      }else{
                            printf(" |_%d|", aux->valor);
                      }
                  }else{
                      printf(" |_*_|");
                  }
              }
              aux = aux->prox;
          }
          printf("\n");
      }
    }
}

int qtdDigitos(int valor){
    int cont=0;
    while(valor != 0){
        valor = valor/10;
        cont++;
    }
    return cont;
}

void fazerMovimentoPP(int pilhaOrigem, int valorCarta, int pilhaDestino, TPPilhaCarta *pilhasCartas, TPHistorico *movimento, int *contMovimentos, int opc){
    pilhaOrigem--;
    pilhaDestino--;
    //verificar se o descarte está vazio
    if(pilhasCartas[pilhaOrigem].carta->prox == NULL){
        printf("O descarte está vazio (DICA: puxe a próxima carta do estoque :) )\n");
        return ;
    }
    TPCarta *aux, *aux2, *aux3;
    TPHistorico *newMovimento;
    int validarMov;
    newMovimento = malloc(sizeof(TPHistorico));
    aux = malloc(sizeof(TPCarta));
    aux = pilhasCartas[pilhaOrigem].carta;
    //buscar carta da pilha de origem
    if(pilhaOrigem > 6){ // pegando de montagem ou descarte
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        aux->visivel = 'S';
    }else{ // pegando de pilha normal
        while(aux->prox != NULL){
            if(valorCarta == aux->valor && aux->visivel == 'S'){
                break;
            }
            aux = aux->prox;
        }
    }
    
    //Buscar carta da pilha de destino
    aux3 = pilhasCartas[pilhaDestino].carta;
    while(aux3->prox != NULL){
        aux3 = aux3->prox;
    }
    
    aux2 = aux->ant; // anterior da pilha de origem
    newMovimento->cartaAnterior = aux2;
    newMovimento->visivelCartaAnterior = newMovimento->cartaAnterior->visivel;
    validarMov = validarMovimento(aux->valor, aux3->valor, aux->cor, aux3->cor, aux->naipe, aux3->naipe, opc);
    if(validarMov == 0 || validarMov == 1){    
        aux2->prox = NULL;
        aux2->visivel = 'S';
        //aux->ant = NULL; - tirar
        //atribuição a nova pilha
        aux3->prox = aux;
        aux->ant = aux3;
        //Pegar dados desse movimento para jogar na função de registrar no histórico
      //  newMovimento->pilhaOrigem = pilhaOrigem;
      //  newMovimento->pilhaDestino = pilhaDestino;
        newMovimento->carta = aux;
        newMovimento->prox = NULL;
        newMovimento->ant = NULL;
        newMovimento->movEspecial = 'N';
        registrarMovimento(newMovimento, movimento);
        (*contMovimentos)++;
    }else{
        printf("\nMovimento inválido\n");
    }
}

void desfazerMovimento(TPPilhaCarta *pilhasCartas, TPHistorico *historico, int *contMovimentos){
    if(historico->prox == NULL){
        printf("Ainda não consta nenhum movimento feito.\n");
    }else{
        TPHistorico *aux, *anteriorAux;
        TPCarta *destino, *auxDestino;
        aux = historico;
        while(aux->prox != NULL){ // Buscar o historico em cima da pilha (o ultimo movimento feito)
            aux = aux->prox;
        }
        if(aux->movEspecial == 'S'){ // corrigir o movimento de repor o estoque, e desfazer esse movimento
            reporEstoque(&pilhasCartas[8], &pilhasCartas[7]);
            anteriorAux = aux->ant;
            anteriorAux->prox = NULL;
            free(aux);
        }else{
            destino =  aux->carta;

            auxDestino = destino->ant; // a carta que tinha recebido a carta movimentada
            auxDestino->prox = NULL; // o proximo da carta que recebeu, recebe NULO;
            destino->ant = NULL; // o anterior da carta movimentada recebe NULO;

            aux->cartaAnterior->prox = destino; // Vai colocar a carta na carta em que ela estava abaixo;
            destino->ant = aux->cartaAnterior; // Vai apontar o anterior da carta com a carta anterior
            aux->cartaAnterior->visivel = aux->visivelCartaAnterior;
            // Após isso, retirar da pilha de movimentos, esse movimento que foi desfeito
            anteriorAux = aux->ant;
            anteriorAux->prox = NULL;
            free(aux);
            (*contMovimentos)++;
        }
    }
}

void registrarMovimento(TPHistorico *newMovimento, TPHistorico *movimento){
    TPHistorico *aux, *aux2;
    aux = movimento;
    while(aux->prox != NULL) aux = aux->prox;
    
    aux->prox = newMovimento;
    newMovimento->ant = aux;
}

void inicializarHistorico(TPHistorico *historico){
    historico->ant = NULL;
    historico->carta = NULL;
  //  historico->pilhaOrigem = -1;
  //  historico->pilhaDestino = -1;
    historico->prox = NULL;
    historico->cartaAnterior = NULL;
    historico->movEspecial = 'Z';
}

void fazerMovimentoEstoqueDescarte(TPPilhaCarta *estoque, TPPilhaCarta *descarte, TPHistorico *movimento){ // Função para passar a carta para o descarte
    if(estoque->carta->prox == NULL){
        reporEstoque(estoque, descarte);// Caso o estoque esteja vazio, chama a função de repor o estoque
        TPHistorico *newMovimentoEspecial; // e guarda esse movimento como especial
        newMovimentoEspecial = malloc(sizeof(TPHistorico));
        newMovimentoEspecial->cartaAnterior = NULL;
        newMovimentoEspecial->prox = NULL;
        newMovimentoEspecial->ant = NULL;
        newMovimentoEspecial->carta = NULL;
        newMovimentoEspecial->movEspecial = 'S';
     //   newMovimentoEspecial->pilhaDestino = -1;
     //   newMovimentoEspecial->pilhaOrigem = -1;
        newMovimentoEspecial->visivelCartaAnterior = 'N';
        registrarMovimento(newMovimentoEspecial, movimento);
    }else{
        TPCarta *aux, *auxDescarte, *auxEstoque, *auxMovimento;
        TPHistorico *newMovimento;
        newMovimento = malloc(sizeof(TPHistorico));

        aux = estoque->carta;
        while(aux->prox != NULL){ // Buscar a carta do estoque (a que está em cima)
            aux = aux->prox;
        }
        
        auxDescarte = descarte->carta;
        while(auxDescarte->prox != NULL){ // Buscar o prox NULL para jogar no descarte
            auxDescarte = auxDescarte->prox;
        }
        // Corrigindo o estoque após tirar a carta
        
        auxEstoque = aux->ant;
        newMovimento->cartaAnterior = aux->ant;
        newMovimento->visivelCartaAnterior = newMovimento->cartaAnterior->visivel;
        auxEstoque->prox = NULL;
        aux->ant = NULL;
        // Ajustando a carta para o descarte
        auxDescarte->prox = aux;
        aux->ant = auxDescarte; 
        auxDescarte->visivel = 'S';
        
   //     newMovimento->pilhaOrigem = 7;
    //    newMovimento->pilhaDestino = 8;
        newMovimento->carta = aux;
        newMovimento->prox = NULL;
        newMovimento->ant = NULL;
        newMovimento->movEspecial = 'N';
        registrarMovimento(newMovimento, movimento);
    }
}

void reporEstoque(TPPilhaCarta *estoque, TPPilhaCarta *descarte){ //Função para a reposição das cartas para o estoque
    TPCarta *reporEstoque, *estoqueFinal, *aux;
    int tam = 0;
    reporEstoque = descarte->carta;
    while(reporEstoque->prox != NULL){ // Pegar o o total de cartas que tem do descarte
        tam++;
        reporEstoque = reporEstoque->prox;
    }   
        
        for(int i=0; i<tam; i++){
            reporEstoque = descarte->carta;
            while(reporEstoque->prox != NULL){ // buscar a ultima carta do descarte (no caso a ultima passada para o descarte)
                reporEstoque = reporEstoque->prox;
            }
            
            estoqueFinal = estoque->carta;
            while(estoqueFinal->prox != NULL){ // buscar a carta com o PROX NULL para poder colocar no estoque
                estoqueFinal = estoqueFinal->prox;
            }
            
            //Reposição das cartas para o estoque
            aux = reporEstoque->ant;
            aux->prox = NULL;
            reporEstoque->ant = estoqueFinal;
            estoqueFinal->prox = reporEstoque;
        }
}

int buscarCarta(TPCarta *carta){
    TPCarta *aux;
    aux = carta;
    
    while(aux->prox != NULL){
        aux = aux->prox;
    }
    return aux->valor;
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
    int valorDescarte, valorMontagem10, valorMontagem11, valorMontagem12, valorMontagem13;
    char naipeEstoque, naipeDescarte, naipeMontagem10, naipeMontagem11, naipeMontagem12, naipeMontagem13;
    
    //Imprimir o estoque, o descarte e a montagem
    printf("  ___    ___           ___   ___   ___   ___\n" );
    //Estoque
    aux = pilhasCarta[7].carta->prox;
    if(aux == NULL){
        naipeEstoque = ' ';
        
    }else{
        naipeEstoque = '*';
    }
    
    //Descarte
    aux = pilhasCarta[8].carta->prox;
    if(aux == NULL){
        naipeDescarte = ' ';
        valorDescarte = 32;
    }else{
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        naipeDescarte = aux->naipe;
        valorDescarte = aux->valor;
    }
    
    //Montante10
    aux = pilhasCarta[9].carta->prox;
    if(aux == NULL){
        naipeMontagem10 = ' ';
        valorMontagem10 = 32;
    }else{
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        naipeMontagem10 = aux->naipe;
        valorMontagem10 = aux->valor;
    }
    
    //Montante11
    aux = pilhasCarta[10].carta->prox;
    if(aux == NULL){
        naipeMontagem11 = ' ';
        valorMontagem11 = 32;
    }else{
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        naipeMontagem11 = aux->naipe;
        valorMontagem11 = aux->valor;
    }
    
    //Montante12
    aux = pilhasCarta[11].carta->prox;
    if(aux == NULL){
        naipeMontagem12 = ' ';
        valorMontagem12 = 32;
    }else{
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        naipeMontagem12 = aux->naipe;
        valorMontagem12 = aux->valor;
    }
    
    //Montante13
    aux = pilhasCarta[12].carta->prox;
    if(aux == NULL){
        naipeMontagem13 = ' ';
        valorMontagem13 = 32;
    }else{
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        naipeMontagem13 = aux->naipe;
        valorMontagem13 = aux->valor;
    }
    
    printf(" | %c |  ", naipeEstoque);
    
    //Descarta - naipe
    if(naipeDescarte == 'O' || naipeDescarte == 'C'){
        printf("| \033[31m%c\033[39m |", naipeDescarte);           
    }else{
        printf("| %c |", naipeDescarte);           
    }
    
    printf("         ");
    //Montagem 10 - naipe
    if(naipeMontagem10 == 'O' || naipeMontagem10 == 'C'){
        printf("| \033[31m%c\033[39m | ", naipeMontagem10);           
    }else{
        printf("| %c | ", naipeMontagem10);
    }
    
    // Montagem 11 - naipe
    if(naipeMontagem11 == 'O' || naipeMontagem11 == 'C'){
        printf("| \033[31m%c\033[39m | ", naipeMontagem11);
    }else{
        printf("| %c | ", naipeMontagem11);
    }
    
    // Montagem 12 - naipe
    if(naipeMontagem12 == 'O' || naipeMontagem12 == 'C'){
        printf("| \033[31m%c\033[39m | ", naipeMontagem12);
    }else{
        printf("| %c | ", naipeMontagem12);
    }
    
    // Montagem 13 - naipe
    if(naipeMontagem13 == 'O' || naipeMontagem13 == 'C'){
        printf("| \033[31m%c\033[39m |\n", naipeMontagem13);
    }else{
        printf("| %c |\n", naipeMontagem13);
    }

    //-=-=-=-=-=--=-=-=-=-=-=-=
    //-=-=-=Valores das cartas
    printf(" |_%c_|  ", naipeEstoque);
    
    //Descarte - valor
    if(valorDescarte == 32){
        printf("|_%c_|", (char)valorDescarte);
    }else{
        if(qtdDigitos(valorDescarte) == 1){
            if(naipeDescarte == 'O' || naipeDescarte == 'C'){
                printf("|_\033[31m%d\033[39m_|", valorDescarte);
            }else{
                printf("|_%d_|", valorDescarte);
            }
        }else{
            if(naipeDescarte == 'O' || naipeDescarte == 'C'){
                printf("|_\033[31m%d\033[39m|", valorDescarte);
            }else{
                printf("|_%d|", valorDescarte);
            }
        }
    }        
    
    printf("         ");
    
    //Montagem 10 - valor
    if(valorMontagem10 == 32){
        printf("|_%c_| ", (char)valorMontagem10);
    }else{
        if(qtdDigitos(valorMontagem10) == 1){
            if(naipeMontagem10 == 'O' || naipeMontagem10 == 'C'){
                printf("|_\033[31m%d\033[39m_| ", valorMontagem10);
            }else{
                printf("|_%d_| ", valorMontagem10);
            }
        }else{
            if(naipeMontagem10 == 'O' || naipeMontagem10 == 'C'){
                printf("|_\033[31m%d\033[39m| ", valorMontagem10);
            }else{
                printf("|_%d| ", valorMontagem10);
            }
        }
    }
    
    //Montagem 11 - valor
    if(valorMontagem11 == 32){
        printf("|_%c_| ", (char)valorMontagem11);
    }else{
        if(qtdDigitos(valorMontagem11) == 1){
            if(naipeMontagem11 == 'O' || naipeMontagem11 == 'C'){
                printf("|_\033[31m%d\033[39m_|", valorMontagem11);
            }else{
                printf("|_%d_|", valorMontagem11);
            }
        }else{
            if(naipeMontagem11 == 'O' || naipeMontagem11 == 'C'){
                printf("|_\033[31m%d\033[39m|", valorMontagem11);
            }else{
                printf("|_%d|", valorMontagem11);
            }
        }
    }
    
    //Montagem 12 - valor
    if(valorMontagem12 == 32){
        printf("|_%c_| ", (char)valorMontagem12);
    }else{
        if(qtdDigitos(valorMontagem12) == 1){
            if(naipeMontagem12 == 'O' || naipeMontagem12 == 'C'){
                printf("|_\033[31m%d\033[39m_|", valorMontagem12);
            }else{
                printf("|_%d_|", valorMontagem12);
            }
        }else{
            if(naipeMontagem12 == 'O' || naipeMontagem12 == 'C'){
                printf("|_\033[31m%d\033[39m|", valorMontagem12);
            }else{
                printf("|_%d|", valorMontagem12);
            }
        }
    }
    
    //Montagem 13 - valor
    if(valorMontagem13 == 32){
        printf("|_%c_|\n", (char)valorMontagem13);
    }else{
        if(qtdDigitos(valorMontagem13) == 1){
            if(naipeMontagem13 == 'O' || naipeMontagem13 == 'C'){
                printf("|_\033[31m%d\033[39m_|", valorMontagem13);
            }else{
                printf("|_%d_|", valorMontagem13);
            }
        }else{
            if(naipeMontagem13 == 'O' || naipeMontagem13 == 'C'){
                printf("|_\033[31m%d\033[39m|", valorMontagem13);
            }else{
                printf("|_%d|", valorMontagem13);
            }
        }
    }
}

int validarMovimento(int valorCartaMovida, int valorCartaAcima, char corCartaMovida, char corCartaAcima,char naipeCartaMovida, char naipeCartaAcima, int opc){
    if(opc == 1){
        if(valorCartaMovida == 13 && valorCartaAcima == -1){ // de 13 para espaço vazio
            return 0; //  movimento válido
        }else if(((valorCartaMovida +1) == valorCartaAcima) && (corCartaMovida != corCartaAcima)){
            return 1; // movimento válido
        }else{
            return 2;
        }
    }else if(opc == 2){ // De descarte para montagem, ou de pilha para montagem.
        if(valorCartaMovida == 1 && valorCartaAcima == -1){
            return 0;
        }else if((valorCartaMovida - 1) == valorCartaAcima && naipeCartaMovida == naipeCartaAcima){
            return 1;
        }else{
            return 2;
        }
    }
    return 3;
}

void imprimirEstoque(TPPilhaCarta *cartasEstoque){
    TPCarta *aux;
    aux = cartasEstoque->carta->prox;
    printf("\nESTOQUE\n");
    while(aux != NULL){
        printf("%d%c  ", aux->valor, aux->naipe);
        aux = aux->prox;
    }
    printf("\n");
}

void imprimirDescarte(TPPilhaCarta *cartasEstoque){
    TPCarta *aux;
    aux = cartasEstoque->carta->prox;
    printf("\nDESCARTE\n");
    while(aux != NULL){
        printf("%d%c  ", aux->valor, aux->naipe);
        aux = aux->prox;
    }
    printf("\n");
}

void imprimirMontagem(TPPilhaCarta *pilhasCartas){
    TPCarta *aux;
    for(int i=9; i<13; i++){
        printf("MONTAGEM %d: ", i+1);
        aux = pilhasCartas[i].carta->prox;
        while(aux != NULL){
            printf("%d%c ", aux->valor, aux->naipe);
            aux = aux->prox;
        }
        printf("\n");
    }
}

int condicaoVitoria(TPPilhaCarta *pilhasCartas){
    TPCarta *aux;
    int i, cont = 0;
    for(i=9; i<13; i++){
        aux = pilhasCartas[i].carta;
        while(aux->prox != NULL){
            aux = aux->prox;
            cont++;
        }
    }
    
    if(cont == 52){
        return 0;
    }else{
        return 1;
    }
}