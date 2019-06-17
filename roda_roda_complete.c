#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <time.h>
#include <conio.h>
#include <unistd.h>
#include <string.h>

//ESTRUTURA DOS JOGADORES
struct jogador{
	char nome[64];
	float valor;
};typedef struct jogador Jogador;
//ESTRUTURA DAS PALAVRAS E PISTAS
struct palavras {
	char pista[17];
	char vetpalavras[3][17];
	int qtd;
};typedef struct palavras Palavra;
//PROCEDIMENTOS E GAME MAIN
void gravar_premios();
void gravar_dicas();
void roda_game (Jogador[], int);
void jogador_screen (Jogador[]);
//FUNÇÕES
int verifica_resp(char*, char, int, int);
int game_final(char*, char*);	
int verifica_word(char*, char*, int, int);
//VARIÁVEIS GLOBAIS	
float premios[12];
Palavra dicas[3];
FILE *arq;
FILE *arq2;
char arquivo2[]="palavras.dat";
char arquivo[]="premios.dat";

main()
{
	int i;
	Jogador jog[3];
	char op, op2;


	setlocale(LC_ALL, "Portuguese");
	
	
do{
	reset:
	//MENU
	system("cls");
	//Apenas para deixar o jogo mais criativo e original!
	printf("\n   <--------------------------------------------------------------------->         "
		   "\n   |  __    __    __       _          _       __    __    __       _     |         "   
		   "\n   |  | \\  /  \\  |   \\    / \\        / \\      | \\  /  \\  |   \\    / \\    |"
		   "\n   |  |_/ /    \\ |    \\  /___\\  __  /___\\  __ |_/ /    \\ |    \\  /___\\   |  " 
		   "\n   |  | \\ \\    / |    / /     \\    /     \\    | \\ \\    / |    / /     \\  |  "
		   "\n   |  |  \\ \\__/  |__ / /       \\  /       \\   |  \\ \\__/  |__ / /       \\ |  "
		   "\n   <--------------------------------------------------------------------->         "
		   "\n       By:Joao Victor Teixeira de Oliveira && Juliana Macedo Santiago\n");
	printf("\n\n=========Menu=========");
	printf("\n\n1 - Jogar\n2 - Instruções\n3 - Créditos\n\n");
	scanf("%c", &op);
	system("cls");
	switch(op){
		case '1':
				//PEDINDO OS NOMES E COLOCANDO NAS VARÍAVEIS JOG
					for(i=0; i<3; i++){
						printf("\n\nDigite o nome do jogador %d: ", i+1);
						fflush(stdin);
						gets(jog[i].nome);
}			
			gravar_dicas();
			gravar_premios();
			roda_game(jog , i);
		break;
		case '2':	
//INFORMAÇÕES IMPORTANTES:
	printf("\n\t\t\t\t<------------>"
		   "\n\t\t\t\t| Instruções |"
		   "\n\t\t\t\t<------------>"
		   "\n\n    Este jogo é uma simulação em linguagem de programação C do game-show\n    'Roda-a-Roda Jequiti', apresentado por Sílvio Santos."
		   "\n\n    É necessário três jogadores para para jogar este Roda-A-Roda.O objetivo\n    do jogador será acertar as palavras da pista que for sorteada."
		   "\n\n    Ao iniciar o jogo, deverá ser informado o nome dos três jogadores.E\n    quando o jogo iniciar efetivamente, voce deverá espear por a sua vez\n    de jogar. E quando for sua vez, voce deverá digitar uma letra que\n    voce acha que exista na palavra da pista sorteada"
		   "\n\n    Caso a sua letra faça parte das palavras, você pontuará a quantia\n    sorteada na roda. Mas cuidado: a roda não possui apenas prêmios! As\n    opções 'Passe a Vez' e 'Perde Tudo' também circulam na roda!"
		   "\n\n    Os prêmios disponiveis neste jogo vão de R$100,00 a R$1000,00. Onde\n    os prêmios variam de R$100,00 em R$100,00. "
		   "\n\n    Ao final do jogo (todas as palavras foram acertadas), o jogador com a\n    maior pontuação vence! Porém, isto é apenas um jogo para a diversão de\n    todos, portanto não há dinheiro real envolvido nesta simulação."
		   "\n\n    Volte ao menu principal e divirta-se com os seus amigos!"
		   "\n\n    ->Pressione qualquer tecla para voltar.\n");
		   fflush(stdin);
		   scanf("%c", &op2);
		break;
		case '3':
//CRÉDITOS AOS DESENVOLVEDORES DESTE RODA-A-RODA:	   
	printf("\n\t\t\t\t<---------->"
		   "\n\t\t\t\t| Créditos |"
		   "\n\t\t\t\t<---------->"
		   "\n\n\t     IFSP - Análise e Desenvolvimento de Sistemas - 1° Semestre."
		   "\n\t\t   Projeto Didático - Linguagem de Programação - C"
		   "\n\t\t   Matéria: Lógica de Programação I.	 "
		   "\n\t\t   Professora: Josceli Maria Tenorio."
		   "\n\n\t    Programa desenvolvido por:"
		   "\n\n\t\tJoão Victor Teixeira de Oliveira - SP3030644\n\t\tJuliana Macedo Santiago - SP3032337"
	   	   "\n\n\t--> Obrigado por experimentar este modelo de 'RODA-A-RODA!' <--"
		   "\n\n     -> Pressione qualquer tecla para voltar.\n");
		   	fflush(stdin);
			scanf("%c", &op2);
		break;
}
}while(op != '1');
	system ("pause");
}

//JOGO BASE
void roda_game (Jogador jog[], int j)
{
	char palavra[9], *pista, resposta, linhas[9], respf[9];
	int i=0, cont=0, rprem, rpista, rpalavra;
//SEED
	srand(time(NULL));
//SORTEANDO O NÚMERO PARA A DICA E PALAVRA
	rpista = rand() % 3;
	rpalavra = rand() % 3;
//ARMAZENANDO A DICA E A PALAVRA
	system("cls");
	arq2 = fopen(arquivo2, "r");
	fread(&dicas, sizeof(dicas), 1, arq);
	pista = dicas[rpista].pista;
	strcpy(palavra, dicas[rpista].vetpalavras[rpalavra]);
//FECHANDO ARQUIVO DAS PALAVRAS
	fclose(arq2);
//ZERANDO CONTADOR
	j=0;
//CONTANDO AS LETRAS
	while(palavra[j] != '\0'){
		j++;
		cont++;
}
//IMSERINDO AS AS LINHAS
	for(j=0; j<cont; j++){
		linhas[j]='_';
	}
//ZERANDO OS ACUMULADORES DE VALOR DOS JOGADORES
	jog[0].valor=0; jog[1].valor=0; jog[2].valor=0;
//REPETIÇÃO
do{	
	//INICIANDO A RODADA
		system("cls");
		//PRINTANDO A DICA
		printf("\n\t\t\t\t\tA palavra está associada com: %s\n\n\n", pista);
		printf("\t\t\t\t\t\t     ");
		//PRINTANDO A PALAVRA
			for(j=0; j<cont; j++){
				printf("%c ", linhas[j]);
	}
		printf("\n\n\n");
		//VEZ DOS JOGADORES
		//PRINTANDO OS JOGADORES
			jogador_screen (jog);
			//ABRINDO ARQUIVO PARA ARMAZENAR OS DADOS NO VETOR PREMIOS
				arq = fopen(arquivo, "r");
				fread(&premios, sizeof(premios), 1, arq);
						//NÚMERO SORTEADO
						rprem = rand() % 12;
						if(rprem == 4){
								printf("\nPASSA A VEZ!\n");
								i++;
								goto fim;	
							}
						else if (rprem == 8){
								printf("\nPERDEU TUDO!\n");
								jog[i].valor = 0;
								i++;
								goto fim;
								}
						//VERIFICANDO SE FALTAM 3 LETRAS OU MENOS PARA SEREM CHUTADAS
						else if	(verifica_word(palavra, linhas, j, cont) != 0){
								sleep(1);
								printf("\nRoda a Roda!\n");
								printf("\nJogador %s: Faltam %d letras para completar a palavra", jog[i].nome, verifica_word(palavra, linhas, j, cont));
								sleep(1);
								printf("\nVocê tem 5 segundos para para pensar e depois digitar a palavra");
								for(j=0; j<=5; j++){
									printf("...%d", j);
									sleep(1);
								}
								printf("\nValendo R$ %0.2f, a palavra é: ", premios[rprem]+jog[i].valor);
								fflush(stdin);
								gets(respf);
									if (game_final(respf, palavra) == 0){
										strcpy(linhas, respf);
										system("cls");
										sleep(1);
										printf("\n\t\tA PALAVRA É: %s\n\n\n", linhas);
										jogador_screen (jog);
										if(i==0){
											printf("\nVENCEDOR\n\n");
											sleep(2);
											goto end;
										}
										else if(i==1){
											printf("\n\t\t\tVENCEDOR\n\n");
											sleep(2);
											goto end;
										}
										else{
											printf("\n\t\t\t\t\tVENCEDOR\n\n");
											sleep(2);
											goto end;
										}
								}
							}
						else{
								printf("\nUma letra por R$ %0.2f: ", premios[rprem]);
								fflush(stdin);
								scanf("%c", &resposta);	
								//VEIRIFICANDO SE ERROU A LETRA
							if(verifica_resp(palavra, resposta, j, cont) == cont){
										i++;
										sleep(1);
										printf("Não existe letra %c na palavra\n", resposta);
										sleep(1);
										goto fim;		
								}
							else{
									sleep(1);
									printf("Você acertou!\n");
									sleep(1);		
			//ANALISANDO A RESPOSTA, PREENCHENDO O VETOR AUX E DISTRIBUINDO O MONEY
				for(j=0; j<cont; j++){
						if (resposta==palavra[j]){
							jog[i].valor+= premios[rprem];
							linhas[j]=palavra[j];
						}
					}
				}
			}
i++;
fim:
	if(i==3)
		i=0;
printf("A vez será passada para o jogador: %s", jog[i].nome);
sleep(1.5);
fclose(arq);
end:
sleep(1);
	}while(game_final(respf, palavra) != 0);
}

//GRAVA PREMIOS NO ARQ
void gravar_premios(){
	
	arq = fopen(arquivo, "w");
	
	premios[0]=100;
	premios[1]=900;	
	premios[2]=600;
	premios[3]=1000;
	premios[4]=0.00;
	premios[5]=400;
	premios[6]=200;
	premios[7]=800;
	premios[8]=0.01;
	premios[9]=300;
	premios[10]=500;
	premios[11]=700;
	
	fwrite(&premios, sizeof(premios), 1, arq);
	fclose(arq);
	}
//GRAVA AS PALAVRAS E DICAS NO ARQ
void gravar_dicas(){
	
	arq2 = fopen(arquivo2, "w");
		
		strcpy(dicas[0].pista, "Vegetal");
		dicas[0].qtd= 2;
		strcpy(dicas[1].pista, "Automovel");
		dicas[1].qtd=3;
		strcpy(dicas[2].pista, "Reptil");
		dicas[2].qtd=1;
		
	 	strcpy(dicas[0].vetpalavras[0], "ACELGA");
	 	strcpy(dicas[0].vetpalavras[1], "ALFACE");
	 	strcpy(dicas[0].vetpalavras[2], "CENOURA");
		strcpy(dicas[1].vetpalavras[0], "MOTOR");	
		strcpy(dicas[1].vetpalavras[1], "EMBREAGEM");
		strcpy(dicas[1].vetpalavras[2], "PNEU");
		strcpy(dicas[2].vetpalavras[0], "JARARACA");
		strcpy(dicas[2].vetpalavras[1], "COBRA");
		strcpy(dicas[2].vetpalavras[2], "LAGARTO");
	
	fwrite(&dicas, sizeof(dicas), 1, arq);
	fclose(arq2);
}
//IMPRIME OS DADOS DOS JOGADORES
void jogador_screen (Jogador jog[]){
	printf("\n\t\t        Jogador: %s\t\t      Jogador: %s\t\t Jogador: %s\n\t\t\t==========\t\t      ==========\t\t ==========\n\t\t\tR$ %0.2f \t\t      R$ %0.2f   \t\t R$ %0.2f\n\n", jog[0].nome, jog[1].nome, jog[2].nome, jog[0].valor, jog[1].valor, jog[2].valor);
}
//VERIFICA SE A RESPOSTA FOI ERRADA
int verifica_resp(char *palavra, char resposta, int j, int cont){

	int erro = 0;	
		
		for(j=0; j<cont; j++){
			if (resposta != palavra[j]){
				erro++;
			}		
}
return erro;
}
//FUNÇÃO QUE VERIFICA SE A PALAVRA ESTÁ COM 3 LETRAS OU MENOS
int verifica_word(char *palavra, char *linhas, int j, int cont){
		
		int word = 0;
		
		for(j=0; j<cont; j++){
			if (palavra[j] != linhas[j])
				word++;
}
	if(word <= 3)
		return word;
	else
		return 0;
}
//FUNÇÃO QUE VERIFICA SE O GAME ACABOU
int game_final(char *respf, char *palavra){
//STRCMP PARA COMPARAR AS STRINGS	
		if (strcmp(respf, palavra) == 0)
			return 0;
		else
			return 1;
}
