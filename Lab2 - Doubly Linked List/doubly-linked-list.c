/********************************************/
/* Aluno: Otávio Guimarães                  */
/* CES-11: Algoritmos e Estruturas de Dados */
/* Prof. Fábio Carneiro Mokarzel            */
/********************************************/

/* Inclusao de bibliotecas da linguagem */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*  Declaracoes de uso geral            */

typedef char logic;
const logic True = 1, False = 0;
typedef char cadeia[101];

/*  Declaracoes de tipos para a estrutura duplamente encadeada do TAD numero */

typedef struct noh noh;
struct noh{
    int num;
    noh *next;
    noh *prev;
};
typedef noh *numero;
typedef noh *posicao;

/*  Prototipos das funcoes operadoras para o TAD numero    */

void InicializarNumero (numero*);
void EsvaziarNumero (numero*);
logic NumeroNulo (numero);
posicao Primeira (numero);
void Inserir (int, posicao, numero*);
posicao Proxima (posicao, numero);
posicao Previa (posicao, numero);
posicao Fim (numero);
int Elemento (posicao, numero);

/*  Prototipos de funcoes independentes da estrutura para o TAD numero  */

numero NovoNumero (void);
void EscreverNumero (numero);
numero SomaNumeros (numero, numero);
numero MultiplicaNumeros (numero, numero);

/*  Funcao main: cabecalho, declaracoes e escrita de titulo    */

int main () {
	numero N1, N2, NS, NM;
	printf ("PROGRAMA PARA MANIPULAR NUMEROS INTEIROS MUITO GRANDES EM LISTA LINEAR");

/*  Leitura e armazenamento de dois numeros    */

	printf ("\n\nDigite o 1o numero: ");
	N1 = NovoNumero();
	printf ("\n\nDigite o 2o numero: ");
	N2 = NovoNumero();

/*  Escrita do conteudo dos dois numeros    */

    printf ("\n\nConteudo do 1o numero: ");
    EscreverNumero(N1);
    printf ("\n\nConteudo do 2o numero: ");
    EscreverNumero(N2);

/*  Soma dos dois numeros e sua escrita    */

    NS = SomaNumeros (N1, N2);
	printf ("\n\nSoma dos numeros: "); EscreverNumero (NS);

/*  Multiplicacao dos dois numeros e sua escrita    */

    NM = MultiplicaNumeros (N1, N2);
	printf ("\n\nProduto dos numeros: "); EscreverNumero (NM);


/*  Encerramento    */

	printf ("\n\n"); system ("pause"); return 0;
}

/*  Funcao NovoNumero: faz a leitura dos numeros e os
    	insere numa lista a ser retornada                       */

numero NovoNumero () {
    numero N;
    posicao p;
    logic r = True;
    InicializarNumero(&N);
    cadeia numinput;
    int par, elem;
    char *check, *ptreffec, subptr[5];
    p = Primeira(N);
    scanf("%s", numinput);
    check = numinput;
    while(*check != '\0' && r){
        if(isdigit(*check)==0)
            r = False;
        check++;
    }
    if(!r)
        Inserir(0, p, &N);
    else{
        ptreffec = &numinput[0];
        while(*ptreffec == '0' && *(ptreffec+1)!='\0')
            ptreffec = ptreffec+1;
        par = strlen(ptreffec)%4;
        if(par==0)
            par=4;
        for(int k = 0; k<par; k++)
            *(subptr+k) = *(ptreffec+k);
        *(subptr+par)='\0';
        ptreffec = ptreffec+par;
        elem = atoi(subptr);
        Inserir(elem, p, &N);
        p = Proxima (p, N);
        while(strlen(ptreffec) != 0){
            for(int k = 0; k<4; k++)
                *(subptr+k) = *(ptreffec+k);
            *(subptr+4)='\0';
            ptreffec = ptreffec+4;
            elem = atoi(subptr);
            Inserir(elem, p, &N);
            p = Proxima (p, N);
        }
    }
    return N;
}

/*  Funcao EscreverNumero: escreve o conteudo de um numero   */

void EscreverNumero (numero N) {
    for(posicao pos = Primeira(N); Proxima(pos, N) != Primeira(N); pos = Proxima(pos, N))
        if(pos == N)
            printf("%d", Elemento(Proxima(pos, N), N));
        else
            printf("%04d", Elemento(Proxima(pos, N), N));
}

/*  Funcao SomaNumeros: soma dois numeros armazenados numa
    	lista linear e retorna o numero soma                    */

numero SomaNumeros (numero N1, numero N2) {
    numero NS;
    posicao p1, p2;
    int elem1, elem2, elems, carry;
    p1 = Fim(N1);
    p2 = Fim(N2);
    carry = 0;
    InicializarNumero(&NS);
    do{
        if(p1!=Primeira(N1)){
            elem1 = Elemento(p1, N1);
            p1 = Previa(p1, N1);
        }
        else
            elem1 = 0;
        if(p2!=Primeira(N2)){
            elem2 = Elemento(p2, N2);
            p2 = Previa(p2, N2);
        }
        else
            elem2 = 0;
        elems = (elem1+elem2+carry)%10000;
        carry = (elem1+elem2+carry)/10000;
        Inserir(elems, Primeira(NS), &NS);
    }while(p1!=Primeira(N1) || p2!=Primeira(N2) || carry!=0);
    return NS;
}

/*  Funcao MultiplicaNumeros: multiplica dois numeros armazenados 	numa lista linear e retorna o numero produto             */

numero MultiplicaNumeros (numero N1, numero N2) {
    numero NM, NP;
    posicao p1, p2;
    int elem1, elem2, elemm, carry, disloc;
    disloc = 0;
    p1 = Fim(N1);
    InicializarNumero(&NM);
    if(NumeroNulo(N1) == True)
        return N1;
    else if(NumeroNulo(N2) == True)
        return N2;
    InicializarNumero(&NP);
    while(p1 != Primeira(N1)){
        p2 = Fim(N2);
        EsvaziarNumero(&NP);
        carry = 0;
        elem1 = Elemento(p1, N1);
        for(int i=0; i<disloc; i++)
            Inserir(0, Proxima(NP, NP), &NP);
        while(p2 != Primeira(N2) || carry != 0){
            if(p2 == Primeira(N2))
                elem2 = 0;
            else{
                elem2 = Elemento(p2, N2);
                p2 = Previa(p2, N2);
            }
            elemm = (elem1*elem2+carry)%10000;
            carry = (elem1*elem2+carry)/10000;
            Inserir(elemm, Primeira(NP), &NP);
        }
        NM = SomaNumeros(NP, NM);
        p1 = Previa(p1, N1);
        disloc++;
    }
    return NM;
}

/*  FUNCOES OPERADORAS DE NUMERO NA ESTRUTURA DUPLAMENTE ENCADEADA   */

void InicializarNumero (numero *N) {
    *N = (numero)malloc(sizeof(noh));
    (*N)->num = 0;
    (*N)->next = *N;
    (*N)->prev = *N;
}

void EsvaziarNumero (numero *N) {
    posicao remove;
    while((*N)->next != *N){
        remove = (*N)->next;
        (*N)->next = (*N)->next->next;
        (*N)->next->prev = *N;
        free(remove);
    }
}

logic NumeroNulo (numero N) {
    logic r = True;
    for(posicao pos = N; pos->next!= N && r; pos = pos->next)
        if(N->next->num != 0)
            r = False;
    return r;
}

posicao Primeira (numero N) {return N;}

void Inserir (int num, posicao p, numero *N) {
    posicao proxima = NULL;
    posicao insert;
    proxima = p->next;
    insert = malloc(sizeof(noh));
    insert->num = num;
    insert->prev = p;
    insert->next = proxima;
    p->next = insert;
    proxima->prev = insert;
}

posicao Proxima (posicao p, numero N) {return p->next;}

posicao Previa (posicao p, numero N) {return p->prev;}

posicao Fim (numero N) {return N->prev;}

int Elemento (posicao p, numero N) {return p->num;}
