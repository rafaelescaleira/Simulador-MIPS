/* * * * * * * * * * * * * * * * * * * * * * * * * * *
*						     *
*	Alunos : Rafael Escaleira e Gabriel Spontoni *
*						     *
*	Instruções para Compilar:		     *
*	gcc simmips.c -Wall -std=c99		     *
*						     *
* * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simmips.h"

/* Variáveis Universais */

/* Memória principal do processador. */

unsigned char * memoria_principal;

/* Registradores do processador (32). */

int * registradores;

/* Variável necessária para finalizar a execução do programa. */

unsigned int fim_execucao = 2;

/* Hi e Lo, necessário para multiplicação e divisão. */

long long int hi;
long long int lo;

/* Instrução */

unsigned int IR;

/* Registrador $zero, sempre com valor zero. */

static int zero;

/* Posição do código na memória. */

int PC;

/* Pilha */

int sp;

/* Função Principal (Main) */

int main (int argc, char const * argv[]) {

	tinstrucao instrucao;

	inicializa (argv);

	/* Laço que realiza ciclo de execução de uma instrução. */
	/* A cada iteração do laço, uma instrução é executada. */

	while (fim_execucao == 0) {

		/* Lê da memória instrução a executar (apontada por PC) e guarda em IR. */

		IR = busca_instrucao ();

		/* Atualiza PC. */

		PC = PC + 4;

		/* Decodifica instrução em IR: determina campo op e demais campos. */

		instrucao = decodifica_instrucao ();

		/* Executa a intrução obtida anteriormente. */

		executa_instrucao (instrucao);

	}

	/* Coloca os resultados no arquivo */

	if (fim_execucao == 1) {

		finaliza (argv);

	}

	return 0;

}

/* Aloca variaveis universais dinamicamente */

void aloca_variaveis_universais () {

	memoria_principal = (unsigned char *) malloc (MAX_MEMORIA_PRINCIPAl * sizeof (unsigned char));
	registradores = (int *) malloc (MAX_REGISTRADOR * sizeof (int));

}

/* Inicializa as variaveis, lê os arquivos, para o programa.
Recebe como parâmetro a linha de comando. */

void inicializa (char const * argv []) {

	FILE * arquivo_data, * arquivo_text;
	unsigned int tamanho_arquivo_data, tamanho_arquivo_text, i;

	if (* (argv + 1) != NULL && * (argv + 2) != NULL && * (argv + 3) != NULL) {

		aloca_variaveis_universais ();

		for (i = 0; i < MAX_REGISTRADOR; i ++) {

			* (registradores + i) = 0;

		}

		PC = DATA_ENDERECO_MEMORIA;
		zero = REGISTRADOR_ZERO;
		sp = INICIO_PILHA_MEMORIA;
		fim_execucao = 0;
		hi = 0;
		lo = 0;
		* (registradores + 29) = sp;

		arquivo_text = fopen (argv [1], "r");
		arquivo_data = fopen (argv [2], "r");

		tamanho_arquivo_text = calcular_tamanho_arquivo (arquivo_text);
		tamanho_arquivo_data = calcular_tamanho_arquivo (arquivo_data);

		fread (memoria_principal + PC, sizeof (char), tamanho_arquivo_text, arquivo_text);
		fread (memoria_principal, sizeof (char), tamanho_arquivo_data, arquivo_data);

		fclose (arquivo_text);
		fclose (arquivo_data);

	}

	else {

		printf("\n	* * * * * * * * * * * * * * *\n");
		printf("	*     ARQUIVO INEXISTENTE   *\n");
		printf("	* * * * * * * * * * * * * * *\n\n");
		return ;

	}

}

/* Busca pela instrução, a qual pode se retirar os campos */

unsigned int busca_instrucao () {

	IR = (unsigned int) (* (memoria_principal + PC + 3));
	IR = IR << 8;
	IR = IR | (unsigned int) (* (memoria_principal + PC + 2));
	IR = IR << 8;
	IR = IR | (unsigned int) (* (memoria_principal + PC + 1));
	IR = IR << 8;
	IR = IR | (unsigned int) (* (memoria_principal + PC));

	return IR;

}

/* Decodifica instrução, extraindo assim, todas as instruções necessárias */

tinstrucao decodifica_instrucao () {

	tinstrucao instrucao;

	instrucao.op = 0;
	instrucao.rs = 0;
	instrucao.rt = 0;
	instrucao.rd = 0;
	instrucao.shamt = 0;
	instrucao.funct = 0;
	instrucao.immediate = 0;
	instrucao.address = 0;
	instrucao.formato_r = FALSE;
	instrucao.formato_i = FALSE;
	instrucao.formato_j = FALSE;

	instrucao.op = IR >> 26;

	/* Formato R */

	if (instrucao.op == 0) {

		instrucao.formato_r = TRUE;
		instrucao.rs = IR << 6;
		instrucao.rs = instrucao.rs >> 27;
		instrucao.rt = IR << 11;
		instrucao.rt = instrucao.rt >> 27;
		instrucao.rd = IR << 16;
		instrucao.rd = instrucao.rd >> 27;
		instrucao.shamt = IR << 21;
		instrucao.shamt = instrucao.shamt >> 27;
		instrucao.funct = IR << 26;
		instrucao.funct = instrucao.funct >> 26;

	}

	/* Formato J */

	else if (instrucao.op == 3 || instrucao.op == 2) {

		instrucao.formato_j = TRUE;
		instrucao.address = IR << 6;
		instrucao.address = instrucao.address >> 6;

	}

	/* Formato I */

	else {

		instrucao.formato_i = TRUE;
		instrucao.rs = IR << 6;
		instrucao.rs = instrucao.rs >> 27;
		instrucao.rt = IR << 11;
		instrucao.rt = instrucao.rt >> 27;
		instrucao.immediate = IR << 16;
		instrucao.immediate = instrucao.immediate >> 16;

	}

	return instrucao;

}

/* Recebe um registro contendo os campos da instrução. 
Por fim executa a instrução. */

void executa_instrucao (tinstrucao instrucao) {

	int endereco, mascara;

	switch (instrucao.op) {

		case 0:

		switch (instrucao.funct) {

			/* Shift Left Logical */

			case 0:

			* (registradores + instrucao.rd) = * (registradores + instrucao.rt) << instrucao.shamt;

			break;

			/* Shift Right Logical */

			case 2:

			* (registradores + instrucao.rd) = * (registradores + instrucao.rt) >> instrucao.shamt;

			break;

			/* Jump Register */

			case 8:

			PC = * (registradores + instrucao.rs);

			break;

			/* System Call */

			case 12:

			if (* (registradores + 2) == 10) {

				fim_execucao = 1;
				printf("\n");

			}

			else if (* (registradores + 2) == 1) {

				printf("%d", * (registradores + 4));

			}

			else if (* (registradores + 2) == 4) {

				printf("%s", memoria_principal + (* (registradores + 4)));

			}

			else if (* (registradores + 2) == 5) {

				scanf ("%d", (registradores + 2));

			}
			
			else if (* (registradores + 4) == 8) {

				scanf ("%s", (memoria_principal + (* (registradores + 4))));

			}
			
			break;

			/* Move From Hi */

			case 16:

			* (registradores + instrucao.rd) = hi;

			break;

			/* Move From Lo */

			case 18:

			* (registradores + instrucao.rd) = lo;

			break;

			/* Multiply */

			case 24:

			hi = (* (registradores + instrucao.rs)) * (* (registradores + instrucao.rt));
			lo = (* (registradores + instrucao.rs)) * (* (registradores + instrucao.rt));

			break;

			/* Divide */

			case 26:

			lo = (* (registradores + instrucao.rs)) / (* (registradores + instrucao.rt));
			hi = (* (registradores + instrucao.rs)) % (* (registradores + instrucao.rt));

			break;

			/* Add */

			case 32:

			* (registradores + instrucao.rd) = * (registradores + instrucao.rs) + (* (registradores + instrucao.rt));

			break;

			/* Subtract */

			case 34:

			* (registradores + instrucao.rd) = * (registradores + instrucao.rs) - (* (registradores + instrucao.rt));

			break;

			/* And */

			case 36:

			* (registradores + instrucao.rd) = * (registradores + instrucao.rs) & (* (registradores + instrucao.rt));

			break;

			/* Or */

			case 37:

			* (registradores + instrucao.rd) = * (registradores + instrucao.rs) | (* (registradores + instrucao.rt));

			break;

			/* Xor */

			case 38:

			* (registradores + instrucao.rd) = * (registradores + instrucao.rs) ^ (* (registradores + instrucao.rt));

			break;

			/* Nor */

			case 39:

			* (registradores + instrucao.rd) = ~ ((* (registradores + instrucao.rs)) | (* (registradores + instrucao.rt)));

			break;

			/* Set On Less Than */

			case 42:

			if ((* (registradores + instrucao.rs)) < (* (registradores + instrucao.rt))) {

				* (registradores + instrucao.rd) = 1;

			}

			else {

				* (registradores + instrucao.rd) = 0;

			}

			break;

		}

		break;

		/* Formato J : Jump */

		case 2:

		PC = (instrucao.address) * 4;

		break;

		/* Formato J : Jump and Link */

		case 3:

		* (registradores + 31) = PC;
		PC = (instrucao.address * 4);

		break;

		/* Formato I : Branch On Equal */

		case 4:


		if (* (registradores + instrucao.rs) == * (registradores + instrucao.rt)) {

			PC = PC + (sinal_extendido (instrucao.immediate) * 4);

		}

		break;

		/* Formato I : Branch On Not Equal */

		case 5:	


		if ((* (registradores + instrucao.rs)) != (* (registradores + instrucao.rt))) {

			PC = PC + (sinal_extendido (instrucao.immediate) * 4);

		}

		break;

		/* Formato I : Add Immediate */

		case 8:

		* (registradores + instrucao.rt) = * (registradores + instrucao.rs) + sinal_extendido (instrucao.immediate);

		break;

		/* Formato I : Set On Less Than Immediate */

		case 10:


		if ((* (registradores + instrucao.rs)) < sinal_extendido (instrucao.immediate)) {

			* (registradores + instrucao.rt) = 1;

		}

		else {

			* (registradores + instrucao.rt) = 0;

		}

		break;

		/* Formato I : And Immediate */

		case 12:

		* (registradores + instrucao.rt) = * (registradores + instrucao.rs) & zero_extendido (instrucao.immediate);

		break;

		/* Formato I : Or Immediate */

		case 13:

		* (registradores + instrucao.rt) = * (registradores + instrucao.rs) | zero_extendido (instrucao.immediate);

		break;

		/* Formato I : Load Upper Immediate */

		case 15:

		* (registradores + instrucao.rt) = instrucao.immediate << 16;

		break;

		/* Formato I : Load Byte */

		case 32:

		* (registradores + instrucao.rt) = sinal_extendido (* (memoria_principal + (* (registradores + instrucao.rs) + sinal_extendido (instrucao.immediate))));

		break;

		/* Formato I : Load Word */

		case 35:

		endereco = * (registradores + instrucao.rs) + sinal_extendido (instrucao.immediate);

		* (registradores + instrucao.rt) = * (memoria_principal + endereco + 3);
		* (registradores + instrucao.rt) = * (registradores + instrucao.rt) << 8;
		* (registradores + instrucao.rt) = * (registradores + instrucao.rt) | * (memoria_principal + endereco + 2);
		* (registradores + instrucao.rt) = * (registradores + instrucao.rt) << 8;
		* (registradores + instrucao.rt) = * (registradores + instrucao.rt) | * (memoria_principal + endereco + 1);
		* (registradores + instrucao.rt) = * (registradores + instrucao.rt) << 8;
		* (registradores + instrucao.rt) = * (registradores + instrucao.rt) | * (memoria_principal + endereco + 0);

		break;

		/* Formato I : Store Byte */

		case 40:

		mascara = 0b00000000000000000000000011111111;
        * (registradores + instrucao.rt) = (mascara & (* (registradores + instrucao.rt)));
        * (memoria_principal + (* (registradores + instrucao.rs) + sinal_extendido (instrucao.immediate))) = * (registradores + instrucao.rt);

		break;

		/* Formato I : Store Word */

		case 43:

        instrucao.address = * (registradores + instrucao.rs) + sinal_extendido (instrucao.immediate);

        * (memoria_principal + instrucao.address + 3) = * (registradores + instrucao.rt) >> 24;
        * (memoria_principal + instrucao.address + 2) = * (registradores + instrucao.rt) >> 16;
        * (memoria_principal + instrucao.address + 1) = * (registradores + instrucao.rt) >> 8;
        * (memoria_principal + instrucao.address + 0) = * (registradores + instrucao.rt);

		break;

	}

}

/* Recebe o nome do arquivo para escrita dos resultados 
da linha de comando. */

void finaliza (char const * argv []) {

	FILE * arquivo_saida;

	arquivo_saida = fopen (argv[3], "w");

	fprintf(arquivo_saida, "$zero	%d\n", * (registradores));
	fprintf(arquivo_saida, "$at	%d\n", * (registradores + 1));
	fprintf(arquivo_saida, "$v0	%d\n", * (registradores + 2));
	fprintf(arquivo_saida, "$v1	%d\n", * (registradores + 3));
	fprintf(arquivo_saida, "$a0	%d\n", * (registradores + 4));
	fprintf(arquivo_saida, "$a1	%d\n", * (registradores + 5));
	fprintf(arquivo_saida, "$a2	%d\n", * (registradores + 6));
	fprintf(arquivo_saida, "$a3	%d\n", * (registradores + 7));
	fprintf(arquivo_saida, "$t0	%d\n", * (registradores + 8));
	fprintf(arquivo_saida, "$t1	%d\n", * (registradores + 9));
	fprintf(arquivo_saida, "$t2	%d\n", * (registradores + 10));
	fprintf(arquivo_saida, "$t3	%d\n", * (registradores + 11));
	fprintf(arquivo_saida, "$t4	%d\n", * (registradores + 12));
	fprintf(arquivo_saida, "$t5	%d\n", * (registradores + 13));
	fprintf(arquivo_saida, "$t6	%d\n", * (registradores + 14));
	fprintf(arquivo_saida, "$t7	%d\n", * (registradores + 15));
	fprintf(arquivo_saida, "$s0	%d\n", * (registradores + 16));
	fprintf(arquivo_saida, "$s1	%d\n", * (registradores + 17));
	fprintf(arquivo_saida, "$s2	%d\n", * (registradores + 18));
	fprintf(arquivo_saida, "$s3	%d\n", * (registradores + 19));
	fprintf(arquivo_saida, "$s4	%d\n", * (registradores + 20));
	fprintf(arquivo_saida, "$s5	%d\n", * (registradores + 21));
	fprintf(arquivo_saida, "$s6	%d\n", * (registradores + 22));
	fprintf(arquivo_saida, "$s7	%d\n", * (registradores + 23));
	fprintf(arquivo_saida, "$t8	%d\n", * (registradores + 24));
	fprintf(arquivo_saida, "$t9	%d\n", * (registradores + 25));
	fprintf(arquivo_saida, "$k0	%d\n", * (registradores + 26));
	fprintf(arquivo_saida, "$k1	%d\n", * (registradores + 27));
	fprintf(arquivo_saida, "$gp	%d\n", * (registradores + 28));
	fprintf(arquivo_saida, "$sp	%d\n", * (registradores + 29));
	fprintf(arquivo_saida, "$fp	%d\n", * (registradores + 30));
	fprintf(arquivo_saida, "$ra	%d\n", * (registradores + 31));

	fclose (arquivo_saida);

}
