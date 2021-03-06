/* File:   main.c
 * Author: Romão
 *
 * Created on 25 de Maio de 2019, 19:36
 */
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

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
    int pont;
    int qtdMov;
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
    TPCarta *cartaAnterior;
    int pilhaOrigem;
    int pilhaDestino;
    int pontosJogada;
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
void imprimirTemp(TPPilhaCarta *pilhasCarta, int *contMovimentos, TPHistorico *historico);
int qtdDigitos(int valor);
int condicaoVitoria(TPPilhaCarta *pilhasCartas);
int verificarMontagem(int montagem);
int verificarValorPilhaMovimento(int origem, char id );
void salvarJogador(TPJogador *jogadores, char *nomeJogador, int pontos, int mov);
int contarQTDMovimento(TPHistorico *historico);
int verificarPontosJogada(int origem, int destino);
void verificarNaipeValorHistoricoJogada(char naipe, int valor);
void imprimirHistoricoJogadasNomePilhas(int valor);
void imprimirHistoricoJogadas(TPHistorico *historico);
void imprimirJogadores(TPJogador *jogador);
void salvarNoArquivoJogadores(FILE **rank, TPJogador *jogadores);
void ordenarJogadores(TPJogador *jogador);
void contarJogadoresArquivo(FILE *rank, TPJogador *jogadores);
int menuPrincipal();
void novoJogo(TPCarta *cartas, TPPilhaCarta *pilhasCartas, TPHistorico *historico, int *contMovimentos);
int contarPontos(TPHistorico *historico);
void imprimirValorCabecalho(char naipe, int valor);
void imprimirNaipeCabecalho(char naipe);
void simularDoisPassos(TPPilhaCarta *pilhas);
void regrasJogo();
int main(int argc, char** argv){
    FILE *rank;
    rank = fopen("rank.txt", "r+"); // vai tentar abrir o arquivo do rank em RW.
    if(rank == NULL)rank = fopen("rank.txt", "w+"); // se não existir, ele cria o arquivo em RW.
    TPCarta cartas[52];
    TPPilhaCarta PilhasC[13];
    TPHistorico movimento;
    TPJogador *jogador;
    jogador = malloc(sizeof(TPJogador));
    jogador->ant = NULL, jogador->prox = NULL, jogador->nome[0] = '\0', jogador->pont = 0, jogador->qtdMov = 0;
    int pOrigem, pDestino, valorC, contMovimentos = 0, opc, menu,voltarJogada, histJogada;
    char loop = 'N', nomeJogador[14];
    contarJogadoresArquivo(rank, jogador);

    while(loop == 'N'){
        menu = menuPrincipal();
        if(menu == 1){
            loop = 'S';
            iniciarCartas(&cartas);
            embaralharCartas(&cartas);
            inicializarCabecaPilhas(&PilhasC);
            distribuicaoInicial(&PilhasC, &cartas);
            distribuicaoInicialEstoque(&PilhasC, &cartas);
            inicializarHistorico(&movimento);
        }else if( menu == 2){
            regrasJogo();
            getchar();
            printf("\nPressione Enter...\n");
            getchar(); 
            system("clear");
        }else if( menu == 3){
            imprimirJogadores(jogador);
            getchar();
            printf("\nPressione Enter...\n");
            getchar();
            system("clear");
        }else if(menu == 4){
            exit(0);
        }
    }
    system("clear");
    while(loop == 'S'){
        imprimirTemp(&PilhasC, &contMovimentos, &movimento);
        printf("\n");
        imprimir(&PilhasC);
        printf("\n");
        printf("Escolha a operação: \n");
        printf("1 - Mover | ");
        printf("2 - Desfazer movimento | ");
        printf("3 - Próxima carta do Estoque | 4 - Outras Funções | 0 - Sair\n--> ");
        scanf("%d", &opc);
        switch(opc){
            case 1:
                printf("1 - Mover de DESCARTE para MONTAGEM | ");
                printf("2 - Mover de DESCARTE para PILHA | ");
                printf("3 - Mover de PILHA para MONTAGEM | ");
                printf("4 - Mover de PILHA para PILHA |\n");
                printf("5 - Mover de MONTAGEM para PILHA\n-->");
                scanf("%d", &opc);
                switch(opc){
                    case 1:
                        printf("Informe para qual MONTAGEM quer mover: ");
                        scanf("%d", &pDestino);
                        valorC = buscarCarta(&PilhasC[8].carta);
                        if(verificarValorPilhaMovimento(pDestino, 'M') == -1){
                            printf("informe o número de montagem válido(1-2-3-4).\n");
                            sleep(3);
                            break;
                        }
                        pDestino = verificarMontagem(pDestino);
                        fazerMovimentoPP(9, valorC, pDestino, &PilhasC, &movimento, &contMovimentos, 2);
                        break;
                    case 2:
                        printf("Informe para qual PILHA quer mover: ");
                        scanf("%d", &pDestino);
                        if(verificarValorPilhaMovimento(pDestino, 'P') == -1){
                            printf("informe o número de pilha válido(1-2-3-4-5-6-7).\n");
                            sleep(3);
                            break;
                        }
                        valorC = buscarCarta(&PilhasC[8].carta);
                        fazerMovimentoPP(9, valorC, pDestino, &PilhasC, &movimento, &contMovimentos, 1);
                        break;
                    case 3:
                        printf("\nInforme a PILHA que tem a carta: ");
                        scanf("%d", &pOrigem);
                        if(verificarValorPilhaMovimento(pOrigem, 'P') == -1){
                            printf("informe o número de pilha válido(1-2-3-4-5-6-7).\n");
                            sleep(3);
                            break;
                        }
                        valorC = buscarCarta(PilhasC[(pOrigem-1)].carta);
                        printf("\nInforme a MONTAGEM que vai receber a carta: ");
                        scanf("%d", &pDestino);
                        if(verificarValorPilhaMovimento(pDestino, 'M') == -1){
                            printf("informe o número de montagem válido(1-2-3-4).\n");
                            sleep(3);
                            break;
                        }
                        pDestino = verificarMontagem(pDestino);
                        fazerMovimentoPP(pOrigem, valorC, pDestino, &PilhasC, &movimento, &contMovimentos, 2);
                        break;
                    case 4:
                        printf("\nInforme a PILHA que tem a carta: ");
                        scanf("%d", &pOrigem);
                        if(verificarValorPilhaMovimento(pOrigem, 'P') == -1){                            
                            printf("informe o número de pilha válido(1-2-3-4-5-6-7).\n");
                            sleep(3);
                            break;
                        }
                        printf("\nInforme o VALOR da carta: ");
                        scanf("%d", &valorC);
                        printf("\nInforme a PILHA que vai receber a carta: ");
                        scanf("%d", &pDestino);
                        if(verificarValorPilhaMovimento(pDestino, 'P') == -1){
                            printf("informe o número de pilha válido(1-2-3-4-5-6-7).\n");
                            sleep(3);
                            break;
                        }
                        fazerMovimentoPP(pOrigem, valorC, pDestino, &PilhasC, &movimento, &contMovimentos, 1);
                        break;
                    case 5:
                        printf("\nInforme a MONTAGEM que tem a carta: ");
                        scanf("%d", &pOrigem);
                        if(verificarValorPilhaMovimento(pOrigem, 'M') == -1){
                            printf("informe o número de montagem válido(1-2-3-4).\n");
                            sleep(3);
                            break;
                        }
                        printf("\nInforme a PILHA que vai receber a carta: ");
                        scanf("%d", &pDestino);
                        if(verificarValorPilhaMovimento(pDestino, 'P') == -1){
                            printf("informe o número de pilha válido(1-2-3-4-5-6-7).\n");
                            sleep(3);
                            break;
                        }
                        pOrigem = verificarMontagem(pOrigem);
                        valorC = buscarCarta(PilhasC[(pOrigem-1)].carta);
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
                printf("1 - Histórico de Jogadas | 2 - Dicas | 3 - Novo Jogo | 4 - Voltar para o jogo\n--> ");
                scanf("%d", &opc);
                switch(opc){
                    case 1:
                        histJogada =contarQTDMovimento(&movimento);
                        if(histJogada == 0){ 
                            printf("\nHistórico vazio.\n");
                            sleep(3);
                        }else{
                            imprimirHistoricoJogadas(&movimento);
                            printf("Deseja voltar para uma jogada específica?\n1-SIM\n2-NÃO\n--> ");
                            scanf("%d", &voltarJogada);
                            switch(voltarJogada){
                                case 1:
                                    printf("Para qual jogada deseja voltar?(Informe o número da jogada no canto esquerdo)\n-->");
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
                        break;
                    case 2://Dicas
                        getchar();
                        simularDoisPassos(&PilhasC);
                        break;
                    case 3:
                        novoJogo(&cartas, &PilhasC, &movimento, &contMovimentos);
                        break;
                    case 4:
                        break;
                }
                break;
            case 0:
                exit(0);
        }
        if(condicaoVitoria(&PilhasC) == 52){
            system("clear");
            imprimirTemp(&PilhasC, &contMovimentos, &movimento);
            printf("\n");
            imprimir(&PilhasC);
            printf("\n");
            printf("\n\nPARABÉNS, VOCÊ TERMINOU!!!\n\n");
            printf("Informe o seu nome ou apelido para salvar no RANK(mínimo 5 caracteres).\n-->");
            getchar();
            gets(nomeJogador);
            salvarJogador(jogador, &nomeJogador, contarPontos(&movimento), contMovimentos);
            ordenarJogadores(jogador);
            printf("\n%s salvo no RANK com sucesso!\nVeja abaixo a sua posição:\n\n", nomeJogador);
            imprimirJogadores(jogador);
            printf("\n\nDeseja jogar novamente?\n1-SIM\t2-NÂO\n-->");
            scanf("%d", &opc);
            switch(opc){
                case 1:
                    novoJogo(&cartas, &PilhasC, &movimento, &contMovimentos);
                    break;
                case 2:
                    remove(rank);
                    rank = fopen("rank.txt", "w");
                    salvarNoArquivoJogadores(&rank, jogador);
                    loop = 'N';
                    break;
            }
        }else{
            system("clear");
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
        auxEstoque = estoque[7].carta;
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
                        printf(" |_\033[31m%d\033[39m_| ", aux->valor);
                      }else{
                        printf(" |_%d_| ", aux->valor);  
                      }
                  }else{
                      printf(" |_*_| ");
                  }
              }else{
                  if(aux->visivel == 'S'){
                      if(aux->naipe == 'O' || aux->naipe == 'C'){
                            printf(" |_\033[31m%d\033[39m| ", aux->valor);
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
        if(pilhaOrigem == 8){
            printf("\nO descarte está vazio (DICA: puxe a próxima carta do estoque :) )\n");
            sleep(3);
        }else if(pilhaOrigem == 9 || pilhaOrigem == 10 || pilhaOrigem == 11 || pilhaOrigem == 12){
            printf("\nNão tem nenhuma carta na montagem para mover.\n");
            sleep(3);
        }
        
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
        newMovimento->pontosJogada = verificarPontosJogada(pilhaOrigem, pilhaDestino);
        registrarMovimento(newMovimento, movimento);
        (*contMovimentos)++;
    }else{
        printf("\nMovimento inválido.\n");
        sleep(3);
    }
}

void desfazerMovimento(TPPilhaCarta *pilhasCartas, TPHistorico *historico, int *contMovimentos){
    if(historico->prox == NULL){
        printf("\nAinda não consta nenhum movimento feito.\n");
        sleep(3);
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
        newMovimentoEspecial->pontosJogada = 0;
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
        newMovimento->pontosJogada = 0;
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
    printf("\n");
    printf("-> Objetivo: \n"
           "Mover todas as 52 cartas para as MONTAGENS em ordem crescente no naipe.\n\n");

    printf("-> Sobre o Jogo: \n\n"
           "- Montagem: \n"
           "Existem 4 na parte superior direita, e cada montagem recebe um tipo de naipe.\n\n"
           "- Pilhas: \n"
           "São 7 ao todo, e devem ser organizadas em sequência decrescente e com cores alternadas.\n"
           "Podem ser movidas em grupos.\n"
           "Os espaços em branco devem ser destinados ao Rei(13), ou grupos de cartas com o Rei sendo a maior carta.\n\n"
           "- Estoque: \n"
           "Localizado no canto superior esquerdo.\n"
           "Puxe uma carta por vez.\n"
           "Quando todas as cartas forem passadas para o descarte, selecione novamente a opção de próxima carta do estoque\n"
            "para fazer a reposição das cartas.\n\n"
           "- Descarte: \n"
           "Localizado ao lado do estoque.\n"
           "Apenas a carta do topo pode ser utilizada para mover tanto nas montagens quanto nas pilhas.\n\n");

    printf("Identificação das cartas: \n"
           "1-A, 2-2, 3-3, 4-4, 5-5, 6-6, 7-7, 8-8, 9-9, 10-10, 11-J, 12-Q, 13-K \n"
           "C-Coroa, P-Paus, E-Espadas, O-Ouros\n\n");

    printf("-> Jogabilidade: \n"
           "Selecione a Opção Mover; \n"
           "Escolha o tipo de movimentação; \n"
           "Preencha com a localizaão da carta pedida(somente quando for de pilha para pilha); \n"
           "Continue até atingir o Objetivo;\n");
    printf("-Para puxar uma nova carta do Estoque: \n"
           "Escolha a opção Proxima carta do Estoque;\n\n");
    printf("-Para desfazer algum movimento: \n"
           "Escolha a opção Desfazer Movimento;\n\n");
    printf("-Para momentos de dúvidas é possivel pedir uma dica: \n"
           "Basta ir em Outras Funções -> Dicas \n\n");
    printf("-Para quando não houver mais dicas ou possibilidades: \n"
           "Basta ir em Outras Funções -> Novo Jogo \n\n");
    printf("-Através do Histórico é possível retornar para qualquer momento do jogo, do inicio ao momento atual\n"
           "Basta ir em Outras Funções -> Histórico de Jogadas\n");
}

void imprimirTemp(TPPilhaCarta *pilhasCarta, int *contMovimentos, TPHistorico *historico){
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
    printf("          PONTOS: %d", contarPontos(historico));
    printf("\n |_%c_|  ", naipeEstoque);
    
    //VALORES CABEÇALHO
    for(int i=8; i<13; i++){
      aux = pilhasCarta[i].carta->prox;
      if(aux == NULL){
          valor = 32; // é um espaço em branco, de acordo com a tabela ASCII
          if(i == 9)printf("        ");
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
int contarPontos(TPHistorico *historico){
    TPHistorico *aux;
    aux = historico;
    int totalPontos=0;
    while(aux->prox != NULL){
        aux = aux->prox;
        totalPontos += aux->pontosJogada;
    }
    return totalPontos;
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

void novoJogo(TPCarta *cartas, TPPilhaCarta *pilhasCartas, TPHistorico *historico, int *contMovimentos){
    int qtdMovimentos, auxContMovimentos;
    auxContMovimentos = *(contMovimentos);
    qtdMovimentos = contarQTDMovimento(historico);
    for(int i=0; i<qtdMovimentos; i++)desfazerMovimento(pilhasCartas, historico, &auxContMovimentos);
    *(contMovimentos) = auxContMovimentos;
    for(int i=0; i<52; i++){
        cartas[i].ant = NULL;
        cartas[i].prox = NULL;
        cartas[i].visivel = 'N';
    }
    for(int i=0; i<13; i++)pilhasCartas[i].carta->prox = NULL;
    embaralharCartas(cartas);
    distribuicaoInicial(pilhasCartas, cartas);
    distribuicaoInicialEstoque(pilhasCartas, cartas);
}

int menuPrincipal(){
    int opc;
    printf("\t\t\t-=-=-=-=-=PACIENCIALAN-=-=-=-=-=\n");
    printf("\t\t\t           versão 1.0\n\n\n");
    printf("1 - Novo Jogo | 2 - Regras | 3 - Rank | 4 - Sair\n-->");
    scanf("%d", &opc);
    switch(opc){
        case 1: return 1;
        case 2: return 2;
        case 3: return 3;
        case 4: return 4;
    }
}

void contarJogadoresArquivo(FILE *rank, TPJogador *jogadores){
        int linhas = 0;
        TPJogador *novoJogador, *aux;
        aux = jogadores;
        novoJogador = malloc(sizeof(TPJogador));
         while(!feof(rank)){
                if(linhas == 3)linhas = 0;
                if(linhas == 0){
                    fscanf(rank, "%s", &(novoJogador->nome));// pegar os nomes
                    linhas++;
                }else if(linhas == 1){
                    fscanf(rank, "%d", &(novoJogador->pont)); // pegar os pontos
                    linhas++;
                }else if(linhas == 2){
                    fscanf(rank, "%d", &(novoJogador->qtdMov)); // pegar a qtd de movimentos
                    linhas++;
                    novoJogador->prox = NULL;
                    aux->prox = novoJogador;
                    novoJogador->ant = aux;
                    aux = aux->prox;
                    novoJogador = malloc(sizeof(TPJogador));
                }
        }
        free(novoJogador);
}

void ordenarJogadores(TPJogador *jogador){
    TPJogador *aux, *jogadoresOrdenados, *ultimoOrdenado, *aux2, *aux3, *aux4;
    //aux --  navegar nos jogadores;
    //aux2 -- jogador com maior pontuação da vez a ser retirado;
    //aux3 -- proximo do jogador a ser retirado;
    //aux4 -- anterior do jogador a ser retirado;
    int pontos=0, totalJogadores=0;
    
    jogadoresOrdenados = malloc(sizeof(TPJogador));
    jogadoresOrdenados->prox = NULL;
    ultimoOrdenado = jogadoresOrdenados;
    
    aux = jogador;
    while(aux->prox != NULL){ // while para pegar o total de jogadores
        aux = aux->prox;
        totalJogadores++;
    }
    
    if(totalJogadores > 0){ // caso seja maior que zero, entra aqui
        for(int i=0; i<totalJogadores; i++){ // for para executar a quantidade de jogadores
            aux = jogador->prox;
            while(aux != NULL){//while para pegar o jogador de maior pontuação da vez do for
                if(pontos < aux->pont){
                    pontos = aux->pont;
                    aux2 = aux;
                }
                aux = aux->prox;
            }
            pontos = 0; // precisa zerar para quando o FOR for iterar novamente, pois precisa pegar
            //o próximo maior da vez
            
            //ajustando os jogadores após remover o maior da vez
            aux3 = aux2->ant;
            aux4 = aux2->prox;
            aux3->prox = aux4;
            if(aux4 != NULL)aux4->ant = aux3; //para não dar erro
            aux2->prox = NULL;
            
            //pegar o ultimo da lista dos ordenados
            while(ultimoOrdenado->prox != NULL)ultimoOrdenado = ultimoOrdenado->prox;
            
            ultimoOrdenado->prox = aux2;// Adicionando o removido a lista ordenada
            aux2->ant = ultimoOrdenado;
            
        }
        aux = jogador;//após ordenar todos os jogadores, joga de volta para a lista que tava(só que ordenado pela pontuação)
        aux->prox = jogadoresOrdenados->prox;
        aux->prox->ant = aux;
    }
    free(jogadoresOrdenados);
}

void salvarNoArquivoJogadores(FILE **rank, TPJogador *jogadores){
    TPJogador *aux;
    aux = jogadores;
    while(aux->prox != NULL){
        aux = aux->prox;
        fprintf(*rank, "%s", aux->nome);
        fprintf(*rank, "%s", "\r\n");
        fprintf(*rank, "%d", aux->pont);
        fprintf(*rank, "%s", "\r\n");
        fprintf(*rank, "%d", aux->qtdMov);
        fprintf(*rank, "%s", "\r\n");
    }
}

void imprimirJogadores(TPJogador *jogador){
    int cont=0;
    TPJogador *aux;
    aux = jogador->prox;
    if(aux == NULL){
        printf("\nSem jogadores no RANK.\n");
        return ;
    }
    printf("   JOGADOR\tPONTOS\t MOVIMENTOS\n");
    while(aux != NULL){
        printf("%dº %s\t  %d\t    %d\n",(cont+1), aux->nome, aux->pont, aux->qtdMov);
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

void imprimirHistoricoJogadas(TPHistorico *historico){
    TPHistorico *aux;
    int totalRegistros=0;
    aux = historico->prox;
    printf("   P.Origem\tNaipe/Valor     P.Destino\n");
    printf("-------------------------------------------\n");
    // caso o proximo seja diferente de null, ele mostra aqui a posição 0, tendo a possibilidade de volltar
    //para o inicio do jogo.
    if(aux != NULL){ 
        printf("0º  (VOLTAR PARA O INICIO DO JOGO)\n");
    }
    while(aux != NULL){
        // caso seja um MovEspecial, entra nesse if
        //MovEspecial é aquele onde é reposto o estoque.
        if(aux->movEspecial == 'S'){ 
            totalRegistros++;
            printf("%dº  REPOSIÇÂO DE ESTOQUE\n", totalRegistros);
            aux = aux->prox;
        }else{
            totalRegistros++;// colocar para imprimir o nome referente as pilhas
            printf("%dº  ",totalRegistros);
            imprimirHistoricoJogadasNomePilhas(aux->pilhaOrigem);
            printf("\t    ");
            verificarNaipeValorHistoricoJogada(aux->carta->naipe, aux->carta->valor);
            printf("      \t");
            imprimirHistoricoJogadasNomePilhas(aux->pilhaDestino);
            printf("\n");                
            aux = aux->prox;
        }
    }
    printf("-------------------------------------------\n\n");
}

void imprimirHistoricoJogadasNomePilhas(int valor){
    if(valor < 7){
        printf("PILHA(%d)", (valor + 1));
    }else if(valor == 7){
        printf("ESTOQUE");
    }else if(valor == 8){
        printf("DESCARTE");
    }else if(valor > 8){
        switch(valor){
            case 9:
                printf("MONTAGEM(1)");
                break;
            case 10:
                printf("MONTAGEM(2)");
                break;
            case 11:
                printf("MONTAGEM(3)");
                break;
            case 12:
                printf("MONTAGEM(4)");
        }
    }
}

void verificarNaipeValorHistoricoJogada(char naipe, int valor){
    if(naipe == 'O' || naipe == 'C'){
       printf("\033[31m%c/%d\033[39m", naipe, valor);   
    }else{
       printf("%c/%d", naipe, valor);    
    }
}

int verificarPontosJogada(int origem, int destino){
   if(origem == 8 && destino >= 9){
       return 10;
   }else if(origem == 8 && destino <=6){
       return 5;
   }else if(origem <= 6 && destino >=9){
       return 20;
   }else if(origem <= 6 && destino <= 6){
       return 5;
   }else if(origem >= 9 && destino <= 6){
       return -15;
   }
}

int contarQTDMovimento(TPHistorico *historico){
    TPHistorico *aux;
    int cont=0;
    aux = historico->prox;
    while(aux != NULL){
        aux = aux->prox;
        cont++;
    }
    return cont;
}

int verificarValorPilhaMovimento(int origem, char id ){
    if(id == 'M'){
        if((origem > 4 || origem <= 0)&& id == 'M')return -1;
    }else{
        if((origem > 7 || origem <= 0)&& id == 'P')return -1;
    }
    return 0;
}

void salvarJogador(TPJogador *jogadores, char *nomeJogador, int pontos, int mov){
    TPJogador *novoJogador, *aux;
    novoJogador = malloc(sizeof(TPJogador));
    novoJogador->prox = NULL;
    for(int i=0; i<strlen(nomeJogador); i++)novoJogador->nome[i] = nomeJogador[i];
    novoJogador->pont = pontos;
    novoJogador->qtdMov = mov;
    
    aux = jogadores;
    while(aux->prox != NULL)aux = aux->prox;
    
    aux->prox = novoJogador;
    novoJogador->ant = aux;
}

void simularDoisPassos(TPPilhaCarta *pilhas){
    TPCarta *aux, *aux2;
    int i,j, cont=0;
    printf("\n");
    for(i=0; i<7; i++){
        aux = pilhas[i].carta;
        while(aux->prox != NULL){
            if(aux->visivel == 'S')break;
            aux = aux->prox;
        }
        j=0;
        if(j == i)j++;
        while(j!= i && j<7){
            aux2 = pilhas[j].carta;
            while(aux2->prox != NULL)aux2 = aux2->prox;
            if(aux->cor != aux2->cor && (aux->valor+1) == aux2->valor && aux->valor != 13){
                if(aux->cor == 'V'){
                    printf("Mova a CARTA \033[31m%c|%d\033[39m da PILHA %d para a PILHA %d\n", aux->naipe, aux->valor,(i+1), (j+1));
                }else{
                    printf("Mova a CARTA %c|%d da PILHA %d para a PILHA %d\n", aux->naipe, aux->valor,(i+1), (j+1));
                }
                cont++;
            }else if(aux->valor == 13 && aux2->valor == -1){
                if(aux->cor == 'V'){
                    printf("Mova a CARTA \033[31m%c|%d\033[39m da PILHA %d para a PILHA %d\n", aux->naipe, aux->valor,(i+1), (j+1));
                }else{
                    printf("Mova a CARTA %c|%d da PILHA %d para a PILHA %d\n", aux->naipe, aux->valor,(i+1), (j+1));
                }
                cont++;
            }
            j++;
            if(j == i)j++;
        }
    }
    if(cont == 0){
        printf("Não há dicas no momento :(\n");
        sleep(3);
    }else{
        printf("\nPressione Enter...\n");
        getchar();
    }
}