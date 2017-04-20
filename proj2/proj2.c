/*
MARCO 2017
UNIVERSIDADE DE BRASILIA
DEPARTAMENTO DE CIENCIA DA COMPUTACAO

TEORIA E APLICACAO DE GRAFOS
DOCENTE RESPONSAVEL:
PROFESSOR DR. DIBIO LEANDRO BORGES

PROJETO 2 - Grafo CIC e Caminhos Críticos das Disciplinas

DISCENTES:
GUILHERME GOMES PRANDI 15/0128274
MIGUEL A. MONTAGNER FILHO 13/0127302

COMPILADO UTILIZANDO gcc -ansi -Wall

*/


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
    int qtdAdj;
    struct Adj *adj;

} Disciplina;



/*define numero de disciplinas*/
#define NUM_DISC 36
/*define tamanho max de cada linha do arquivo de entrada*/
#define TAM_LINHA 100
/*Nome do arquivo a ser lido*/
#define NOME_ARQUIVO "disciplinas.txt"

/*CABECALHO DE FUNCOES*/
int povoaLista (Disciplina *);
void pushAdj(Disciplina *, int);
void freeAdj(Adj *);
void freeLista(Disciplina *);
void Imprime(Disciplina *, char);
void Ordena(Disciplina *);
void popAdj(Disciplina *, int);
int verificaAmizade(Disciplina *, int, int);
void imprimeAdj(Disciplina *, Disciplina);
Adj *pushAdjNo(Adj *, int);
Adj *popAdjNo(Adj *, int);
Adj *copiaListaAdj(Adj *);

/*Declaracao de globais*/
int maiorGrau;


/*
ESTRUTURA DE DADOS UTILIZADA:
VETOR DE CABECAS (STRUCT Disciplina) DE LISTA (STRUCT ADJ)
EXEMPLO:
VETOR Disciplina     LISTA ADJACENCIA
    0               ->3->5->8->NULL
    1               ->2->18->NULL
    ...             ...

A UTILIZACAO DE UM VETOR FOI ESCOLHIDA PARA PROMOVER MELHOR DESEMPENHO
NOS ACESSOS A LISTA, POIS O ID CONTIDO EM CADA NO PODE SER UTILIZADO PARA
UM ACESSO DIRETO, SEM NECESSIDADE DE PERCORRER TODA A LISTA PARA ENCONTRA-LO
*/

/*FUNCAO MAIN*/
int main () {


    /*declara vetor de ponteiros para Disciplina*/
    Disciplina *lista;



    /*Imprime mensagens guia*/
    printf("Ola, este programa pega o arquivo 'disciplinas.txt' e:\n(1) Carrega o Grafo com Lista de Adjacencia das Disciplinas e seus pre-requisitos.");


    /*aloca memoria necessaria para o vetor de Disciplina*/
    lista = calloc(NUM_DISC, sizeof(Disciplina));

    /*Procura no arquivo os vertices, colocando suas adjacencias*/
    if (!povoaLista(lista)) {
        /*trata o erro*/
        printf("ERRO: NAO FOI POSSIVEL TRANSFERIR DO ARQUIVO PARA O PROGRAMA\n");
        return -1;
    };




    printf("\n\nPressione ENTER para imprimir a lista de disciplinas");
    getchar();

    /*Ordena a lista com bubbleSort*/
    Ordena(lista);

    /*Imprime na saida padrao a lista ordenada, com flag 1 vertice por linha*/
    Imprime(lista, 'l');

    printf("\nFIM DO PROGRAMA\n");
    /*desaloca a memoria utilizada*/
    freeLista(lista);
    return 0;

}


/*Percorre o arquivo Disciplinas.txt e povoa a lista de adjacencia*/
int povoaLista(Disciplina *lista) {

    /*declara e inicializa variaveis a serem utilizadas*/
    FILE *arqDisciplinas = NULL;
    char linhaAtual[TAM_LINHA];
    int i = 0;
    int idAdj = 0;
    int offsetLinha = 0;



    /*Abre arquivo com nomes e matriculas, em modo leitura*/
    arqDisciplinas = fopen(NOME_ARQUIVO, "r");
    if (arqDisciplinas == NULL) {
        printf("\nERRO: Arquivo Disciplinas.txt nao encontrado\n");
        return -1;
    }

    /*
    percorre o arquivo Disciplinas.txt
    vai de linha em linha, colocando em uma string, supondo formato nome#matricula#idAdj1#idAdj2#...#idAdjn#\n
    percorre ate chegar a NUM_DISC ou EOF (feof == 0)
    a cada iteracao, aloca espaco para novo vertice e atribui a ele os valores lidos, e depois adiciona suas adjacencias
    supoe id do Disciplina a partir da ordem de entrada

    */
    /*
    no for, primeira condicao de saida verifica se houve erro
    segunda condicao se ja leu todos os Disciplinas registrados
    terceira se chegou no final do arquivo
    */
    for(i = 0;(i < NUM_DISC) && fgets(linhaAtual, TAM_LINHA, arqDisciplinas) != NULL; i++) {

        /*declara o offselinha, pois o arquivo sera lido linha a linha, e esta variavel ira funcionar como forma de apontar para o caracter a ser lido*/
        offsetLinha = 0;

        /*coloca o id do Disciplina*/
        lista[i].id = i+1;
        lista[i].qtdAdj = 0;
        sscanf(linhaAtual, "%9[0-9][^#]#", lista[i].nome);
        /*calcula o novo offset, dado que foi lido nome e matricula*/



        offsetLinha = strlen(lista[i].nome) + 1;

        /*Le os adjacentes, e sai povoando a lista de adjacencia*/
        while(sscanf(&linhaAtual[offsetLinha],"%d#", &idAdj) == 1) {
           /*Coloca o novo vertice no inicio da lista de adj*/
           pushAdj(&lista[i], idAdj);

           (lista[i].qtdAdj)++;
           /*anda na string para o prox valor*/
           for(;linhaAtual[offsetLinha] != '#' || strlen(&linhaAtual[offsetLinha]) == 0; offsetLinha++);
           offsetLinha++;
        }




    }
    return i;
}


/*Recebe um Disciplina por referencia, cria um novo Adj e coloca no inicio da lista apontada pela Disciplina.adj*/
void pushAdj(Disciplina *cabeca, int id) {

    /*aloca espaco para novo vertice*/
    Adj *adjTemp = malloc(sizeof(Adj));

    /*coloca valor do id*/
    adjTemp->id = id;
    /*aponta o prox do novo vertice para o primeiro elemento da lista de adjacencia da disciplina*/
    adjTemp->prox = cabeca->adj;
    /*aponta primeiro elemento da lista de adj da disciplina para novo vertice*/
    cabeca->adj = (Adj *)adjTemp;



}

/*procura na lista dada pela cabeca e tira da lista*/
void popAdj(Disciplina *cabeca, int id) {

    Adj *adjTemp = (Adj *) cabeca->adj;
    Adj *adjAnterior = adjTemp;

    /*checa para ver se a lista eh nula*/
    if(adjTemp == NULL) {
        return;
    }


    if (adjTemp->id == id) {
        cabeca->adj = (Adj *) adjTemp->prox;
        free(adjTemp);
        cabeca->qtdAdj--;
        return;
    }
    adjTemp = (Adj *) adjTemp->prox;
    while(adjTemp != NULL) {
        if(adjTemp->id == id) {
            adjAnterior->prox = (Adj *) adjTemp->prox;
            cabeca->qtdAdj--;
            free(adjTemp);
            break;
        } else {
            adjAnterior = adjTemp;
            adjTemp = (Adj *)adjTemp->prox;
        }

    }

}

void freeAdj(Adj *adj) {

    Adj *temp = adj;
    Adj *next = NULL;
    while (temp != NULL) {
        next = (Adj *)temp->prox;
        free(temp);
        temp = next;
    }

}
void freeLista(Disciplina *lista) {
    int i;
    /*Libera a lista de adjacencia primeiro*/
    for(i = 0; i < NUM_DISC; i++) {
        freeAdj((Adj *)lista[i].adj);
    }
    /*Finalmente libera o vetor*/
    free(lista);

}
/*
Utiliza o algoritmo bubblesort, pela simplicidade de implementacao
*/
void Ordena(Disciplina *lista)
{
    int i,j;
    int idTemp, qtdAdjTemp;
    char nomeTemp[30];
    for(j=0;j<NUM_DISC;j++)
    {
        for(i=j+1;i<NUM_DISC;i++)
        {
            if(lista[j].qtdAdj < lista[i].qtdAdj)
            {
                idTemp = lista[j].id;
                strcpy(nomeTemp, lista[j].nome);
                qtdAdjTemp = lista[j].qtdAdj;

                lista[j].id = lista[i].id;
                strcpy(lista[j].nome, lista[i].nome);
                lista[j].qtdAdj = lista[i].qtdAdj;

                lista[i].id = idTemp;
                strcpy(lista[i].nome, nomeTemp);
                lista[i].qtdAdj = qtdAdjTemp;
            }
        }
    }
}


/*
imprime a lista toda, de formas diferentes dependendo do parametro
arg:
n = normal
l = um por linha

default: normal
*/
void Imprime(Disciplina *lista, char arg)
{
    int i;

    if (arg == 'l')
    {
        printf("ID  DISCIPLINA            GRAU\n");
        for(i=0;i<NUM_DISC;i++)
        {

            printf("%02d||",lista[i].id);
            printf("%-20s||", lista[i].nome);
            printf(" %02d ||\n", lista[i].qtdAdj);
        }
        /*Volta para a funcao chamadora*/
        return;
    }
    for(i=0;i<NUM_DISC;i++)
    {
        printf("id=%d\n",lista[i].id);
        printf("nome=%s\n", lista[i].nome);
        printf("grau=%d\n\n", lista[i].qtdAdj);
    }
}


void imprimeAdj(Disciplina *lista, Disciplina materia) {
    Adj *cursor = NULL;
    cursor = (Adj *)materia.adj;
    while(cursor != NULL) {
        printf("\n(%d)%s", cursor->id, lista[cursor->id - 1].nome);
        cursor = (Adj *)cursor->prox;
    }
}



Adj *pushAdjNo(Adj *adj, int id) {
    Adj *aux1 = NULL;
    Adj *inicio = adj;
    if(adj == NULL) {
        aux1  = malloc(sizeof(Adj));
        aux1->id = id;
        aux1->prox = NULL;
        return aux1;
    }

    while(adj->prox != NULL){
        adj = (Adj *)adj->prox;
    }
    aux1  = malloc(sizeof(Adj));
    aux1->id = id;
    aux1->prox = NULL;
    adj->prox = (Adj *)aux1;

    return inicio;

}

Adj *popAdjNo(Adj *adj, int id) {
    Adj *inicio = (Adj *) adj;
    Adj *aux = NULL;
    if(adj->prox == NULL) {
        free(adj);
        return NULL;
    }
    aux = adj->prox;
    while(aux->id != id) {
        adj = (Adj *)adj->prox;
        aux = (Adj *)adj->prox;
    }
    aux = (Adj *)adj->prox;
    adj->prox = (Adj *)aux->prox;
    free (aux);

    return inicio;

}

Adj *copiaListaAdj(Adj *inicial) {
    Adj *nova = NULL;
    Adj *cursor = inicial;
    while(cursor!= NULL) {
        nova = pushAdjNo(nova, cursor->id);
        cursor = cursor->prox;

    }
    return nova;

}
