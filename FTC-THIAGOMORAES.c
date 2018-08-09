#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int indiceEstado(char* estados[], char* estado, int num_estados){ // Busca o indice do estado no vetor de estados
	
	int i;
	
	for(i = 0; i < num_estados; i++){
		if(strcmp(estados[i], estado) == 0){	
			return i;
		}
	}
	
	return -1;
}

int indiceSimbolo(char alfabeto[], int num_alfabeto, char simbolo){ // Busca o indice do simbolo dentro do alfabeto
	
	int i ;
	
	for(i = 0; i < num_alfabeto; i++){
		if(alfabeto[i] == simbolo){
			return i;
		}
	}
	
	return -1;
}


/* 	Registra os estados do AFD dentro de um vetor de estados. 
	Retorna uma referencia para o vetor de estados. 
 	O tamanho do vetor é colocado na variavel de entrada passado por referencia. */
char** lerEstados(int* num_estados){
	
	int i, len = 0;
	char* buffer = (char*)malloc(100);
	
	char* tmp_estado;
	char** vetor_estados;
	char** tmp_vetor_estados = (char**)malloc(sizeof(char*)*300);
	
	scanf("%s", buffer);
	
	while(strcmp(buffer, ";")){
		
		tmp_estado = (char*)malloc(strlen(buffer) + 1);
		strcpy(tmp_estado, buffer);
		tmp_vetor_estados[len] = tmp_estado;
		len += 1;
		
		scanf("%s", buffer);
	}
	
	vetor_estados = (char**)malloc(sizeof(char*)*len);
	
	for(i = 0; i < len; i++){
		vetor_estados[i] = tmp_vetor_estados[i];
	}

	*num_estados = len;
	
	free(buffer);
	free(tmp_vetor_estados);
	
	return vetor_estados;	
}

/* 	Registra o alfabeto do AFD dentro de um vetor de simbolos.
	Retorna uma referencia para o alfabeto. 
 	O tamanho do vetor é colocado na variavel de entrada passado por referencia. */
char* lerAlfabeto(int* num_alfabeto){
	
	int i = 0;
	char* buffer = (char*)malloc(3);
	char* alfabeto;
	char* tmp_alfabeto = (char*)malloc(256);
	
	scanf("%s", buffer);
	
	while(strcmp(buffer, ";")){
		tmp_alfabeto[i]	= buffer[0];
		i += 1;
		
		scanf("%s", buffer);
	}
		
	alfabeto = (char*)malloc(i);
	strncpy(alfabeto, tmp_alfabeto, i);
	
	free(buffer);
	free(tmp_alfabeto);
	
	*num_alfabeto = i;
	
	return alfabeto;
}

int main(){
	
	// Elementos de um AFD
	int** afd;
	char** estados ;
	char* alfabeto ;
	char* estados_finais;
	int estado_inicial;
	
	
	char* buffer ;
	int num_estados = 0;
	int num_alfabeto = 0;
	int i, j, len;
	int estado_atual, estado_destino, simbolo;
	
	estados = lerEstados(&num_estados); // Lendo estados do AFD
	
	alfabeto = lerAlfabeto(&num_alfabeto); // Lendo o alfabeto do AFD
	
	// Lendo o estado inicial do AFD
	buffer = (char*)malloc(sizeof(char)*100);
	scanf("%s", buffer);
	
	if((estado_inicial = indiceEstado(estados, buffer, num_estados)) == -1){
		printf("Erro 03 ;");
		exit(-1);
	}
	scanf("%s", buffer); //descartando o ';'

	estados_finais = (char*)malloc(num_estados); // Vetor indicando quais estados sao finais
	
	memset(estados_finais, 0, num_estados); // seta o valor zero para todas as posicoes do vetor
	
	// Lendo os estados finais do AFD. A ideia e que se um estado esta marcado com 1 ele e um estado final.
	scanf("%s", buffer);
	while(strcmp(buffer, ";")){
		
		if((i = indiceEstado(estados, buffer, num_estados)) == -1){
			printf("Erro 03 ;");
			exit(-1);
		}
		
		estados_finais[i] = 1;
		
		scanf("%s", buffer);
	}
	
	// Matriz de Incidencia com os estados sendo as linhas e os simbolos sendo as colunas
	afd = (int**)malloc(sizeof(int*)*num_estados);
	for(i = 0; i < num_estados; i++){
		afd[i] = (int*)malloc(sizeof(int)*num_alfabeto);
		memset(afd[i], -1, num_alfabeto*sizeof(int*));
	}
	
	
	// Lendo as transicoes do AFD
	scanf("%s", buffer);
	while(strcmp(buffer, "#")){
		while(strcmp(buffer, ";")){
						
			if((estado_atual = indiceEstado(estados, buffer, num_estados)) == -1){ // Verifica se o estado pertence ao AFD
				printf("Erro 03 ;");
				exit(-1);
			}
			
			
			scanf("%s", buffer);
			if((simbolo = indiceSimbolo(alfabeto, num_alfabeto, buffer[0])) == -1){ // Verifica se o simbolo pertence ao alfabeto do AFD
				printf("Erro 01 ;");
				exit(-1);
			}
			
			
			scanf("%s", buffer);
			if((estado_destino = indiceEstado(estados, buffer, num_estados)) == -1){ // Verifica se o estado pertence ao AFD
				printf("Erro 03 ;");
				exit(-1);
			}
			
			
			if(afd[estado_atual][simbolo] != -1){ 
				printf("Erro 04 ;");
				exit(-1);
			}
			
			afd[estado_atual][simbolo] = estado_destino; // Seta a transicao do estado atual para o destino consumindo o 'simbolo'
			
			scanf("%s", buffer);
		}
		scanf("%s", buffer);
	}
	scanf("%s", buffer); //descartando o ';'
	
	
	// Verificando se o AFD de entrada e um AFD valido. 
	// A ideia e que o estado na linha i consumindo o simbolo j vai para o estado[i][j] e se nao existir este estado (-1) entao falta uma transicao no AFD.
	for(i = 0; i < num_estados;i++){
		for(j = 0; j < num_alfabeto; j++){
			if(afd[i][j] == -1){
				printf("Erro 04 ;");
				exit(-1);
			}
		}
	}
	
	// Verificando se as palavras pertecem ao AFD
	scanf("%s", buffer);	
	while(strcmp(buffer, ";")){
		len = strlen(buffer);
		
		estado_atual = estado_inicial;
		
		for(i = 0; i < len; i++){
			
			// Verifica se o simbolo da palavra pertence ao alfabeto do AFD
			if((simbolo = indiceSimbolo(alfabeto, num_alfabeto, buffer[i])) == -1){
				printf("Erro 02 ;\n");
				estado_atual = -1;
				break;
			}
			
			estado_atual = afd[estado_atual][simbolo];
		}
		
		// Se o estado atual existe entao verifica se ele e um estado final.
		if(estado_atual != -1){
			if(estados_finais[estado_atual]){
				printf("%s sim \n", buffer);
			}else{
				printf("%s nao \n", buffer);
			}
		}
		
		scanf("%s", buffer);
	}
}
