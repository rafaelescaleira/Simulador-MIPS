/* * * * * * * * * * * * * * * * * * * * * * * * * * *
*                                                    *
*   Alunos : Rafael Escaleira e Gabriel Spontoni     *
*                                                    *
*   Instruções para Compilar:                        *
*   gcc simmips.c -Wall -std=c99 -pedantic           *
*                                                    *
* * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef _SIM_MIPS_
#define _SIM_MIPS_

#define TRUE 1
#define FALSE 0

/* A memória possui um espaço de 16.384 bytes, multiplicando esse valor por 8, 
obtem-se a quantidade de bits, ou seja, o tamanho do vetor (memória) */

#define MAX_MEMORIA_PRINCIPAl 16384

/* Endereço 12288 da memória, para armazenar o arquivo de entrada */

#define DATA_ENDERECO_MEMORIA 12288

/* $zero (Registrador $zero recebe o valor 0) */

#define REGISTRADOR_ZERO 0

/* Endereço do início da pilha na memória */

#define INICIO_PILHA_MEMORIA 12284

/* Quantidade máxima de registradores */

#define MAX_REGISTRADOR 32

/* Campos da Instrução */

typedef struct _ir {

    unsigned int op;			// Campo OP;
    unsigned int rs;			// Campo RS;
    unsigned int rt;			// Campo RT;
    unsigned int rd;			// Campo RD;
    unsigned int shamt;			// Campo SHAMT;
    unsigned int funct;			// Campo FUNCT;
    int immediate;				// Campo IMMEDIATE;
    unsigned int address;		// Campo ADDRESS;

    /* Váriaveis auxiliares para facilitar a impressão da intrução */

    unsigned int formato_r;		
    unsigned int formato_i;
    unsigned int formato_j;

} tinstrucao;

/* * * * * * * * * * * * Declaração de Funções * * * * * * * * * * * * */

/* Aloca variaveis universais dinamicamente. */

void aloca_variaveis_universais ();

/* Inicializa as variaveis, lê os arquivos, para o programa.
Recebe como parâmetro a linha de comando. */

void inicializa ();

/* Busca pela instrução, a qual pode se retirar os campos. */

unsigned int busca_instrucao ();

/* Decodifica instrução, extraindo assim, todas as instruções
 necessárias. */

tinstrucao decodifica_instrucao ();

/* Recebe um registro contendo os campos da instrução. 
Por fim executa a instrução. */

void executa_instrucao (tinstrucao instrucao);

/* Recebe o nome do arquivo para escrita dos resultados 
da linha de comando. */

void finaliza (char const * argv []);

/* Recebe um arquivo e calcula seu tamanho bytes, */

unsigned int calcular_tamanho_arquivo (FILE * arquivo);

/* Recebe um valor em decimal e o imprime em binário. */

void imprime_binario (unsigned int valor);

/* Imprime a instrução e seus campos. */

void imprime_instrucao (tinstrucao instrucao, int * contador, int IR);

/* Extende o sinal do valor recebido para 32 bits com sinal. */

int sinal_extendido (int instrucao);

/* Extende o sinal do valor recebido para 32 bits sem sinal. */

int zero_extendido (int instrucao);

/* * * * * * * * * * * * * * * * Funções * * * * * * * * * * * * * * * */

unsigned int calcular_tamanho_arquivo (FILE * arquivo) {

    unsigned int posicaoAtual = ftell (arquivo);

    unsigned int tamanho;

    fseek (arquivo, 0, SEEK_END);

    tamanho = ftell (arquivo);

    fseek (arquivo, posicaoAtual, SEEK_SET);
 
    return tamanho;

}

void imprime_binario (unsigned int valor) {

	int i;
	unsigned int r;

 	for (i = 31; i >= 0; i --) {

    	r = valor >> i;

    	if(r & 1) {

        	printf("1");

    	}

    	else {

        printf("0");

    }

 }
  
 printf("\n");

}

void imprime_instrucao (tinstrucao instrucao, int * contador, int IR) {

    printf("\n\t%dª Instrução : ", * contador);
    imprime_binario (IR);

    printf("\n");

    if (instrucao.formato_r == TRUE) {

        printf("\tFormato R\n");

    }

    else if (instrucao.formato_i == TRUE) {

        printf("\tFormato I\n");

    }

    else if (instrucao.formato_j == TRUE) {

        printf("\tFormato J\n");

    }

    printf("OP : %d\n", instrucao.op);
    printf("RS : %d\n", instrucao.rs);
    printf("RT : %d\n", instrucao.rt);
    printf("RD : %d\n", instrucao.rd);
    printf("SHAMP : %d\n", instrucao.shamt);
    printf("FUNCT : %d\n", instrucao.funct);
    printf("IMMEDIATE : %d\n", instrucao.immediate);
    printf("ADDRESS : %d\n", instrucao.address);

    printf("\n");

    * contador = * contador + 1;

}

int sinal_extendido (int instrucao) {

    int mascara;

    if(instrucao < 0) {

        mascara = 0b11111111111111110000000000000000;
        instrucao = (mascara | instrucao);

    }

    else if (instrucao > 0){

        mascara = 0b00000000000000001111111111111111;
        instrucao = (mascara & instrucao);

    }

    return instrucao;

}

int zero_extendido (int instrucao) {

    int mascara;

    mascara = 0b00000000000000001111111111111111;
    instrucao = (mascara & instrucao);

    return instrucao;

}

#endif
