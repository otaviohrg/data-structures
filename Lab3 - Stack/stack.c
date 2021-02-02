/********************************************/
/* Aluno: Otávio Guimarães                  */
/* CES-11: Algoritmos e Estruturas de Dados */
/* Prof. Fábio Carneiro Mokarzel            */
/********************************************/

/*	Inclusao de bibliotecas de C	*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

/* Declaracoes para o tipo logic  */

typedef char logic;
const logic TRUE = 1, FALSE = 0;

/*	Definicao dos tipos de atomos	*/

#define NUM 		1
#define OPER		2
#define ABPAR		3
#define FPAR		4
#define INVAL		5

/*	Declaracoes de tipos para as expressoes e listas de atomos	*/

typedef char expressao[100];

typedef union atribatomo atribatomo;
union atribatomo {
    double valor; char carac;
};

typedef struct atomo atomo;
struct atomo {
    int tipo; atribatomo atrib;
};

typedef struct listaatomos listaatomos;
struct listaatomos {
    int natom; atomo Vetor[50];
};

typedef struct noh noh;
typedef noh *pilha;
struct noh {atomo elem; noh *prox;};

/*	Variaveis globais  */

int i; expressao Expr;
listaatomos Parentetica;
listaatomos Polonesa;
pilha Poper;
pilha Pol;

/*	Prototipos das funcoes auxiliares	*/

void ArmazenarParentetica (void);
void EscreverListaAtomos (listaatomos *);
void ResolverPolonesa(listaatomos *);
char ProxNaoBranco (void);
char ProxCarac (void);
void InicExpr (void);
logic ParenteticaCorreta(void);
logic TesteRecursivo(void);
void Empilhar (atomo, pilha*);
void Desempilhar (pilha*);
atomo Topo (pilha);
void InicPilha (pilha*);
logic Vazia (pilha);

/*	Funcao main		*/

int main() {
    char c;
    logic VeriParen;
    printf ("Armazenar expressao parentetica? (s/n): ");
    do c = getchar ();
    while (c!='s' && c!='n' && c!='S' && c!='N');
    while (c == 's' || c == 'S') {
        printf ("\n\n"); printf ("Digite a expressao: ");
        setbuf (stdin, NULL); scanf(" %[^\n]s", Expr);
        VeriParen = ParenteticaCorreta();
        if(VeriParen){
            ArmazenarParentetica ();
            printf("\nATOMOS NA FORMA PARENTETICA\n");
            EscreverListaAtomos (&Parentetica);
            printf("\nATOMOS NA FORMA POLONESA\n");
            EscreverListaAtomos(&Polonesa);
            ResolverPolonesa(&Polonesa);
            printf("\nValor obtido do calculo da polones: %f\n", Topo(Pol).atrib.valor);
        }
        else
            printf("Parentetica incorreta\n");
        printf
                ("\n\nArmazenar expressao parentetica? (s/n): ");
        do c = getchar ();
        while (c!='s' && c!='n' && c!='S' && c!='N');
    }
    printf ("\n\n"); printf ("Fim das atividades!");
    printf ("\n\n"); system ("pause"); return 0;
}

void ArmazenarParentetica(){
    InicExpr();
    char ch = Expr[0];
    if(ch == ' ')
        ProxNaoBranco();
    char numstr[20];
    double num;
    int k;
    atomo *atom;
    Parentetica.natom = 0;
    while(ch != '\0'){
        atom = &(Parentetica.Vetor[Parentetica.natom]);
        if(isdigit(ch)){
            k = 0;
            for(int p=0; p<20; p++){
                numstr[p] = '0';
            }
            numstr[0] = ch;
            k++;
            ch = ProxCarac();
            while(isdigit(ch)||ch == '.'){
                numstr[k] = ch;
                ch = ProxCarac();
                k++;
            }
            numstr[k] = '\0';
            sscanf(numstr, "%lf", &num);
            atom->tipo = NUM;
            (atom->atrib).valor = num;
        }
        else if(ch == '+'||ch == '*'||ch == '-'||ch == '/'||ch == '~'||ch == '^'||ch == 'R'||ch == 'L'){
            atom->tipo = OPER;
            (atom->atrib).carac = ch;
            ProxCarac();
        }
        else if(ch=='(') {
            atom->tipo = ABPAR;
            ProxCarac();
        }
        else if(ch==')') {
            atom->tipo = FPAR;
            ProxCarac();
        }
        else{
            atom->tipo = INVAL;
            (atom->atrib).carac = ch;
            ProxCarac();
        }
        (Parentetica.natom)++;
        ch = ProxNaoBranco();
    }
}

void EscreverListaAtomos(listaatomos *lista){
    int tipo;
    printf("TIPO   |  ATRIBUTO  \n--------------------\n");
    for(int k=0; k<lista->natom; k++){
        tipo = (lista->Vetor[k]).tipo;
        switch(tipo){
            case NUM:       printf("NUM    |  %f\n", (lista->Vetor[k]).atrib.valor); break;
            case ABPAR:     printf("ABPAR  |\n"); break;
            case FPAR:      printf("FPAR   |\n"); break;
            case INVAL:     printf("INVAL  |  %c\n", (lista->Vetor[k]).atrib.carac); break;
            case OPER:      printf("OPER   |  %c\n", (lista->Vetor[k]).atrib.carac); break;
            default:        break;
        }
    }
}

logic ParenteticaCorreta () {
    logic r;
    i = 0;
    InicPilha(&Poper);
    Polonesa.natom = 0;
    r = TesteRecursivo();
    if (Expr[i] != '\0') r = FALSE;
    return r;
}

logic TesteRecursivo () {
    logic r = TRUE;
    atomo x;
    int k;
    logic decimal;
    logic unario = FALSE;
    char numstr[20];
    double num;
    if (Expr[i] == '(') {
        ProxCarac();
        ProxNaoBranco();
        if (isdigit (Expr[i])){
            decimal = FALSE;
            k = 0;
            for(int p=0; p<20; p++){
                numstr[p] = '0';
            }
            numstr[0] = Expr[i];
            k++;
            ProxCarac();
            while(isdigit(Expr[i])||Expr[i] == '.'){
                if(Expr[i] == '.'){
                    if(decimal == FALSE)
                        decimal = TRUE;
                    else
                        r = FALSE;
                }
                numstr[k] = Expr[i];
                ProxCarac();
                k++;
            }
            if(decimal == FALSE)
                numstr[k]='.';
            sscanf(numstr, "%lf", &num);
            (Polonesa.Vetor[Polonesa.natom]).tipo = NUM;
            (Polonesa.Vetor[Polonesa.natom]).atrib.valor = num;
            Polonesa.natom++;
            ProxNaoBranco();
        }
        else if(r && (Expr[i] == '~' || Expr[i] == 'R')){
            x.tipo = OPER;
            x.atrib.carac = Expr[i];
            Empilhar(x,&Poper);
            ProxCarac();
            ProxNaoBranco();
            unario = TRUE;
        }
        else if (Expr[i] == '(')
            r = TesteRecursivo ();
        else r = FALSE;
        if(unario == FALSE){
            if (r && (Expr[i] == '+' || Expr[i] == '*' || Expr[i] == '-' || Expr[i] == '/' || Expr[i] == '^' ||
                      Expr[i] == 'L')) {
                x.tipo = OPER;
                x.atrib.carac = Expr[i];
                Empilhar(x, &Poper);
                ProxCarac();
                ProxNaoBranco();
            } else r = FALSE;
        }
        if (r && isdigit (Expr[i])){
            decimal = FALSE;
            k = 0;
            for(int p=0; p<20; p++){
                numstr[p] = '0';
            }
            numstr[0] = Expr[i];
            k++;
            ProxCarac();
            while(isdigit(Expr[i])||Expr[i] == '.'){
                if(Expr[i] == '.'){
                    if(decimal == FALSE)
                        decimal = TRUE;
                    else
                        r = FALSE;
                }
                numstr[k] = Expr[i];
                ProxCarac();
                k++;
            }
            if(decimal == FALSE)
                numstr[k]='.';
            sscanf(numstr, "%lf", &num);
            (Polonesa.Vetor[Polonesa.natom]).tipo = NUM;
            (Polonesa.Vetor[Polonesa.natom]).atrib.valor = num;
            Polonesa.natom++;
            ProxNaoBranco();
        }
        else if (r && Expr[i] == '(')
            r = TesteRecursivo ();
        else r = FALSE;
        if (r && Expr[i] == ')'){
            ProxCarac();
            ProxNaoBranco();
            (Polonesa.Vetor[Polonesa.natom]) = Topo(Poper);
            Polonesa.natom++;
            Desempilhar(&Poper);
        }
        else r = FALSE;
    }
    else r = FALSE;
    return r;
}

void ResolverPolonesa(listaatomos *lista) {
    InicPilha(&Pol);
    atomo x, y;
    char ch;
    double num1, num2, numres = 0;
    y.tipo = NUM;
    for (int k = 0; k < lista->natom; k++) {
        x = lista->Vetor[k];
        if (x.tipo == NUM)
            Empilhar(x, &Pol);
        else if (x.tipo == OPER) {
            ch = x.atrib.carac;
            if (ch == '~') {
                numres = Topo(Pol).atrib.valor;
                Desempilhar(&Pol);
                numres = -numres;
            }
            else if (ch == 'R') {
                numres = Topo(Pol).atrib.valor;
                Desempilhar(&Pol);
                numres = sqrt(numres);
            }
            else {
                num1 = Topo(Pol).atrib.valor;
                Desempilhar(&Pol);
                num2 = Topo(Pol).atrib.valor;
                Desempilhar(&Pol);
                switch (ch) {
                    case '+':
                        numres = num1 + num2;
                        break;
                    case '*':
                        numres = num1 * num2;
                        break;
                    case '-':
                        numres = num2 - num1;
                        break;
                    case '/':
                        numres = num2 / num1;
                        break;
                    case '^':
                        numres = pow(num2, num1);
                        break;
                    case 'L':
                        numres = log(num2)/log(num1);
                        break;
                    default:
                        break;
                }
            }
            y.atrib.valor = numres;
            Empilhar(y, &Pol);
        }
    }
}

/*Operadores do TAD*/
void Empilhar (atomo x, pilha *P) {
    noh *temp;
    temp = *P;
    *P = (noh *) malloc (sizeof (noh));
    (*P)->elem = x; (*P)->prox = temp;
}

void Desempilhar (pilha *P) {
    noh *temp;
    if (! Vazia(*P)){
        temp = *P;
        *P = (*P)->prox;
        free (temp);
    }
}

atomo Topo (pilha P) {
    if (! Vazia(P))
        return P->elem;
}

void InicPilha (pilha *P) {
    *P = NULL;
}

logic Vazia (pilha P) {
    if (P == NULL)
        return TRUE;
    else
        return FALSE;
}

/*   Funcoes para percorrer a expressao     */
char ProxNaoBranco(){
    while (isspace (Expr[i]) ||
           (iscntrl (Expr[i]) && Expr[i] != '\0')) i++;
    return Expr[i];
}

char ProxCarac(){
    i++;
    return Expr[i];
}

void InicExpr(){
    i = 0;
}