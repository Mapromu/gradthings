/*  SCC0503 - Algoritmos e Estruturas de Dados II - Pratica 3 - Índices Secundários */
// Gabriel Guimarães Vilas Boas Marin NUSP: 11218521
#include "queries.h"

int main(void) {
	FILE *df;
	int comando= 0, contadorDeRegistros = 0, auxiliar = 0, posAux = 0, contadorIndexSecundario = 0, auxNusp = 0;
	int operador = 0;
	tipoAluno *aluno = (tipoAluno*) calloc(1, sizeof(tipoAluno));
	char auxNome[TAM];


	index_p *indexPrimario = carregaIndex(&contadorDeRegistros);
	index_s *indexSecundario = carregaIndexSecundario(&contadorIndexSecundario);

	tipoChave *aux = NULL;

	listaInvertidaElem *listaInvertida = leituraDaListaInvertida();

	do {
	    df = fopen("alunos.dat", "r+");
	    if (df == NULL) criaArquivo(df);
	} while(df == NULL);


	do {
		printf("Operacoes:\n1 - Gravar\n2 - Pesquisar chave primaria\n3 - Pesquisar sobrenome\n4 - Remover\n5 - Dump secundario\n6 - Dump primario \n0 - Finalizar\n\n");
		scanf("%d", &comando);
		switch (comando)	{
		case 1:
			// Gravar registro
			aluno = leituraDosDadosDoAluno();
			if (pesquisaIndex(indexPrimario, aluno->numUSP, contadorDeRegistros) == -1) {
				insereIndex(df, &indexPrimario, *aluno, &contadorDeRegistros);
				insereIndexSec(df, &indexSecundario, &listaInvertida, *aluno, contadorDeRegistros, &contadorIndexSecundario);
				insereRegistro(df, *aluno);
				printf("Aluno inserido !!!\n");
			} else {
				printf("O Aluno já existe ou foi deletado !!!\n");
			}
			
			break;
		case 2:
			printf("\nInsira o NUSP : ");
			scanf("%d", &auxNusp);
			aluno = pesquisaRegistro(df, pesquisaIndex(indexPrimario, auxNusp, contadorDeRegistros));
			if (!aluno || aluno->nota == -1) printf("NUSP não encontrado !!!\n");
			else {
				printf("NUSP : %d\n", aluno->numUSP);
				printf("Primeiro nome : %s\n", aluno->nome);
				printf("Sobrenome : %s\n", aluno->sobrenome);
				printf("Abreviacao do curso (BCC, BSI, ESTAT...) : %s\n", aluno->curso);
				printf("Nota : %f\n", aluno->nota);
			}
			break;
		case 3:
			printf("\nInsira o sobrenome : ");
			scanf("\n%[^\n]s", auxNome);
			printf("Procurando: %s\n", auxNome);
			posAux = pesquisaChaveSecundaria(indexSecundario, auxNome, &contadorIndexSecundario);
			if (posAux != -1) {

				aux = pesquisaListaInvertida(listaInvertida, posAux, contadorDeRegistros, &auxiliar);	

				for (int i = 0; i < auxiliar; i++) {
					aluno = pesquisaRegistro(df, pesquisaIndex(indexPrimario, aux[i], contadorDeRegistros));
					if (!aluno || aluno->nota == -1) {
						continue;
					} else {
						printf("\n\nNUSP : %d\n", aluno->numUSP);
						printf("Primeiro nome : %s\n", aluno->nome);
						printf("Sobrenome : %s\n", auxNome);
						printf("Abreviacao do curso (BCC, BSI, ESTAT...) : %s\n", aluno->curso);
						printf("Nota : %f\n", aluno->nota);
					}
				}

			} else {
				printf("Sobrenome não encontrado !!!\n");
			}
			break;
		case 4:
			printf("Remoção:\n1 - Pelo NUSP\n2 - Pelo sobrenome\n\n");
			scanf("%d", &operador);
			if (operador == 1) {
				// remover pelo NUSP
				printf("\nInsira o NUSP : ");
				scanf("%d", &aluno->numUSP);
				removeRegistro(&indexPrimario, &indexSecundario, aluno->numUSP, &contadorDeRegistros, &contadorIndexSecundario);
			} else if (operador == 2) {
				// remover pelo sobrenome
				printf("\nInsira o sobrenome : ");
				scanf("\n%[^\n]s", auxNome);
				
				if (pesquisaChaveSecundaria(indexSecundario, auxNome, &contadorIndexSecundario) == -1) {
						printf("Sobrenome não encontrado na chave secundária !!!\n");
				} else {
						aux = pesquisaListaInvertida(listaInvertida, pesquisaChaveSecundaria(indexSecundario, auxNome, &contadorIndexSecundario), contadorDeRegistros, &auxiliar);
					// auxiliar = sizeof(aux)/sizeof(listaInvertidaElem);
					printf("ALUNOS ENCONTRADOS:\n");
					for (int i = 0; i < auxiliar; i++) {
						aluno = pesquisaRegistro(df, pesquisaIndex(indexPrimario, aux[i], contadorDeRegistros));
						printf("%d - %s %s\n", i, aluno->nome, aluno->sobrenome);
					}

					printf("Digite o numero de qual deseja remover: ");
					scanf("%d", &operador);
					if (operador >= auxiliar || operador < 0) {
						printf("Numero invalido !!\n");
					} else {
						removeRegistro(&indexPrimario, &indexSecundario, aluno->numUSP, &contadorDeRegistros, &contadorIndexSecundario);
					}	
				}
				
			} else {
				printf("Comando inválido !!!\n");
			}
			break;
		case 5:
			dumpListaSecundaria(indexSecundario, contadorIndexSecundario);
			dumpListaInvertida();
			break;
		case 6:
			dumpIndexPrimario(indexPrimario, contadorDeRegistros);
			dumpFile(df);
			break;
		case 0:
			// Escreve dados
			escreveIndexPrimario(indexPrimario, contadorDeRegistros);
			escreveIndexSecundario(indexSecundario, contadorIndexSecundario);
			// Finalizar registro
			if (aluno) free(aluno);
			if (indexPrimario) free(indexPrimario);
			if (indexSecundario) free(indexSecundario);
			if (aux) free(aux);
			if (listaInvertida) free(listaInvertida);
			finalizaExecucao(df);
			break;
		default:
			printf("\n\nOperação invalida !!!\n\n");
			break;
		}

		/* Programa deve proceder de acordo com a operacao desejada */


	} while(comando > 0);

return 0;

}