/********************************************/
/* Aluno: Otávio Guimarães                  */
/* CES-11: Algoritmos e Estruturas de Dados */
/* Prof. Fábio Carneiro Mokarzel            */
/********************************************/

#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#define infinito INT_MAX

/*  Declaracoes de uso geral            */
typedef char logic;
const logic True = 1, False = 0;

/*  Declaracoes referentes a estrutura do digrafo */
typedef int vertice;
typedef struct CelulaAdj CelulaAdj;
typedef struct CelulaVertice CelulaVertice;
typedef struct Grafo Grafo;

struct CelulaAdj {
    vertice vert;
    int custo;
    logic first;
    CelulaAdj *prox;
};
struct CelulaVertice {
    logic visit;
    int cfc;
    int nvisit;
    CelulaAdj *listadj, *listcontradj;
};
struct Grafo {
    CelulaVertice *EspacoVertices;
    int nvert;
    logic conexo;
};

/*  Declaracoes referentes ao Algoritmo de Dijkstra */
typedef struct Dijkstra Dijkstra;
typedef struct caminho caminho;
typedef struct noh noh;
typedef struct noh *posicao;

struct noh {
    vertice elem;
    posicao prox;
};

struct caminho {
    posicao inic, fim;
};

struct Dijkstra {
    int custo;
    logic definitivo;
    caminho path;
};

/*	 Variaveis globais */
Grafo G;
FILE *filein;
Dijkstra *D;
int cont = 0;

/*  Prototipos de funcoes operadoras do Digrafo*/
int LerGrafo (Grafo *, int);
logic LinhaValida ();
void EscreverGrafo (Grafo *, logic);
void InicializarGrafo (Grafo *, int);
void LimparGrafo (Grafo *);
logic CaminhoRepetido (Grafo *, int, int);
void NovoAdjunto (Grafo *, int, int, int);
void CriarContraAdjuntos (Grafo *);
void Desorientar(Grafo *);
void TestarConectividade(Grafo *);
int CustoArco(vertice, vertice, Grafo *);

/*  Prototipos de funcoes operadoras do Algoritmo de Dijkstra*/
void AlgoritmoDijkstra(Grafo *);
void InicializarDijkstra (Grafo *);
void CalcDijkstraDef(Grafo *);
void EscreverDijkstra(Grafo *);
int Minimo(Dijkstra *, int);
int SomarCaminhos(Dijkstra *, Grafo *, int, int);
void ImprimirCusto(Dijkstra *, int);
void LimparDijkstra();

/*  Prototipos de funcoes operadoras de caminhos */
void InicCaminho (caminho *);
posicao Primeira (caminho);
posicao Proxima (posicao);
posicao Fim (caminho);
void InserirNoFinal (int, caminho *);
int Elemento (posicao, caminho);
void EscreverCaminho (caminho);
void CopiarCaminho (caminho *, caminho);
void EsvaziarCaminho (caminho *);

/*  Prototipos de funções operadoras do Algoritmo de Kosaraju */
void AlgoritmoKosaraju(Grafo *);
void ExecutarBusca(Grafo *);
void BuscaProfundidade (Grafo *, vertice, int);
void InverterGrafo (Grafo *);
void EscreverKorasaju (Grafo *, caminho *, int);

/*  Prototipos de funções operadoras do Algoritmo de Prim */
void AlgoritmoPrim(Grafo *);
void DeterminarPrimMinimo(Grafo *, vertice **);
void DeterminarCustoMinimo(Grafo *, vertice **);
void EscreverPrimMinimo(Grafo *, vertice **);

/*		Programa Principal:  	*/
int main () {
/*		Leitura e escrita do grafo  	*/
    int nvert;
    int i = 1;
    filein = fopen ("DadosGrafo.dat", "r");
    fscanf(filein, "%d", &nvert);
    while(nvert != 0){
        printf("\nGRAFO %d\n", i);
        nvert = LerGrafo (&G, nvert);
        printf ("\n\tGrafo em fase inicial\n");
        ExecutarBusca(&G);
        EscreverGrafo (&G, True);
/*      Execução do Algoritmo de Dijkstra       */
        printf ("\n\tCaminhos minimos pelo Algoritmo de Dijkstra\n\n");
        AlgoritmoDijkstra(&G);
/*      Execução do Algoritmo de Kosaraju   */
        printf ("\n\tComponentes fortemente conexos\n\n");
        AlgoritmoKosaraju(&G);
/*      Transformação de G em um grafo nao-orientado    */
        Desorientar(&G);
        TestarConectividade(&G);
        printf("\n\tGrafo %d nao-orientado:\n", i);
        EscreverGrafo(&G, False);
/*      Execução do Algoritmo de Prim   */
        if(G.conexo)
            AlgoritmoPrim(&G);
        LimparGrafo(&G);
        i++;
        printf("---------------------------------------------\n");
    }
    fclose(filein);
    printf ("\n\n"); system ("pause"); return 0;
}

/*		Funcoes   operadoras do Digrafo*/
int LerGrafo(Grafo *graf, int nvert){
    int vert1, vert2, dist;
    InicializarGrafo(graf, nvert);
    fscanf(filein,"%d", &vert1);
    while(LinhaValida()){
        fscanf(filein, "%d", &vert2);
        fscanf(filein, "%d", &dist);
        if(vert1 <= nvert && vert2 <= nvert && vert1 >=0 && vert2 >= 0 && dist > 0 && !CaminhoRepetido(graf, vert1, vert2))
            NovoAdjunto(graf, vert1, vert2, dist);
        fscanf(filein, "%d", &vert1);
    }
    CriarContraAdjuntos(graf);
    return vert1;
}
logic LinhaValida(){
    char ch;
    ch = fgetc(filein);
    while(ch == ' ') ch = fgetc(filein);
    if(ch == '\n'){
        fseek(filein, -1, SEEK_CUR);
        return False;
    }
    else{
        fseek(filein, -1, SEEK_CUR);
        return True;
    }
}
void InicializarGrafo (Grafo *graf, int size){
    graf->nvert = size;
    graf->EspacoVertices = (CelulaVertice*)malloc((size+1)*sizeof(CelulaVertice));
    for(int i=1; i<=size; i++){
        (graf->EspacoVertices+i)->listadj = (CelulaAdj*)malloc(sizeof(CelulaAdj));
        (graf->EspacoVertices+i)->listcontradj = (CelulaAdj*)malloc(sizeof(CelulaAdj));
        (graf->EspacoVertices+i)->listadj->prox = NULL;
        (graf->EspacoVertices+i)->listcontradj->prox = NULL;
        (graf->EspacoVertices+i)->visit = False;
        (graf->EspacoVertices+i)->nvisit = 0;
        (graf->EspacoVertices+i)->cfc = 0;
    }
    graf->conexo = False;
}
void LimparGrafo(Grafo *graf){
    free(graf->EspacoVertices);
    graf->nvert = 0;
    cont = 0;
}
logic CaminhoRepetido (Grafo *graf, int vert1, int vert2){
    logic r = False;
    CelulaAdj *p = (graf->EspacoVertices+vert1)->listadj;
    while(p != NULL){
        if(p->vert == vert2) r = True;
        p = p->prox;
    }
    return r;
}
void NovoAdjunto (Grafo *graf, int vert1, int vert2, int dist){
    CelulaAdj *p, *ins;
    for(p = (graf->EspacoVertices+vert1)->listadj; p->prox!=NULL; p = p->prox);
    ins = (CelulaAdj*)malloc(sizeof(CelulaAdj));
    ins->vert = vert2;
    ins->custo = dist;
    ins->prox = NULL;
    p->prox=ins;
    if(CaminhoRepetido(graf,vert2,vert1)) ins->first = False;
    else ins->first = True;
}
void EscreverGrafo(Grafo *graf, logic direcionado){
    printf("\n\n");
    for(int i=1; i<=graf->nvert; i++){
        printf("Vertice %d)\tVisitado: %d;\t N.o de Visita: %d;\n", i, (graf->EspacoVertices+i)->visit, (graf->EspacoVertices+i)->nvisit);
        printf("\tAdjacentes e custos: ");
        for(CelulaAdj *p = (graf->EspacoVertices+i)->listadj; p->prox!=NULL; p = p->prox)
            printf("(%d,%d)", p->prox->vert, p->prox->custo);
        printf("\n\n");
    }
    if (!direcionado)
        printf("Conectividade: %d\n", graf->conexo);
}
void CriarContraAdjuntos(Grafo *graf){
    CelulaAdj *p, *ins, *q;
    for(int i = 1; i<=graf->nvert; i++){
        for(p = (graf->EspacoVertices+i)->listadj; p->prox!=NULL; p=p->prox){
            ins = (CelulaAdj*)malloc(sizeof(CelulaAdj));
            q = (graf->EspacoVertices+p->prox->vert)->listcontradj;
            ins->vert = i;
            ins->custo = p->prox->custo;
            ins->first = p->prox->first;
            ins->prox = q->prox;
            q->prox=ins;
        }
    }
}
void Desorientar (Grafo *graf){
    CelulaVertice *p;
    CelulaAdj *q, *v;
    int i = 1;
    for(p = graf->EspacoVertices+1; i<=graf->nvert; p++){
        p->visit = False;
        p->nvisit = 0;
        q = p->listadj;
        if(q->prox == NULL){
            q->prox = p->listcontradj->prox;
            p->listcontradj->prox = NULL;
        }
        while(q->prox!=NULL){
            if(q->prox->prox == NULL){
                q->prox->prox = p->listcontradj->prox;
                p->listcontradj->prox = NULL;
            }
            if(q->prox->vert == i || !q->prox->first){
                v = q->prox;
                q->prox = q->prox->prox;
                free(v);
            }
            else q = q->prox;
        }
        i++;
    }
    cont = 0;
}
void TestarConectividade(Grafo *graf){
    int i;
    logic conexo = True;
    BuscaProfundidade(graf, 1, 0);
    for(i=1; i<=graf->nvert; i++)
        if((graf->EspacoVertices+i)->visit == False)
            conexo = False;
    graf->conexo=conexo;
}
int CustoArco(vertice vert1, vertice vert2, Grafo *graf){
    CelulaAdj *p;
    int custo = infinito;
    for(p = (graf->EspacoVertices+vert1)->listadj; p->prox!=NULL; p=p->prox){
        if(p->prox->vert == vert2)
            custo = p->prox->custo;
    }
    return custo;
}

/*      Funcoes operadoras do Algoritmo de Dijkstra*/
void AlgoritmoDijkstra(Grafo *graf){
    InicializarDijkstra(graf);
    CalcDijkstraDef(graf);
    EscreverDijkstra(graf);
    LimparDijkstra();
}
void InicializarDijkstra(Grafo *graf){
    D = malloc ((graf->nvert + 1) * sizeof(Dijkstra));
    for (int i = 2; i <= graf->nvert; i++) {
        D[i].custo = infinito;
        D[i].definitivo = False;
        InicCaminho (&D[i].path);
        InserirNoFinal (1, &D[i].path);
        InserirNoFinal (i, &D[i].path);
    }
    for (CelulaAdj *p = (graf->EspacoVertices+1)->listadj; p->prox != NULL; p = p->prox)
        D[p->prox->vert].custo = p->prox->custo;
}
void CalcDijkstraDef(Grafo *graf){
    int novocusto, w;
    for (int i = 2; i <= graf->nvert; i++) {
        w = Minimo (D, graf->nvert);
        D[w].definitivo = True;
        for (int v = 2; v <= graf->nvert; v++)
            if (D[v].definitivo == False) {
                novocusto = SomarCaminhos(D,graf, w, v);
                if (novocusto < D[v].custo) {
                    D[v].custo = novocusto;
                    CopiarCaminho
                            (&D[v].path, D[w].path);
                    InserirNoFinal (v, &D[v].path);
                }
            }
    }
}
void EscreverDijkstra(Grafo *graf){
    for(int i=2; i<=graf->nvert; i++){
        printf("Caminho(1,%d):", i);
        printf("\n\tCusto = ");
        ImprimirCusto(D, i);
        printf("\tVertices = ");
        if(D[i].custo == infinito)
            printf("Caminho inexistente\n");
        else
            EscreverCaminho(D[i].path);
    }
}
int Minimo(Dijkstra *dij, int size){
    int j;
    for(j=2; dij[j].definitivo!=False; j++);
    for(int i=j; i<=size; i++)
        if(dij[i].custo<=dij[j].custo && dij[i].definitivo == False)
            j=i;
    return j;
}
int SomarCaminhos(Dijkstra *dij, Grafo *graf, int w, int v){
    int i = dij[w].custo;
    int j = CustoArco (w, v, graf);
    if (j > infinito - i)
        return infinito;
    else
        return i+j;
}
void ImprimirCusto(Dijkstra *dij, int w){
    if(dij[w].custo == infinito)
        printf("Infinito\n");
    else
        printf("%d\n", D[w].custo);
}
void LimparDijkstra(){free(D);}

/*  Funções operadoras do Algoritmo de Kosaraju */
void AlgoritmoKosaraju(Grafo *graf){
    int cfc = 1, a=1;
    CelulaVertice *pcel;
    caminho *conexos;
    vertice *ordem;
    ordem = (vertice*)malloc((graf->nvert+1)*sizeof(vertice));
    for(int i = 1; i<=graf->nvert; i++)
        ordem[(graf->EspacoVertices+i)->nvisit]=i;
    InverterGrafo(graf);
    for(int j = graf->nvert; j>=1; j--)
        if(!((graf->EspacoVertices+ordem[j])->visit)){
            BuscaProfundidade(graf, ordem[j],cfc);
            cfc++;
        }
    conexos = (caminho*)malloc(cfc*sizeof(caminho));
    for(int p=1; p<cfc; p++)
        InicCaminho(&conexos[p]);
    for(pcel = graf->EspacoVertices+1; a<=graf->nvert; pcel++){
        InserirNoFinal(a, &conexos[pcel->cfc]);
        a++;
    }
    InverterGrafo(graf);
    EscreverKorasaju(graf, conexos, cfc);
}
void ExecutarBusca(Grafo *graf){
    for(int k = 1; k<=graf->nvert; k++)
        if(!((graf->EspacoVertices+k)->visit))
            BuscaProfundidade(graf, k,0);
}
void BuscaProfundidade (Grafo *graf, vertice vert, int cfc){
    CelulaAdj *p;
    (graf->EspacoVertices+vert)->visit = True;
    (graf->EspacoVertices+vert)->cfc+=cfc;
    p = (graf->EspacoVertices+vert)->listadj;
    while (p->prox != NULL)  {
        if ((graf->EspacoVertices+p->prox->vert)->visit == False)
            BuscaProfundidade(graf, p->prox->vert, cfc);
        p = p->prox;
    }
    cont++;
    (graf->EspacoVertices+vert)->nvisit = cont;
}
void InverterGrafo (Grafo *graf){
    CelulaVertice *p;
    CelulaAdj *q;
    int i = 1;
    for(p = (graf->EspacoVertices+1); i<=graf->nvert; p++){
        q = p->listadj;
        p->listadj = p->listcontradj;
        p->listcontradj = q;
        p->visit = False;
        i++;
    }
}
void EscreverKorasaju (Grafo *graf, caminho *conexos, int max){
    posicao p, q;
    logic printback;
    CelulaAdj *r;
    for(int i=1; i<max; i++){
        printf("CFC %d:\n", i);
        printf("\tVertices= {");
        p = conexos[i].inic;
        while(p!=conexos[i].fim){
            printf("%d", p->prox->elem);
            p = Proxima(p);
            if(p!=conexos[i].fim)
                printf(",");
        }
        printf("}\n");
        printf("\tCaminhos= {");
        p = conexos[i].inic;
        printback = False;
        while(p!=conexos[i].fim){
            q = conexos[i].inic;
            while(q!=conexos[i].fim){
                for(r = (graf->EspacoVertices+p->prox->elem)->listadj; r->prox != NULL; r = r->prox)
                    if(r->prox->vert == q->prox->elem){
                        printf("(%d->%d),", p->prox->elem,q->prox->elem);
                        printback = True;
                    }
                q = Proxima(q);
            }
            p = Proxima(p);
        }
        if(printback) printf("\b}\n");
        else printf("}\n");
    }
}

/*  Funções operadoras do Algoritmo de Prim */
void AlgoritmoPrim(Grafo *graf){
    vertice *P[3];
    for(int i=1; i<=graf->nvert; i++)
        (graf->EspacoVertices+i)->visit = False;
    P[0]= (vertice*)malloc((graf->nvert-1)*sizeof(vertice));
    P[1]= (vertice*)malloc((graf->nvert-1)*sizeof(vertice));
    P[2]= (vertice*)malloc((graf->nvert-1)*sizeof(vertice));
    (graf->EspacoVertices+1)->visit = True;
    DeterminarPrimMinimo(graf, P);
    EscreverPrimMinimo(graf, P);
    DeterminarCustoMinimo(graf, P);
}
void DeterminarPrimMinimo(Grafo *graf, vertice **P){
    CelulaAdj *ptr;
    int min;
    int k = 1, j = 1;
    for(int i = 0; i<(graf->nvert-1); i++){
        min = infinito;
        for(int p = 1;p<=(graf->nvert);p++)
            for(ptr = (graf->EspacoVertices+p)->listadj;ptr->prox!=NULL;ptr=ptr->prox)
                if(ptr->prox->custo < min && !(graf->EspacoVertices+ptr->prox->vert)->visit && (graf->EspacoVertices+p)->visit){
                    min = ptr->prox->custo;
                    k=p;
                    j=ptr->prox->vert;
                }
        P[0][i]=k;
        P[1][i]=j;
        P[2][i]=min;
        (graf->EspacoVertices+j)->visit = True;
    }
}
void DeterminarCustoMinimo(Grafo *graf, vertice **P){
    int custo = 0;
    for(int i = 0; i<(graf->nvert-1); i++)
        custo += P[2][i];
    printf("\tCusto minimo: %d\n", custo);
}
void EscreverPrimMinimo(Grafo *graf, vertice **P){
    printf("\n\tArcos da Arvore de custo minimo:\n");
    for(int i=0; i<(graf->nvert-1); i++)
        printf("\t\t(%d - %d)\n", P[0][i], P[1][i]);
}

/*  Funções operadoras de caminho    */
void InicCaminho (caminho *path){
    path->inic = (noh*)malloc(sizeof(noh));
    path->fim = path->inic;
}
posicao Primeira (caminho path){return path.inic;}
posicao Proxima (posicao p){return p->prox;}
posicao Fim (caminho path){return path.fim;}
void InserirNoFinal (int elem, caminho *path){
    posicao insert;
    insert = (posicao)malloc(sizeof(noh));
    insert->elem = elem;
    path->fim->prox = insert;
    path->fim = insert;
}
int Elemento (posicao p, caminho path){return p->elem;}
void EscreverCaminho (caminho path){
    posicao p = path.inic;
    while(p!=path.fim){
        printf("%-3d", p->prox->elem);
        p = Proxima(p);
    }
    printf("\n");
}
void CopiarCaminho (caminho *path, caminho pathoriginal){
    EsvaziarCaminho(path);
    InicCaminho(path);
    for(posicao p = Primeira(pathoriginal); p!=Fim(pathoriginal); p = Proxima(p))
        InserirNoFinal(Elemento(Proxima(p),pathoriginal),path);
}
void EsvaziarCaminho (caminho *path){
    posicao p, q;
    p = path->inic;
    q = p->prox;
    do{
        free(p);
        p = q;
        q = p->prox;
    }while(p!=path->fim);
    path->inic = NULL;
    path->fim = NULL;
}
