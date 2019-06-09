 struct carta{
    int valor;
    char naipe;
    char visivel;
    char cor;
    char simbolo;
    struct carta *ant;
    struct carta *prox;
};


struct pilhaCarta{
    TPCarta *carta;
};


struct jogador{
    char nome[14];
    //int tempo;
    char pont[5];
    char qtdMov[5];
    struct jogador *prox;
    struct jogador *ant;
};


struct historico{
    struct historico *ant;
    struct historico *prox;
    char visivelCartaAnterior;
    char movEspecial;
    TPCarta *carta;
    TPCarta *cartaAnterior;
    int pilhaOrigem;
    int pilhaDestino;
};
	