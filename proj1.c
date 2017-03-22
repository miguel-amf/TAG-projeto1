#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int id;
    struct Adj *prox;
} Adj;

typedef struct 
{
    int id;
    char nome[30];
    char matricula[10];
    struct Adj *adj;
    
} Aluno;



/*define numero de alunos*/
#define NUM_ALUNOS 39
/*define tamanho max de cada linha do arquivo alunos.txt*/
#define TAM_LINHA 100

/*CABECALHO DE FUNCOES*/
int povoaLista (Aluno **);
void pushAdj(Aluno *, int);

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


/*Percorre o arquivo alunos.txt e povoa a lista de adjacencia*/
int povoaLista(Aluno **lista) {

    /*declara e inicializa variaveis a serem utilizadas*/
    FILE *arqAlunos = NULL;
    char linhaAtual[TAM_LINHA];
    int i = 0;
    int idAdj = 0;
    int offsetLinha = 0;
    
    
    
    /*Abre arquivo com nomes e matriculas, em modo leitura*/
    arqAlunos = fopen("alunos.txt", "r");
    if (arqAlunos == NULL) {
        printf("\nERRO: Arquivo alunos.txt nao encontrado\n");
        return -1;
    }
    
    /*
    percorre o arquivo alunos.txt
    vai de linha em linha, colocando em uma string, supondo formato nome#matricula#idAdj1#idAdj2#...#idAdjn#\n
    percorre ate chegar a NUM_ALUNOS ou EOF (feof == 0)
    a cada iteracao, aloca espaco para novo vertice e atribui a ele os valores lidos, e depois adiciona suas adjacencias
    supoe id do aluno a partir da ordem de entrada
    
    */
    /*
    no for, primeira condicao de saida verifica se houve erro
    segunda condicao se ja leu todos os alunos registrados
    terceira se chegou no final do arquivo
    */
    for(i = 0;(/*!feof(arqAlunos) &&*/ (i < NUM_ALUNOS) && fgets(linhaAtual, TAM_LINHA, arqAlunos) != NULL); i++) {
        
        /*declara o offselinha, pois o arquivo sera lido linha a linha, e esta variavel ira funcionar como forma de apontar para o caracter a ser lido*/
        offsetLinha = 0;
    
        /*Aloca */
        lista[i] = malloc(sizeof(Aluno));
        /*coloca o id do aluno*/
        lista[i]->id = i+1;
        sscanf(linhaAtual, "%[a-zA-Z ]#%9[0-9][^#]#", lista[i]->nome, lista[i]->matricula);
        /*calcula o novo offset, dado que foi lido nome e matricula*/
        
        printf("id=%d\n",lista[i]->id);
        printf("nome=%s\n", lista[i]->nome);
        printf("matricula=%s\n", lista[i]->matricula);
        
        offsetLinha = strlen(lista[i]->nome) + strlen(lista[i]->matricula) + 2;
        
        /*Le os adjacentes, e sai povoando a lista de adjacencia*/
        while(sscanf(&linhaAtual[offsetLinha],"%d#", &idAdj) == 1) {
           /*Coloca o novo vertice no inicio da lista de adj*/
           
           pushAdj(lista[i], idAdj);
           /*anda na string para o prox valor*/
           for(;linhaAtual[offsetLinha] != '#' || strlen(&linhaAtual[offsetLinha]) == 0; offsetLinha++);
           offsetLinha++;
        }
        printf("\n");
        

        
        
    }
    return i;
}

void pushAdj(Aluno *aluno, int id) {

    /*aloca espaco para novo vertice*/
    Adj *adjTemp = malloc(sizeof(Adj));
    
    /*coloca valor do id*/
    adjTemp->id = id;
    printf("->%d", id);
    /*aponta o prox do novo vertice para o primeiro elemento da lista de adjacencia do aluno*/
    adjTemp->prox = aluno->adj;
    /*aponta primeiro elemento da lista de adj do aluno para novo vertice*/
    aluno->adj = adjTemp;
    
}
