/********************************************/
/* Aluno: Otávio Guimarães                  */
/* CES-11: Algoritmos e Estruturas de Dados */
/* Prof. Fábio Carneiro Mokarzel            */
/********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef char logic;
const logic TRUE = 1, FALSE = 0;

/*		Declaracoes de tipos para encadeamento de pais e 	irmaos para arvores    */
typedef struct celula celula;
const celula *nulo = NULL;
typedef char informacao;
typedef celula *noh;
typedef celula *arvore;
struct celula {informacao info; noh pai, fesq, idir;};

/*		Declaracoes de tipos para estrutura encadeada de 	filas de nohs de arvores   */
typedef struct celfila celfila;
typedef struct fila fila;
struct celfila {noh elem; celfila *prox;};
struct fila {celfila *fr, *tr;};

/*		Declaracoes de tipos para floresta, que eh uma lista 	encadeada de arvores, com elemento-lider    */
typedef char string[100];
typedef struct celfloresta celfloresta;
typedef struct celfloresta *floresta;
typedef struct celfloresta *posicfloresta;
struct celfloresta {
    arvore elem; celfloresta *prox;
};

/*  	Variaveis globais  */

floresta F;
string StrFloresta;  // guarda os caracteres digitados

/*  	Prototipos das funcoes dependentes da estrutura para arvores  */

void FormarListaNohs (void);
void EscreverFloresta (void);
void LigarPaisFilhos (void);
arvore Criacao (informacao, floresta);
int Largura (arvore);
int Altura (arvore);

/*  	Prototipos das funcoes operadoras de arvores  */

logic ArvVazia (arvore);
informacao Elemento (noh, arvore);
noh Raiz (arvore);
noh FilhoEsquerdo (noh, arvore);
noh IrmaoDireito (noh, arvore);

/*  	Prototipos das funcoes independentes da estrutura para arvores  */

void EscreverParentetica (arvore);

/*  	Prototipos das funcoes operadoras de filas  */

void InitFila (fila *);
void EntrarFila (noh, fila *);
noh DeletarFila (fila *);
char FilaVazia (fila);

int usado[75] = {0};

/*		Programa principal: le, forma e escreve uma floresta  */

int main () {
    informacao c; arvore A;
    printf ("Leitura e armazenamento dos nohs da floresta F:\n\n");
    FormarListaNohs ();
    if (F->prox != NULL) {
        printf ("\nLeitura e ligacao dos pares pais-filhos:\n\n");
        LigarPaisFilhos ();
    }
    printf ("\n\nArvores da floresta F:\n\n");
    EscreverFloresta ();
    printf ("\n\nCriacao de nova arvore:\n\n\t");
    printf ("Digite a informacao da raiz: ");
    scanf(" %c", &c);
    while(isalnum(c) == 0 || usado[c-'0'] != 0){
        printf("\nInformação invalida. Insira outra.");
        scanf(" %c", &c);
    }
    A = Criacao (c, F);
    printf ("\n\n"); EscreverParentetica (A);
    printf("\nAltura: %d\t Largura: %d", Altura(A), Largura(A));
    printf ("\n\n"); system ("pause"); return 0;
}

/*		Funcao FormarListaNohs: le as informacoes dos nohs 	de uma floresta armazenando-as em celulas; forma uma 	lista linear de nohs ponteiros para essas celulas; 	essa lista eh a floresta F (global)   */

void FormarListaNohs () {
    F = malloc(sizeof(celfloresta));
    posicfloresta ptr = F;
    noh X;
    scanf("%s", StrFloresta);
    char *ch;
    ch = StrFloresta;
    while(*ch != '\0'){
        if(isalnum(*ch) != 0 && usado[*ch-'0'] == 0){
            usado[*ch-'0'] = 1;
            X = malloc(sizeof(celula));
            X->pai = NULL;
            X->info = *ch;
            ptr->prox = malloc(sizeof(celfloresta));
            ptr = ptr->prox;
            ptr->prox = NULL;
            ptr->elem = X;
        }
        ch++;
    }
}

/*		Funcao EscreverFloresta: escreve os nos de todas as
		arvores da floresta F em ordem de nivel */

void EscreverFloresta () {
    posicfloresta ptr;
    for(ptr = F; ptr->prox != NULL; ptr = ptr->prox){
        EscreverParentetica(ptr->prox->elem);
        printf("\n");
        printf("Altura: %d\t", Altura(ptr->prox->elem));
        printf("Largura: %d\n", Largura(ptr->prox->elem));
    }
}

void LigarPaisFilhos (void){
    printf("Escreva os pares na forma de dois caracteres. Separe os pares por espacos.\n");
    char pares[100];
    char *par;
    posicfloresta ptr, ptrpai, ptrfilho;
    noh ptrnoh;
    char infopai, infofilho;
    scanf(" %[^\n]s", pares);
    par = pares;
    while(*par != '\0'){
        ptrpai = NULL;
        ptrfilho = NULL;
        infopai = *par;
        infofilho = *(par+1);
        for(ptr = F; ptr->prox != NULL; ptr = ptr->prox){
            if(Elemento(ptr->prox->elem, ptr->prox->elem) == infopai)
                ptrpai = ptr->prox;
            if(Elemento(ptr->prox->elem, ptr->prox->elem) == infofilho)
                ptrfilho = ptr->prox;
        }
        if(ptrpai != NULL && ptrfilho != NULL && ptrfilho->elem->pai == NULL) {
            ptrnoh = ptrfilho->elem->pai;
            while(ptrnoh != NULL && ptrnoh != ptrfilho->elem){
                ptrnoh = ptrnoh->pai;
            }
            if (ptrnoh != ptrfilho->elem) {
                if(ptrpai->elem->fesq == NULL)
                    ptrpai->elem->fesq = ptrfilho->elem;
                else{
                    noh nohirmao = ptrpai->elem->fesq;
                    while(nohirmao->idir != NULL)
                        nohirmao = nohirmao->idir;
                    nohirmao->idir = ptrfilho->elem;
                }
                ptrfilho->elem->pai = ptrpai->elem;
            }
        }
        par = par+3;
    }
    ptr = F;
    while(ptr->prox != NULL){
        if(ptr->prox->elem->pai != NULL)
            ptr->prox = ptr->prox->prox;
        else
            ptr = ptr->prox;
    }
}

arvore Criacao (informacao c, floresta fl){
    posicfloresta ptr, ptrfree;
    arvore A;
    A = malloc(sizeof(celula));
    A->idir = NULL;
    A->fesq = NULL;
    A->info = c;
    for(ptr=fl; ptr->prox != NULL; ptr=ptr->prox){
        ptr->prox->elem->pai = A;
        if(ptr == fl)
            A->fesq = ptr->prox->elem;
        else{
            noh nohirmao = A->fesq;
            while(nohirmao->idir != NULL)
                nohirmao = nohirmao->idir;
            nohirmao->idir = ptr->prox->elem;
        }
    }
    while(F->prox != NULL){
        ptrfree = F->prox;
        F->prox=F->prox->prox;
        free(ptrfree);
    }
    return A;
}

int Altura (arvore A){
    int recalt;
    int altmax = 1;
    if(ArvVazia(A) == TRUE) return 0;
    else{
        for(arvore arv = A; A != NULL; A = A->idir){
            recalt = 1;
            if(arv->fesq != NULL)
                recalt += Altura(arv->fesq);
            if(recalt > altmax) altmax = recalt;
        }
        return altmax;
    }
}

int Largura (arvore A) {
    fila fp, fs;
    noh x, y;
    int max = 0;
    int lengniv;
    if (ArvVazia (A) == TRUE) return max;
    else {
        InitFila (&fs);
        EntrarFila (Raiz (A), &fs);
        do {
            fp = fs;
            InitFila (&fs);
            lengniv = 0;
            while (FilaVazia(fp) == FALSE) {
                x = DeletarFila(&fp);
                lengniv++;
                for (y = FilhoEsquerdo (x, A); y != nulo;
                     y = IrmaoDireito(y, A))
                    EntrarFila (y, &fs);
            }
            if(lengniv>max) max = lengniv;
        } while (FilaVazia(fs) == FALSE);
    }
    return lengniv;
}

void EscreverParentetica (arvore A){
    if (ArvVazia(A) == TRUE) printf("  Arvore vazia");
    else{
        printf("(");
        printf("%c", A->info);
        if(A->fesq != NULL) EscreverParentetica(A->fesq);
        printf(")");
        if(A->idir != NULL) EscreverParentetica(A->idir);
    }
}


/*		Funcoes operadoras de arvores           */

logic ArvVazia (arvore A) {
    if (A == NULL) return TRUE;
    else return FALSE;
}

informacao Elemento (noh n, arvore A) {return n->info;}

noh Raiz (arvore A) {return A;}

noh FilhoEsquerdo (noh n, arvore A) {return n->fesq;}

noh IrmaoDireito (noh n, arvore A) {return n->idir;}

/*		Funcoes operadoras de filas de nos de arvores     */

void InitFila (fila *f) {
    f->fr = malloc (sizeof(celfila));
    f->tr = f->fr; f->fr->prox = NULL;
}

void EntrarFila (noh x, fila *f) {
    f->tr->prox = malloc (sizeof(celfila)); f->tr = f->tr->prox;
    f->tr->elem = x; f->tr->prox = NULL;
}

noh DeletarFila (fila *f) {
    noh x; celfila *p;
    p = f->fr->prox; x = p->elem;
    if (f->tr == p) f->tr = f->fr;
    f->fr->prox = p->prox; free (p);
    return x;
}

char FilaVazia (fila f) {
    char c;
    if (f.fr == f.tr) c = TRUE;
    else c = FALSE;
    return c;
}
