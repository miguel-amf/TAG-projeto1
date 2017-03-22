#include <stdio.h>
#include <stdlib.h>

typedef struct 
{
    int id;
    char nome[30];
    char matricula[10];
    struct Aluno *prox;
    
} Aluno;

/*define numero de alunos*/
#define NUM_ALUNOS 39

/*CABECALHO DE FUNCOES*/
int povoaLista (Aluno **);

/*FUNCAO MAIN*/
int main () {

    Aluno **lista;
    
    /*aloca memoria necessaria para o vetor de listas, ja inicializando com null*/
    lista = calloc(NUM_ALUNOS, sizeof(Aluno));
    
    /*Procura no arquivo os nos de alunos, sem contar adjacencia*/
    povoaLista(lista);
    

    free(lista);
    return 0;

}

#define TAM_LINHA 100
/*Percorre o arquivo alunos.txt e povoa a lista de adjacencia*/
int povoaLista(Aluno **lista) {

    /*declara e inicializa variaveis a serem utilizadas*/
    FILE *arqAlunos = NULL;
    char linhaAtual[TAM_LINHA];
    Aluno aluno;
    int i = 0;
    
    
    /*Abre arquivo com nomes e matriculas, em modo leitura*/
    arqAlunos = fopen("alunos.txt", "r");
    if (arqAlunos == NULL) {
        printf("\nERRO: Arquivo alunos.txt nao encontrado\n");
        return 1;
    }
    
    /*
    percorre o arquivo alunos.txt
    vai de linha em linha, supondo formato id#nome#matricula\n
    percorre ate chegar a NUM_ALUNOS ou EOF (equivalente a fscanf ler diferente de 3)
    a cada iteracao, aloca espaco para novo vertice e atribui a ele os valores lidos, e depois adiciona suas adjacencias
    
    "%d#%[a-zA-Z ]#%9s\n", &(aluno.id), aluno.nome, aluno.matricula
    */
    for(i = 0;(fgets(linhaAtual, TAM_LINHA, arqAlunos) != NULL) || i < NUM_ALUNOS; i++) {
        /*Aloca */
        lista[i] = malloc(sizeof(Aluno));    
        printf("id=%d\n",aluno.id);
        printf("nome=%s\n", aluno.nome);
        printf("matricula=%s\n", aluno.matricula);
        
    }
    
    return 0;
}
