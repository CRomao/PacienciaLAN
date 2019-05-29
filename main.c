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
int validarMovimento(int valorCartaMovida, int valorCartaAcima, char corCartaMovida, char corCartaAcima);
int buscarCarta(TPCarta *carta);

int main(int argc, char** argv) {
    TPCarta cartas[52];
    TPPilhaCarta PilhasC[13], descarteC, montanteC[4];
    TPHistorico movimento;
//    printf("MAIN LISTAMOV: %p\n", &movimento);
    int pOrigem, pDestino, valorC, contMovimentos = 0, opc;
    iniciarCartas(&cartas);
    embaralharCartas(&cartas);
    inicializarCabecaPilhas(&PilhasC);
    distribuicaoInicial(&PilhasC, &cartas);
    //estoqueC = &PilhasC[7].carta;
    
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
        imprimirEstoque(&PilhasC[7]);
        imprimirDescarte(&PilhasC[8]);
        imprimir(&PilhasC);
        imprimirMontagem(&PilhasC);
        printf("Escolha a operação: \n");
        printf("1 - Mover\n");
        printf("2 - Desfazer movimento\n");
        printf("3 - Próxima carta do Estoque\n");
        scanf("%d", &opc);
        switch(opc){
            case 1:
                printf("1 - Mover de DESCARTE para MONTAGEM\n");
                printf("2 - Mover de DESCARTE para PILHA\n");
                printf("3 - Mover de PILHA para MONTAGEM\n");
                printf("4 - Mover de PILHA para PILHA\n");
                scanf("%d", &opc);
                switch(opc){
                    case 1:
                        printf("Informe para qual MONTAGEM quer mover: ");
                        scanf("%d", &pDestino);
                        valorC = buscarCarta(&PilhasC[8].carta);
                        fazerMovimentoPP(9, valorC, pDestino, &PilhasC, &movimento, &contMovimentos);
                        break;
                    case 2:
                        printf("Informe para qual PILHA quer mover: ");
                        scanf("%d", &pDestino);
                        valorC = buscarCarta(&PilhasC[8].carta);
                        fazerMovimentoPP(9, valorC, pDestino, &PilhasC, &movimento, &contMovimentos);
                        break;
                    case 3:
                        printf("\nInforme a PILHA que tem a carta: ");
                        scanf("%d", &pOrigem);
                        printf("\nInforme o VALOR da carta: ");
                        scanf("%d", &valorC);
                        printf("\nInforme a MONTAGEM que vai receber a carta: ");
                        scanf("%d", &pDestino);
                        fazerMovimentoPP(pOrigem, valorC, pDestino, &PilhasC, &movimento, &contMovimentos);
                        break;
                    case 4:
                        printf("\nInforme a PILHA que tem a carta: ");
                        scanf("%d", &pOrigem);
                        printf("\nInforme o VALOR da carta: ");
                        scanf("%d", &valorC);
                        printf("\nInforme a PILHA que vai receber a carta: ");
                        scanf("%d", &pDestino);
                        fazerMovimentoPP(pOrigem, valorC, pDestino, &PilhasC, &movimento, &contMovimentos);
                        imprimir(&PilhasC);
                }
                break;
            case 2:
                desfazerMovimento(&PilhasC, &movimento, &contMovimentos);
                imprimir(&PilhasC);
                break;
            case 3:
                fazerMovimentoEstoqueDescarte(&PilhasC[7], &PilhasC[8], &movimento);
                break;
        }
        //printf("\nCONTMOV: %d\n", contMovimentos);
        
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
    //aux = malloc(sizeof(TPCarta));
    //aux2 = malloc(sizeof(TPCarta));
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
    validarMov = validarMovimento(aux->valor, aux3->valor, aux->cor, aux3->cor);
    if(validarMov == 0 || validarMov == 1){    
        aux2->prox = NULL;
        aux2->visivel = 'S';
        //aux->ant = NULL; - tirar
        //atribuição a nova pilha
        aux3->prox = aux;
        aux->ant = aux3;
        //Pegar dados desse movimento para jogar na função de registrar no histórico
        newMovimento->pilhaOrigem = pilhaOrigem;
        newMovimento->pilhaDestino = pilhaDestino;
        newMovimento->carta = aux;
        newMovimento->prox = NULL;
        newMovimento->ant = NULL;
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
    destino =  aux->carta;
    //while(destino != aux->carta){ // Vai pegar a carta que tinha sido movimentada nesse movimento
      //  destino = destino->prox;
   // }
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
    aux = movimento;
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

void fazerMovimentoEstoqueDescarte(TPPilhaCarta *estoque, TPPilhaCarta *descarte, TPHistorico *movimento){ // Função para passar a carta para o descarte
    if(estoque->carta->prox == NULL){
        reporEstoque(estoque, descarte);// Caso o estoque esteja vazio, chama a função de repor o estoque
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
        auxEstoque->prox = NULL;
        aux->ant = NULL;
        // Ajustando a carta para o descarte
        auxDescarte->prox = aux;
        aux->ant = auxDescarte; 
        auxDescarte->visivel = 'S';
        
        newMovimento->pilhaOrigem = 7;
        newMovimento->pilhaDestino = 8;
        newMovimento->carta = aux;
        newMovimento->prox = NULL;
        newMovimento->ant = NULL;
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
        printf("MONTAGEM: %d", i);
        aux = pilhasCartas[i].carta;
        while(aux->prox != NULL){
            printf("%d%c ", aux->valor, aux->naipe);
            aux = aux->prox;
        }
        printf("\n");
    }
}