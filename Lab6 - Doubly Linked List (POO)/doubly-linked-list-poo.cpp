/********************************************/
/* Aluno: Otávio Guimarães                  */
/* CES-11: Algoritmos e Estruturas de Dados */
/* Prof. Fábio Carneiro Mokarzel            */
/********************************************/

#include<iostream>
#include <iomanip>
#include<stdlib.h>
#include<string>
#include<ctype.h>

using namespace std;

typedef char logic;
const logic True = 1, False = 0;
typedef struct noh noh;
typedef noh *pont, *posicao;
struct noh {int num; pont prox, prev;};

class numero {
    private:
        noh *num;
    private:
        void InicializarNumero (void);
        void EsvaziarNumero (void);
        logic NumeroNulo (void);
        posicao Primeira (void);
        void Inserir (int, posicao);
        posicao Proxima (posicao);
        posicao Previa (posicao);
        posicao Fim (void);
        int Elemento (posicao);
        numero AutoCopia (void);
        void Copia (numero);
    public:
        void NovoNumero (void);
        void EscreverNumero (void);
        void SomarNumeros (numero, numero);
        void MultiplicarNumeros (numero, numero);
};

class cadeia {
	private:
		char str[200];
	public:
		void Ler (void);
		void Escrever (void);
        char PegarChar (int);
        void MudarChar (int, char);
        int Comprimento (void);
        logic Decimal (void);
};

int main () {
	numero N1, N2, NS, NM;
	cout <<
		"POO PARA MANIPULAR NUMEROS INTEIROS MUITO GRANDES EM LISTA LINEAR";

/*  Leitura e armazenamento de dois numeros    */

	cout << endl << endl << "Digite o 1o numero: ";
	N1.NovoNumero();
	cout << endl << endl << "Digite o 2o numero: ";
	N2.NovoNumero();

/*  Escrita do conteudo dos dois numeros    */

    cout << endl << endl << "Conteudo do 1o numero: ";
    N1.EscreverNumero();
    cout << endl << endl << "Conteudo do 2o numero: ";
    N2.EscreverNumero();

/*  Soma dos dois numeros e sua escrita    */

    NS.SomarNumeros (N1, N2);
	cout << endl << endl << "Soma dos numeros: ";
    NS.EscreverNumero();

/*  Multiplicacao dos dois numeros e sua escrita    */

    NM.MultiplicarNumeros (N1, N2);
	cout << endl << endl << "Produto dos numeros: ";
	NM.EscreverNumero();

/*  Encerramento    */

	cout << "\n\n";
	system ("pause");
	return 0;
}

/*  Metodos privados da classe numero   */

void numero::InicializarNumero (void){
    num = (noh*)malloc(sizeof(noh));
    num->num = 0;
    num->prox = num;
    num->prev = num;
}

void numero::EsvaziarNumero (void){
    posicao removen;
    while(num->prox != num){
        removen = num->prox;
        num->prox = num->prox->prox;
        num->prox->prev = num;
        free(removen);
    }
}

logic numero::NumeroNulo (void){
    logic r = True;
    for(posicao pos = num; pos->prox!= num && r; pos = pos->prox)
        if(num->prox->num != 0)
            r = False;
    return r;
}

posicao numero::Primeira (void){return num;}

void numero::Inserir (int elem, posicao p){
    posicao proxima = NULL;
    posicao insertn;
    proxima = p->prox;
    insertn = (noh*)malloc(sizeof(noh));
    insertn->num = elem;
    insertn->prev = p;
    insertn->prox = proxima;
    p->prox = insertn;
    proxima->prev = insertn;
}

posicao numero::Proxima (posicao p){return p->prox;}

posicao numero::Previa (posicao p){return p->prev;}

posicao numero::Fim (void){return num->prev;}

int numero::Elemento (posicao p){return p->num;}

numero numero::AutoCopia (void){
    numero N;
    N.num = num;
    return N;
}

void numero::Copia (numero N){num = N.num;}

/*  Metodos públicos da classe Numero   */

void numero::NovoNumero (void){
    int nremoved=0, len, par, elem;
    char substring[5];
    posicao p;
    InicializarNumero();
    cadeia numinput;
    numinput.Ler();
    p = Primeira();
    len = numinput.Comprimento();
    if(!numinput.Decimal())
        Inserir(0,p);
    else{
        while(numinput.PegarChar(nremoved)=='0' && numinput.PegarChar(nremoved+1) != '\0') nremoved++;
        par = (len-nremoved)%4;
        if(par == 0) par = 4;
        for(int k=0; k<par; k++)
            substring[k] = numinput.PegarChar(nremoved+k);
        substring[par]='\0';
        nremoved += par;
        elem = atoi(substring);
        Inserir(elem, p);
        p = Proxima(p);
        while(numinput.PegarChar(nremoved)!='\0'){
            for(int k = 0; k<4; k++)
                substring[k] = numinput.PegarChar(nremoved+k);
            substring[4]='\0';
            nremoved += 4;
            elem = atoi(substring);
            Inserir(elem, p);
            p = Proxima (p);
        }
    }
}

void numero::EscreverNumero (void){
    for(posicao pos = Primeira(); Proxima(pos) != Primeira(); pos = Proxima(pos))
        if(pos == Primeira())
            cout << Elemento(Proxima(pos));
        else
            cout << setfill('0') << setw(4) << Elemento(Proxima(pos));
}

void numero::SomarNumeros (numero N1, numero N2) {
    InicializarNumero();
    posicao p1, p2;
    p1 = N1.Fim();
    p2 = N2.Fim();
    int elem1, elem2, elems, carry;
    carry = 0;
    do{
        if(p1!=N1.Primeira()){
            elem1 = N1.Elemento(p1);
            p1 = N1.Previa(p1);
        }
        else
            elem1 = 0;
        if(p2!=N2.Primeira()){
            elem2 = N2.Elemento(p2);
            p2 = N2.Previa(p2);
        }
        else
            elem2 = 0;
        elems = (elem1+elem2+carry)%10000;
        carry = (elem1+elem2+carry)/10000;
        Inserir(elems, Primeira());
    }while(p1!=N1.Primeira() || p2!=N2.Primeira() || carry!=0);
}

void numero::MultiplicarNumeros (numero N1, numero N2) {
    InicializarNumero();
    numero NP;
    int elem1, elem2, elemm, carry, disloc;
    posicao p1, p2;
    disloc = 0;
    p1 = N1.Fim();
    if(N1.NumeroNulo())
        Copia(N1);
    else if(N2.NumeroNulo())
        Copia(N2);
    else{
        NP.InicializarNumero();
        while(p1 != N1.Primeira()){
            NP.EsvaziarNumero();
            p2 = N2.Fim();
            carry = 0;
            elem1 = N1.Elemento(p1);
            for(int i=0; i<disloc; i++)
                NP.Inserir(0, NP.Proxima(NP.Primeira()));
            while(p2 != N2.Primeira() || carry != 0){
                if(p2 == N2.Primeira())
                    elem2 = 0;
                else{
                    elem2 = N2.Elemento(p2);
                    p2 = N2.Previa(p2);
                }
                elemm = (elem1*elem2+carry)%10000;
                carry = (elem1*elem2+carry)/10000;
                NP.Inserir(elemm, NP.Primeira());
            }
            SomarNumeros(NP, AutoCopia());
            p1 = N1.Previa(p1);
            disloc++;
        }
    }
}
/*  Metodos públicos da classe Cadeia   */

void cadeia::Ler (void){cin >> str;}

void cadeia::Escrever (void){cout << str;}

char cadeia::PegarChar (int i){return str[i];}

void cadeia::MudarChar (int i, char c){str[i]=c;}

int cadeia::Comprimento (void){
    char *ch;
    ch = str;
    int len=0;
    while(*ch != '\0'){
        len++;
        ch++;
    }
    return len;
};

logic cadeia::Decimal (void){
    logic r = True;
    char *check;
    check = str;
    while(*check != '\0' && r){
        if(isdigit(*check)==0)
            r = False;
        check++;
    }
    return r;
}
