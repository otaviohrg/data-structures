/********************************************/
/* Aluno: Otávio Guimarães                  */
/* CES-11: Algoritmos e Estruturas de Dados */
/* Prof. Fábio Carneiro Mokarzel            */
/********************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef char string[16], estado[3];

typedef enum faixa faixa;
enum faixa {bebe, crianca, adolescente, jovem, adulto, idoso};
string nomefaixa[6] = {"Bebes", "Criancas", "Adolescentes", "Jovens", "Adultos", "Idosos"};

typedef enum escolaridade escolaridade;
enum escolaridade {nenhuma, fundamental, medio, graduacao, mestrado, doutorado};
string nomeescolaridade[6] = {"Nenhuma", "Fundamental", "Medio", "Graduacao", "Mestrado", "Doutorado"};

estado nomeestado[27] = {"AC", "AL", "AP", "AM", "BA", "CE", "DF", "ES", "GO", "MA", "MT", "MS", "MG", "PA", "PB", "PR", "PE", "PI", "RJ", "RN", "RS", "RO", "RR", "SC", "SP", "SE", "TO"};

typedef struct habitante habitante;
struct habitante {
	string nome;
	int idade;
	escolaridade esc;
	estado est;
	habitante *inic, *fet, *ptesc, *ptest;
};
typedef habitante *ponthab;

ponthab Letra[26] = {NULL}, FaixaEtaria[6] = {NULL}, Escolar[6] = {NULL}, Estado[27] = {NULL};

void CriarHabitante();
faixa FindFaixaEtaria(int idade);
void ExcluirHabitante();
void DeleteNode(ponthab node);
void ListarLetra();
void ListarFaixaEtaria();
void ListarEscolar();
void ListarEstado();
void ListarFaixaEtariaEscolar();
void ListarFaixaEtariaEstado();
void ListarEscolarEstado();
void ListarFaixaEtariaEstadoEscolar();
void LimparFaixaEtaria();
void LimparEscolaridade();
void ListarOrdemAlfabetica();
void CloseAplication();

int main(){
	int exit = 0;
	int command;
	void (*fx[14])();
	fx[0] = CriarHabitante;
	fx[1] = ExcluirHabitante;
	fx[2] = ListarLetra;
	fx[3] = ListarFaixaEtaria;
	fx[4] = ListarEscolar;
	fx[5] = ListarEstado;
	fx[6] = ListarFaixaEtariaEscolar;
	fx[7] = ListarFaixaEtariaEstado;
	fx[8] = ListarEscolarEstado;
	fx[9] = ListarFaixaEtariaEstadoEscolar;
	fx[10] = LimparFaixaEtaria;
	fx[11] = LimparEscolaridade;
	fx[12] = ListarOrdemAlfabetica;
	while(exit == 0){
		command = 0;
		system("clear");
		printf("Menu de tarefas:\n");
		printf("1. Inserir no sistema as informações de um habitante\n");
		printf("2. Deletar do sistema as informações do habitante de um determinado nome\n");
		printf("3. Listar as informações de todos os habitantes cujos nomes se iniciem por uma determinada letra\n");
		printf("4. Listar as informações de todos os habitantes de uma determinada faixa etária\n");
		printf("5. Listar as informações de todos os habitantes de uma determinada escolaridade\n");
		printf("6. Listar as informações de todos os habitantes nascidos em um determinado estado\n");
		printf("7. Listar as informações de todos os habitantes de uma determinada faixa etária com uma determinada escolaridade\n");
		printf("8. Listar as informações de todos os habitantes de uma determinada faixa etária nascidos num determinado estado\n");
		printf("9. Listar as informações de todos os habitantes com uma determinada escolaridade nascidos num determinado estado\n");
		printf("10.Listar as informações de todos os habitantes de uma determinada faixa etária com uma determinada escolaridade, nascidos num determinado estado\n");
		printf("11.Deletar todos os habitantes de uma determinada faixa etária\n");
		printf("12.Deletar todos os habitantes com uma determinada escolaridade\n");
		printf("13.Listar todos os habitantes por ordem alfabética (sem ordenar os nomes com mesma inicial)\n");
		printf("14.Encerrar aplicação\n");
		printf("\nEscolha uma tarefa: ");
		while(command<1 || command>14){
			scanf("%d", &command);
			if(command<1 || command>14)
				printf("Tarefa invalida. Escolha uma tarefa: ");
		}
		if(command == 14){
			CloseAplication();
			system("clear");
			printf("Aplicação Encerrada\n");
			exit = 1;
		}
		else{
			system("clear");
			fx[command-1]();
		}
	}
}

void CriarHabitante(){
	string nome;
	estado est;
	int idade;
	escolaridade esc;
	ponthab temphabitante;
	int estindex;
	printf("Nome do Habitante: ");
	scanf("%s", nome);
	if(*nome<65 || (*nome>90 && *nome<97) || *nome>122){
		printf("Nome inválido. Pressione qualquer tecla.\n");
		getchar();
		getchar();
		return;
	}
	else{
		temphabitante = (habitante *)malloc(sizeof(habitante));
		printf("Idade: ");
		scanf("%d", &idade);
		printf("Grau de escolaridade:\n0. Nenhuma\n1. Fundamental\n2. Medio\n3. Graduacao\n4. Mestrado\n5. Doutorado\n");
		scanf("%u", &esc);
		printf("Estado: ");
		scanf("%s", est);
		strcpy(temphabitante->nome, nome);
		temphabitante->idade = idade;
		temphabitante->esc = esc;
		strcpy(temphabitante->est, est);
		if(*nome>=65 && *nome <= 90){
			temphabitante->inic = Letra[*nome-65];
			Letra[*nome-65] = temphabitante;
		}
		else{
			temphabitante->inic = Letra[*nome-97];
			Letra[*nome-97] = temphabitante;
		}
		temphabitante->fet = FaixaEtaria[FindFaixaEtaria(idade)];
		FaixaEtaria[FindFaixaEtaria(idade)] = temphabitante;
		temphabitante->ptesc = Escolar[esc];
		Escolar[esc] = temphabitante;
		for(estindex =0; strcmp(est, nomeestado[estindex])!=0; estindex++);
		temphabitante->ptest = Estado[estindex];
		Estado[estindex] = temphabitante;
		printf("Habitante criado. Pressione qualquer tecla.\n");
		getchar();
		getchar();
	}
}

faixa FindFaixaEtaria(int idade){
	faixa grupo;
	if(idade<=2)
		return bebe;
	else if(idade>2 && idade<=11)
		return crianca;
	else if(idade>11 && idade<=17)
		return adolescente;
	else if(idade>17 && idade<=30)
		return jovem;
	else if(idade>30 && idade<=60)
		return adulto;
	else
		return idoso;
}

void ExcluirHabitante(){
	string nomealvo;
	int index, iestado, found = 0;
	ponthab search, lastsearch = NULL, iterator;
	printf("Nome do habitante a ser removido: ");
	scanf("%s", nomealvo);
	if(*nomealvo<91)
		index = *nomealvo-65;
	else
		index = *nomealvo-97;
	search = Letra[index];
	while(found == 0 && search != NULL){
		if(strcmp(nomealvo, search->nome) == 0)
			found = 1;
		else{
			lastsearch = search;
			search = search -> inic;
		}
	}
	if(found == 0){
		printf("Habitante não cadastrado. Pressione qualquer tecla\n");
		getchar();
		getchar();
	}
	else{
		DeleteNode(search);
		printf("Habitante removido. Pressione qualquer tecla\n");
		getchar();
		getchar();	
	}
}

void ListarLetra(){
	char first;
	int index;
	ponthab search;
	printf("Digite a letra inicial: ");
	getchar();
	system("clear");
	scanf("%c", &first);
	if(first<91)
		index = first-65;
	else
		index = first-97;
	for(search = Letra[index]; search != NULL; search = search->inic){
		printf("Nome: %s\n", search->nome);
		printf("Faixa Etaria: %s\n", nomefaixa[FindFaixaEtaria(search->idade)]);
		printf("Escolaridade: %s\n", nomeescolaridade[search->esc]);
		printf("Estado: %s\n", search->est);
		printf("----------------------------------\n");
	}
	printf("Pressione qualquer tecla");
	getchar();
	getchar();
}

void ListarFaixaEtaria(){
	faixa index;
	ponthab search;
	printf("0. Bebes\n1. Criancas\n2. Adolescentes\n3. Jovens\n4. Adultos\n5. Idosos\n\n");
	printf("Selecione uma faixa etaria: ");
	getchar();
	scanf("%u", &index);
	system("clear");
	for(search = FaixaEtaria[index]; search != NULL; search = search->fet){
		printf("Nome: %s\n", search->nome);
		printf("Faixa Etaria: %s\n", nomefaixa[FindFaixaEtaria(search->idade)]);
		printf("Escolaridade: %s\n", nomeescolaridade[search->esc]);
		printf("Estado: %s\n", search->est);
		printf("----------------------------------\n");
	}
	printf("Pressione qualquer tecla");
	getchar();
	getchar();
}

void ListarEscolar(){
	escolaridade index;
	ponthab search;
	printf("Selecione um grau de escolaridade: \n");
	printf("0. Nenhuma\n1. Fundamental\n2. Medio\n3. Graduacao\n4. Mestrado\n5. Doutorado\n");		
	getchar();
	scanf("%u", &index);
	system("clear");
	for(search = Escolar[index]; search != NULL; search = search->ptesc){
		printf("Nome: %s\n", search->nome);
		printf("Faixa Etaria: %s\n", nomefaixa[FindFaixaEtaria(search->idade)]);
		printf("Escolaridade: %s\n", nomeescolaridade[search->esc]);
		printf("Estado: %s\n", search->est);
		printf("----------------------------------\n");
	}
	printf("Pressione qualquer tecla");
	getchar();
	getchar();
}

void ListarEstado(){
	estado est;
	int index;
	ponthab search;
	printf("Selecione um estado: ");
	getchar();
	scanf("%s", est);
	system("clear");
	for(index=0; strcmp(est, nomeestado[index]) != 0; index++);
	for(search = Estado[index]; search != NULL; search = search->ptest){
		printf("Nome: %s\n", search->nome);
		printf("Faixa Etaria: %s\n", nomefaixa[FindFaixaEtaria(search->idade)]);
		printf("Escolaridade: %s\n", nomeescolaridade[search->esc]);
		printf("Estado: %s\n", search->est);
		printf("----------------------------------\n");
	}
	printf("Pressione qualquer tecla");
	getchar();
	getchar();
}

void ListarFaixaEtariaEscolar(){
	faixa indexidade;
	escolaridade indexescolar;
	ponthab search;
	printf("0. Bebes\n1. Criancas\n2. Adolescentes\n3. Jovens\n4. Adultos\n5. Idosos\n\n");
	printf("Selecione uma faixa etaria: ");
	getchar();
	scanf("%u", &indexidade);
	system("clear");
	printf("0. Nenhuma\n1. Fundamental\n2. Medio\n3. Graduacao\n4. Mestrado\n5. Doutorado\n\n");		
	printf("Selecione um grau de escolaridade: ");
	getchar();
	scanf("%u", &indexescolar);
	system("clear");
	for(search = FaixaEtaria[indexidade]; search != NULL; search = search->fet){
		if(search->esc == indexescolar){
			printf("Nome: %s\n", search->nome);
			printf("Faixa Etaria: %s\n", nomefaixa[FindFaixaEtaria(search->idade)]);
			printf("Escolaridade: %s\n", nomeescolaridade[search->esc]);
			printf("Estado: %s\n", search->est);
			printf("----------------------------------\n");
		}
	}
	printf("Pressione qualquer tecla");
	getchar();
	getchar();
}

void ListarFaixaEtariaEstado(){
	faixa indexidade;
	estado est;
	int indexestado;
	ponthab search;
	printf("0. Bebes\n1. Criancas\n2. Adolescentes\n3. Jovens\n4. Adultos\n5. Idosos\n\n");
	printf("Selecione uma faixa etaria: ");
	getchar();
	scanf("%u", &indexidade);
	system("clear");
	printf("Selecione um estado: ");
	getchar();
	scanf("%s", est);
	system("clear");
	for(indexestado=0; strcmp(est, nomeestado[indexestado]) != 0; indexestado++);
	for(search = FaixaEtaria[indexidade]; search != NULL; search = search->fet){
		if(strcmp(search->est, nomeestado[indexestado]) == 0){
			printf("Nome: %s\n", search->nome);
			printf("Faixa Etaria: %s\n", nomefaixa[FindFaixaEtaria(search->idade)]);
			printf("Escolaridade: %s\n", nomeescolaridade[search->esc]);
			printf("Estado: %s\n", search->est);
			printf("----------------------------------\n");
		}
	}
	printf("Pressione qualquer tecla");
	getchar();
	getchar();
}

void ListarEscolarEstado(){
	escolaridade indexescolar;
	estado est;
	int indexestado;
	ponthab search;
	printf("0. Nenhuma\n1. Fundamental\n2. Medio\n3. Graduacao\n4. Mestrado\n5. Doutorado\n\n");		
	printf("Selecione um grau de escolaridade: ");
	getchar();
	scanf("%u", &indexescolar);
	system("clear");
	printf("Selecione um estado: ");
	getchar();
	scanf("%s", est);
	system("clear");
	for(indexestado=0; strcmp(est, nomeestado[indexestado]) != 0; indexestado++);
	for(search = Escolar[indexescolar]; search != NULL; search = search->ptesc){
		if(strcmp(search->est, nomeestado[indexestado]) == 0){
			printf("Nome: %s\n", search->nome);
			printf("Faixa Etaria: %s\n", nomefaixa[FindFaixaEtaria(search->idade)]);
			printf("Escolaridade: %s\n", nomeescolaridade[search->esc]);
			printf("Estado: %s\n", search->est);
			printf("----------------------------------\n");
		}
	}
	printf("Pressione qualquer tecla");
	getchar();
	getchar();
}

void ListarFaixaEtariaEstadoEscolar(){
	faixa indexidade;
	escolaridade indexescolar;
	estado est;
	int indexestado;
	ponthab search;
	printf("0. Bebes\n1. Criancas\n2. Adolescentes\n3. Jovens\n4. Adultos\n5. Idosos\n\n");
	printf("Selecione uma faixa etaria: ");
	getchar();
	scanf("%u", &indexidade);
	system("clear");
	printf("0. Nenhuma\n1. Fundamental\n2. Medio\n3. Graduacao\n4. Mestrado\n5. Doutorado\n\n");		
	printf("Selecione um grau de escolaridade: ");
	getchar();
	scanf("%u", &indexescolar);
	system("clear");
	printf("Selecione um estado: ");
	getchar();
	scanf("%s", est);
	system("clear");
	for(indexestado=0; strcmp(est, nomeestado[indexestado]) != 0; indexestado++);
	for(search = FaixaEtaria[indexidade]; search != NULL; search = search->fet){
		if(search->esc == indexescolar && strcmp(search->est, nomeestado[indexestado]) == 0){
			printf("Nome: %s\n", search->nome);
			printf("Faixa Etaria: %s\n", nomefaixa[FindFaixaEtaria(search->idade)]);
			printf("Escolaridade: %s\n", nomeescolaridade[search->esc]);
			printf("Estado: %s\n", search->est);
			printf("----------------------------------\n");
		}
	}
	printf("Pressione qualquer tecla");
	getchar();
	getchar();
}

void LimparFaixaEtaria(){
	ponthab target;
	faixa indexidade;
	printf("0. Bebes\n1. Criancas\n2. Adolescentes\n3. Jovens\n4. Adultos\n5. Idosos\n\n");
	printf("Selecione uma faixa etaria: ");
	getchar();
	scanf("%u", &indexidade);
	target = FaixaEtaria[indexidade];
	while(target != NULL){
		DeleteNode(target);
		target = FaixaEtaria[indexidade];
	}
	printf("Pressione qualquer tecla");
	getchar();
	getchar();
}

void LimparEscolaridade(){
	ponthab target;
	escolaridade indexescolar;
	printf("0. Nenhuma\n1. Fundamental\n2. Medio\n3. Graduacao\n4. Mestrado\n5. Doutorado\n\n");		
	printf("Selecione um grau de escolaridade: ");
	getchar();
	scanf("%u", &indexescolar);
	target = Escolar[indexescolar];
	while(target != NULL){
		DeleteNode(target);
		target = Escolar[indexescolar];
	}
	printf("Pressione qualquer tecla");
	getchar();
	getchar();
}

void ListarOrdemAlfabetica(){
	ponthab search;
	for(int index = 0; index<26; index++){
		for(search = Letra[index]; search != NULL; search = search->inic){
			printf("Nome: %s\n", search->nome);
			printf("Faixa Etaria: %s\n", nomefaixa[FindFaixaEtaria(search->idade)]);
			printf("Escolaridade: %s\n", nomeescolaridade[search->esc]);
			printf("Estado: %s\n", search->est);
			printf("----------------------------------\n");
		}
	}
	printf("Pressione qualquer tecla");
	getchar();
	getchar();
}

void DeleteNode(ponthab node){
	ponthab itinic, itid, itesc, itest;
	int indexinix, indexescolar, indexidade, indexestado;
	int removeinix=0, removeesc=0, removeidade=0, removeestado=0;
	indexescolar = node->esc;
	indexidade = FindFaixaEtaria(node->idade);
	for(indexestado=0; strcmp(node->est, nomeestado[indexestado]) != 0; indexestado++);
	if(*(node->nome)<91)
		indexinix = *(node->nome)-65;
	else
		indexinix = *(node->nome)-97;
	itinic = Letra[indexinix];
	itid = FaixaEtaria[indexidade];
	itesc = Escolar[indexescolar];
	itest = Estado[indexestado];
	if(itinic == node)
		Letra[indexinix] = node->inic;
	else
		while(itinic!=NULL && removeinix == 0){
			if(itinic->inic == node){
				itinic->inic = node -> inic;
				removeinix = 1;
			}
			else
				itinic = itinic -> inic;
		}
	if(itid == node)
		FaixaEtaria[indexidade] = node->fet;
	else
		while(itid!=NULL && removeidade == 0){
			if(itid->fet == node){
				itid->fet = node -> fet;
				removeidade = 1;
			}
			else
				itid = itid -> fet;
		}
	if(itesc == node)
		Escolar[indexescolar] = node->ptesc;
	else
		while(itesc!=NULL && removeesc == 0){
			if(itesc->ptesc == node){
				itesc->ptesc = node -> ptesc;
				removeesc = 1;
			}
			else
				itesc = itesc -> ptesc;
		}
	if(itest == node)
		Estado[indexestado] = node->ptest;
	else
		while(itest!=NULL && removeestado == 0){
			if(itest->ptest == node){
				itest->ptest = node -> ptest;
				removeestado = 1;
			}
			else
				itest = itest -> ptest;
		}
	free(node);
}

void CloseAplication(){
	ponthab target;
	int index;
	for(index = 0; index<26; index++){
		target = Letra[index];
		while(target != NULL){
			DeleteNode(target);
			target = Letra[index];
		}
	}
}