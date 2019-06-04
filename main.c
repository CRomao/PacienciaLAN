/* File:   main.c
 * Author: Romão
 *
 * Created on 25 de Maio de 2019, 19:36
 */
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

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
    char nome[14];
    //int tempo;
    char pont[5];
    char qtdMov[5];
    struct jogador *prox;
    struct jogador *ant;
};
typedef struct jogador TPJogador;

struct historico{
    struct historico *ant;
    struct historico *prox;
    char visivelCartaAnterior;
    char movEspecial;
    TPCarta *carta;
    TPCarta * cartaAnterior;
    int pilhaOrigem;
    int pilhaDestino;
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
void fazerMovimentoEstoqueDescarte(TPPilhaCarta *estoque, TPPilhaCarta *descarte, TPHistorico *movimento, int *contMovimentos);
void fazerMovimentoPP(int pilhaOrigem, int valorCarta, int pilhaDestino, TPPilhaCarta *pilhasCartas, TPHistorico *movimento, int *contMovimentos, int opc);
void reporEstoque(TPPilhaCarta *estoque, TPPilhaCarta *descarte);
void validarcampo(int campo, char tipo);
void imprimirTemp(TPPilhaCarta *pilhasCarta, int *contMovimentos);
int qtdDigitos(int valor);
int condicaoVitoria(TPPilhaCarta *pilhasCartas);
int verificarMontagem(int montagem);
int main(int argc, char** argv){
    FILE *rank;
    rank = fopen("rank.txt", "r+"); // vai tentar abrir o arquivo do rank.
    if(rank == NULL)rank = fopen("rank.txt", "w+"); // se não existir, ele cria o arquivo.
    TPCarta cartas[52];
    TPPilhaCarta PilhasC[13];
    TPHistorico movimento;
    TPJogador jogador;
    jogador.ant = NULL;
    jogador.prox = NULL;
    int pOrigem, pDestino, valorC, contMovimentos = 0, opc, menu,voltarJogada, histJogada;
    char loop = 'N';
    iniciarCartas(&cartas);
    embaralharCartas(&cartas);
    inicializarCabecaPilhas(&PilhasC);
    distribuicaoInicial(&PilhasC, &cartas);
    distribuicaoInicialEstoque(&PilhasC[7], &cartas);
    inicializarHistorico(&movimento);
    
    
    //contarJogadoresArquivo(rank, &jogador);
   // imprimirJogadores(&jogador);
            
    while(loop == 'N'){
        menu = menuPrincipal();
        if(menu == 1){
            loop = 'S';
        }else if( menu == 2){
            regrasJogo();
        }else if( menu == 3){
            //rank
      imprimirJogadores(&jogador);
        }else if(menu == 4){
            break;
        }
    }
    
    
    while(loop == 'S'){
        imprimirTemp(&PilhasC, &contMovimentos);
        printf("\n");
        imprimir(&PilhasC);
        printf("\n");
        printf("Escolha a operação: \n");
        printf("1 - Mover | ");
        printf("2 - Desfazer movimento | ");
        printf("3 - Próxima carta do Estoque | 4 - Histórico de Jogadas\n--> ");
        scanf("%d", &opc);
        switch(opc){
            case 1:
                printf("1 - Mover de DESCARTE para MONTAGEM | ");
                printf("2 - Mover de DESCARTE para PILHA | ");
                printf("3 - Mover de PILHA para MONTAGEM | ");
                printf("4 - Mover de PILHA para PILHA | ");
                printf("5 - Mover de MONTAGEM para PILHA\n-->");
                scanf("%d", &opc);
                switch(opc){
                    case 1:
                        printf("Informe para qual MONTAGEM quer mover: ");
                        scanf("%d", &pDestino);
                        valorC = buscarCarta(&PilhasC[8].carta);
                        pDestino = verificarMontagem(pDestino);
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
                        valorC = buscarCarta(PilhasC[(pOrigem-1)].carta);
                        printf("\nInforme a MONTAGEM que vai receber a carta: ");
                        scanf("%d", &pDestino);
                        pDestino = verificarMontagem(pDestino);
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
                        break;
                    case 5:
                        printf("\nInforme a MONTAGEM que tem a carta: ");
                        scanf("3%d", &pOrigem);
                        valorC = buscarCarta(PilhasC[(pOrigem-1)].carta);
                        printf("\nInforme a PILHA que vai receber a carta: ");
                        scanf("%d", &pDestino);
                        pOrigem = verificarMontagem(pOrigem);
                        fazerMovimentoPP(pOrigem, valorC, pDestino, &PilhasC, &movimento, &contMovimentos, 1);
                }
                break;
            case 2:
                desfazerMovimento(&PilhasC, &movimento, &contMovimentos);
                break;
            case 3:
                fazerMovimentoEstoqueDescarte(&PilhasC[7], &PilhasC[8], &movimento, &contMovimentos);
                break;
            case 4:
                histJogada =imprimirHistoricoJogadas(&movimento);
                if(histJogada == 0){
                    printf("Histórico vazio.\n");
                }else{
                    printf("Deseja voltar para uma jogada específica?\n1-SIM\n2-NÃO\n--> ");
                    scanf("%d", &voltarJogada);
                    switch(voltarJogada){
                        case 1:
                            printf("Para qual jogada deseja voltar?(Informe o número da jogada no canto esquerdo)\n");
                            scanf("%d", &voltarJogada);
                            histJogada = (histJogada - voltarJogada);
                            for(int i=0; i<histJogada; i++)desfazerMovimento(&PilhasC, &movimento, &contMovimentos);
                            break;
                        case 2:
                            break;
                        default:
                            break;
                    }
                    
                }
        }
        if(condicaoVitoria(&PilhasC) == 52){
            printf("\n\nPARABÉNS, VOCÊ TERMINOU!!!\n\n");
            break;
        }else{
            system("cls");
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
        //atribuição a nova pilha
        aux3->prox = aux;
        aux->ant = aux3;
        //Pegar dados desse movimento para jogar na função de registrar no histórico
        newMovimento->carta = aux;
        newMovimento->prox = NULL;
        newMovimento->ant = NULL;
        newMovimento->movEspecial = 'N';
        newMovimento->pilhaOrigem = pilhaOrigem;
        newMovimento->pilhaDestino = pilhaDestino;
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
            (*contMovimentos)--;
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
            (*contMovimentos)--;
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
    historico->prox = NULL;
    historico->cartaAnterior = NULL;
    historico->movEspecial = 'Z';
}

void fazerMovimentoEstoqueDescarte(TPPilhaCarta *estoque, TPPilhaCarta *descarte, TPHistorico *movimento, int *contMovimentos){ // Função para passar a carta para o descarte
    if(estoque->carta->prox == NULL){
        reporEstoque(estoque, descarte);// Caso o estoque esteja vazio, chama a função de repor o estoque
        TPHistorico *newMovimentoEspecial; // e guarda esse movimento como especial
        newMovimentoEspecial = malloc(sizeof(TPHistorico));
        newMovimentoEspecial->cartaAnterior = NULL;
        newMovimentoEspecial->prox = NULL;
        newMovimentoEspecial->ant = NULL;
        newMovimentoEspecial->carta = NULL;
        newMovimentoEspecial->movEspecial = 'S';
        newMovimentoEspecial->visivelCartaAnterior = 'N';
        newMovimentoEspecial->pilhaOrigem = 8;
        newMovimentoEspecial->pilhaDestino = 7;
        registrarMovimento(newMovimentoEspecial, movimento);
        (*contMovimentos)++;
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
        newMovimento->carta = aux;
        newMovimento->prox = NULL;
        newMovimento->ant = NULL;
        newMovimento->movEspecial = 'N';
        newMovimento->pilhaOrigem = 7;
        newMovimento->pilhaDestino = 8;
        registrarMovimento(newMovimento, movimento);
        (*contMovimentos)++;
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
    while(aux->prox != NULL)aux = aux->prox;
    return aux->valor;
}

void regrasJogo(){
    
    
    printf("Pressione qualquer tecla para voltar para o menu...");
    system("pause");

}

void sairJogo(){
    
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

void imprimirTemp(TPPilhaCarta *pilhasCarta, int *contMovimentos){
    TPCarta *aux;
    int valor;
    char naipeEstoque, naipe;
    //Imprimir o estoque, o descarte e a montagem
    printf("  ___    ___           ___   ___   ___   ___\n" );
    //Estoque
    aux = pilhasCarta[7].carta->prox;
    if(aux == NULL){
        naipeEstoque = ' ';
        
    }else{
        naipeEstoque = '*';
    }
    printf(" | %c |  ", naipeEstoque);
     //NAIPES CABEÇALHO
    for(int i=8; i<13; i++){
        aux = pilhasCarta[i].carta->prox;
        if(aux == NULL){
            naipe = ' ';
            if(i == 9)printf("        ");
            imprimirNaipeCabecalho(naipe);
        }else{
            while(aux->prox != NULL) aux = aux->prox;
                if(i == 9)printf("        ");
                imprimirNaipeCabecalho(aux->naipe);
        }
    }
    //Imprimir pontos
    printf("          PONTOS: %d", condicaoVitoria(pilhasCarta));
    printf("\n |_%c_|  ", naipeEstoque);
    
    //VALORES CABEÇALHO
    for(int i=8; i<13; i++){
      aux = pilhasCarta[i].carta->prox;
      if(aux == NULL){
          valor = 32;
          if(i == 9)printf("        ");
          //if(i == 8)printf(" ");
          imprimirValorCabecalho(naipe, valor);
      }else{
        while(aux->prox != NULL) aux = aux->prox;
            if(i == 9)printf("        ");
            imprimirValorCabecalho(aux->naipe, aux->valor);
      }
    }
    printf("          MOVIMENTOS: %d\n", *contMovimentos);
    printf("                        1     2     3     4");
}

void imprimirNaipeCabecalho(char naipe){
    if(naipe == 'O' || naipe == 'C'){
        printf("| \033[31m%c\033[39m | ", naipe);           
    }else{
        printf("| %c | ", naipe);
    }
}        

void imprimirValorCabecalho(char naipe, int valor){
    if(valor == 32){
        printf("|_%c_| ", (char)valor);
    }else{
        if(qtdDigitos(valor) == 1){
            if(naipe == 'O' || naipe == 'C'){
                printf("|_\033[31m%d\033[39m_| ", valor);
            }else{
                printf("|_%d_| ", valor);
            }
        }else{
            if(naipe == 'O' || naipe == 'C'){
                printf("|_\033[31m%d\033[39m| ", valor);
            }else{
                printf("|_%d| ", valor);
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

int condicaoVitoria(TPPilhaCarta *pilhasCartas){
    TPCarta *aux;
    int i, cont = 0;
    for(i=9; i<13; i++){
        aux = pilhasCartas[i].carta->prox;
        while(aux != NULL){
            aux = aux->prox;
            cont++;
        }
    }   
    return cont;
}


void simularDoisPassos(TPPilhaCarta * pilhasCartas){
    int cont = 0, i,j;
    TPCarta *aux;
    for(i=0; i<7; i++){
        aux = pilhasCartas[i].carta;
        while(aux != NULL){
            aux = aux->prox;
            if(aux->visivel == 'S')cont++;
        }
        
        for(j=0; j<cont; j++){
            
        }
    }
}

void novoJogo(TPCarta *cartas, TPPilhaCarta *pilhasCartas){
    for(int i=0; i<52; i++){
        cartas[i].ant = NULL;
        cartas[i].prox = NULL;
    }
    for(int i=0; i<13; i++)pilhasCartas[i].carta->prox = NULL;
    embaralharCartas(cartas);
    distribuicaoInicial(pilhasCartas, cartas);
    //distribuicaoInicialEstoque(pilhasCartas[7], cartas);
}

int menuPrincipal(){
    int opc;
    printf("\t\t\t-=-=-=-=-=PACIENCIALAN-=-=-=-=-=\n");
    printf("\t\t\t           versão 1.0\n\n\n");
    printf("1 - Novo Jogo | 2 - Regras | 3 - Rank 10 | 4 - Sair\n-->");
    scanf("%d", &opc);
    switch(opc){
        case 1:
            return 1;
        case 2:
            return 2;
        case 3:
            return 3;
        case 4:
            return 4;
    }
}

void iniciarJogador(TPJogador *jogador){
    jogador->ant = NULL;
    jogador->prox = NULL;
}

void contarJogadoresArquivo(FILE *rank, TPJogador *jogadores){
        int linhas = 0, tam;
        char auxS[14];
        TPJogador *novoJogador, *aux;
        aux = jogadores;
        novoJogador = malloc(sizeof(TPJogador));
        iniciarJogador(novoJogador);
         while(!feof(rank)){
                if(linhas == 3)linhas = 0;
                fgets(auxS, 14, rank);
                tam = strlen(auxS);
                if(linhas == 0){
                    for(int i=0; auxS[i]!='\r'; i++)novoJogador->nome[i] = auxS[i]; // pegar os nomes
                    linhas++;
                }else if(linhas == 1){
                    for(int i=0; auxS[i]!='\r'; i++)novoJogador->pont[i] = auxS[i]; // pegr os pontos
                    linhas++;
                }else if(linhas == 2){
                    for(int i=0; auxS[i]!='\n'; i++)novoJogador->qtdMov[i] = auxS[i]; // pegar a qtd de movimentos de cada jogador
                    linhas++;
                    aux->prox = novoJogador;
                    novoJogador->ant = aux;
                    aux = aux->prox;
                    novoJogador = malloc(sizeof(TPJogador));
                    iniciarJogador(novoJogador);
                }
        } 
}

void imprimirJogadores(TPJogador *jogador){
    int cont=0;
    TPJogador *aux;
    aux = jogador->prox;
    printf("   JOGADOR\t\tPONTOS\t\tQTD. MOVIMENTOS\n");
    while(aux != NULL){
        printf("%dº %s\t\t%s\t\t%s\n",(cont+1), aux->nome, aux->pont, aux->qtdMov);
        cont++;
        aux = aux->prox;
    }
}

int verificarMontagem(int montagem){
    if(montagem == 1){
        return 10;
    }else if(montagem == 2){
        return 11;
    }else if(montagem == 3){
        return 12;
    }else if(montagem == 4){
        return 13;
    }
}

int imprimirHistoricoJogadas(TPHistorico *historico){
    TPHistorico *aux;
    int totalRegistros=0;
    aux = historico->prox;
    printf("   P.Origem\tNaipe/Valor   P.Destino\n");
    while(aux != NULL){
        totalRegistros++;
        printf("%dº\t%d\t    %c/%d      \t%d\n",totalRegistros,(aux->pilhaOrigem+1), aux->carta->naipe, aux->carta->valor, (aux->pilhaDestino+1));
        aux = aux->prox;
    }
    return totalRegistros;
}