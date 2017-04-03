/*
MARCO 2017
UNIVERSIDADE DE BRASILIA
DEPARTAMENTO DE CIENCIA DA COMPUTACAO

TEORIA E APLICACAO DE GRAFOS
DOCENTE RESPONSAVEL:
PROFESSOR DR. DIBIO LEANDRO BORGES

PROJETO 1 - LISTA DE ADJACENCIA DE AMIGOS DA TURMA, ORDENADO POR ORDEM DOS VERTICES

DISCENTES:
GUILHERME GOMES PRANDI 15/0128274
MIGUEL A. MONTAGNER FILHO 13/0127302

*/

/*
TODO: SORT POR QTDADJ
TODO: VERIFICACAO MAIOR CLIQUE
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
    char matricula[10];
    int qtdAdj;
    struct Adj *adj;

} Aluno;



/*define numero de alunos*/
#define NUM_ALUNOS 39
/*define tamanho max de cada linha do arquivo alunos.txt*/
#define TAM_LINHA 100
/*Nome do arquivo a ser lido*/
#define NOME_ARQUIVO "amigos_tag20171.txt"

/*CABECALHO DE FUNCOES*/
int povoaLista (Aluno *);
void pushAdj(Aluno *, int);
void freeAdj(Adj *);
void freeLista(Aluno *);
void Imprime(Aluno *, char);
void Ordena(Aluno *);
void popAdj(Aluno *, int);
Adj *maiorClique(Aluno *);
int determinaMaiorGrau(Aluno *);
void buscaClique(Aluno *, Aluno *, Adj *);
int verificaAmizade(Aluno *, int, int);
void imprimeAdjNo(Adj *);
void imprimeAdj(Aluno);
Adj *pushAdjNo(Adj *, int);
Adj *popAdjNo(Adj *, int);

int maiorGrau;
Aluno maiorCliqueEncontrado;

/*FUNCAO MAIN*/
int main () {


    /*declara vetor de ponteiros para Aluno*/
    Aluno *lista;
    
    /*Inicializa globais*/
    maiorGrau = 0;
    
    maiorCliqueEncontrado.nome[0] = 'M';
    maiorCliqueEncontrado.nome[1] = '\0';
    maiorCliqueEncontrado.qtdAdj = 0;
    maiorCliqueEncontrado.adj = NULL;
    maiorCliqueEncontrado.id = 0;
    maiorCliqueEncontrado.matricula[0] = '\0';


    /*aloca memoria necessaria para o vetor de Aluno, ja inicializando com null*/
    lista = calloc(NUM_ALUNOS, sizeof(Aluno));

    /*Procura no arquivo os vertices, colocando suas adjacencias*/
    if (!povoaLista(lista)) {
        /*trata o erro*/
        printf("ERRO: NAO FOI POSSIVEL TRANSFERIR DO ARQUIVO PARA O PROGRAMA");
        return -1;
    };
    

    /*Atualiza valor do maior grau*/
    determinaMaiorGrau(lista); 

    maiorClique(lista);
    /*Coloca em ordem decrescente de acordo com o grau de cada vertice* /
    
    
    Ordena(lista);

    /*Imprime na saida padrao a lista ordenada, com flag 1 vertice por linha* /
    Imprime(lista, 'l');
    

    /*desaloca a memoria utilizada*/
    freeLista(lista);
    return 0;

}


/*Percorre o arquivo alunos.txt e povoa a lista de adjacencia*/
int povoaLista(Aluno *lista) {

    /*declara e inicializa variaveis a serem utilizadas*/
    FILE *arqAlunos = NULL;
    char linhaAtual[TAM_LINHA];
    int i = 0;
    int idAdj = 0;
    int offsetLinha = 0;



    /*Abre arquivo com nomes e matriculas, em modo leitura*/
    arqAlunos = fopen(NOME_ARQUIVO, "r");
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
    for(i = 0;(i < NUM_ALUNOS) && fgets(linhaAtual, TAM_LINHA, arqAlunos) != NULL; i++) {
    
        /*declara o offselinha, pois o arquivo sera lido linha a linha, e esta variavel ira funcionar como forma de apontar para o caracter a ser lido*/
        offsetLinha = 0;

        /*coloca o id do aluno*/
        lista[i].id = i+1;
        lista[i].qtdAdj = 0;
        sscanf(linhaAtual, "%[a-zA-Z ]#%9[0-9][^#]#", lista[i].nome, lista[i].matricula);
        /*calcula o novo offset, dado que foi lido nome e matricula*/



        offsetLinha = strlen(lista[i].nome) + strlen(lista[i].matricula) + 2;

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

void pushAdj(Aluno *aluno, int id) {

    /*aloca espaco para novo vertice*/
    Adj *adjTemp = malloc(sizeof(Adj));

    /*coloca valor do id*/
    adjTemp->id = id;
    /*aponta o prox do novo vertice para o primeiro elemento da lista de adjacencia do aluno*/
    adjTemp->prox = aluno->adj;
    /*aponta primeiro elemento da lista de adj do aluno para novo vertice*/
    aluno->adj = (Adj *)adjTemp;

}

void popAdj(Aluno *cabeca, int id) {

    Adj *adjTemp = (Adj *) cabeca->adj;
    Adj *adjAnterior = adjTemp;
    
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
        } else {
            adjAnterior = adjTemp;
            adjTemp = (Adj *)adjTemp->prox;
        }
        
    }

}

/*Retorna o maior grau encontrado*/
int determinaMaiorGrau(Aluno *lista) {
    int i, maior;
    maior = 0;
   
    for(i = 0; i< NUM_ALUNOS;i++){
        if (lista[i].qtdAdj > maior) {
            maior = lista[i].qtdAdj;
        }
    }
    maiorGrau = maior;
    return maior;
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
void freeLista(Aluno *lista) {
    int i;
    /*Libera a lista de adjacencia primeiro*/
    for(i = 0; i < NUM_ALUNOS; i++) {
        freeAdj((Adj *)lista[i].adj);
    }
    /*Finalmente libera o vetor*/
    free(lista);

}
/*
Utiliza o algoritmo bubblesort, pela simplicidade de implementacao
*/
void Ordena(Aluno *lista)
{
    int i,j;
    int idTemp, qtdAdjTemp;
    char nomeTemp[30], matriculaTemp[10];
    for(j=0;j<NUM_ALUNOS;j++)
    {
        for(i=j+1;i<NUM_ALUNOS;i++)
        {
            if(lista[j].qtdAdj < lista[i].qtdAdj)
            {
                idTemp = lista[j].id;
                strcpy(nomeTemp, lista[j].nome);
                strcpy(matriculaTemp, lista[j].matricula);
                qtdAdjTemp = lista[j].qtdAdj;

                lista[j].id = lista[i].id;
                strcpy(lista[j].nome, lista[i].nome);
                strcpy(lista[j].matricula, lista[i].matricula);
                lista[j].qtdAdj = lista[i].qtdAdj;

                lista[i].id = idTemp;
                strcpy(lista[i].nome, nomeTemp);
                strcpy(lista[i].matricula, matriculaTemp);
                lista[i].qtdAdj = qtdAdjTemp;
            }
        }
    }
}
/*
arg:
n = normal
l = um por linha

default: normal
*/

void Imprime(Aluno *lista, char arg)
{
    int i;

    if (arg == 'l')
    {
        printf("ID  NOME                  MATRICULA  GRAU\n");
        for(i=0;i<NUM_ALUNOS;i++)
        {

            printf("%02d||",lista[i].id);
            printf("%-20s||", lista[i].nome);
            printf("%s||", lista[i].matricula);
            printf(" %02d ||\n", lista[i].qtdAdj);
        }
        /*Volta para a funcao chamadora*/
        return;
    }
    for(i=0;i<NUM_ALUNOS;i++)
    {
        printf("id=%d\n",lista[i].id);
        printf("nome=%s\n", lista[i].nome);
        printf("matricula=%s\n", lista[i].matricula);
        printf("grau=%d\n\n", lista[i].qtdAdj);
    }
}

Adj *maiorClique(Aluno *lista){

    int i =0;
    Aluno *clique = malloc(sizeof(Aluno));
    Aluno *possivelClique = malloc(sizeof(Aluno));
    possivelClique->adj = NULL;
    clique->adj = NULL;
    
    possivelClique->qtdAdj = 0;
    clique->qtdAdj = 0;
    
    maiorCliqueEncontrado.qtdAdj = 0;

    for(i = 0; i < NUM_ALUNOS; i++){
        /*a cabeca do clique eh zerada*/
        clique->adj = NULL;
        /*Copia lista de adj do aluno vigente para uma nova lista*/
        Adj *cursorAuxiliar = (Adj *) lista[i].adj;
        printf("\nTestando adjs de %s: \nPossivel clique:", lista[i].nome);
        while(cursorAuxiliar != NULL) {
            pushAdj(possivelClique, cursorAuxiliar->id);
            cursorAuxiliar = cursorAuxiliar->prox;
        }
        imprimeAdj(*possivelClique);
        /*coloca o aluno vigente ja dentro do clique*/
        printf("\nclique:");
        pushAdj(clique, lista[i].id);
        imprimeAdj(*clique);
        printf("\n");
        
        buscaClique(lista, clique, (Adj *) possivelClique->adj);       
    
    }
    
    printf("MaiorClique: ");
    imprimeAdj(maiorCliqueEncontrado);
    printf("\n");
    
    return NULL;

}

void buscaClique(Aluno *lista, Aluno *clique, Adj *listaClique) {
    printf("\n###########\nproxima iteracao:\nlistaClique: ");
    imprimeAdjNo(listaClique);
    printf("\nClique: ");
    imprimeAdj(*clique);
   
    printf("\n");
    /*condicao de parada: nao ha mais adjacentes a serem colocados*/
    if(listaClique == NULL) {
        printf("Esta iteracao eh folha, dando backtrack\n##########\n");
        
        /*verifica se encontrou o maior clique ate o momento* /
        if(clique->qtdAdj > maiorCliqueEncontrado.qtdAdj) {
            maiorCliqueEncontrado.adj = (Adj *) clique->adj;
            maiorCliqueEncontrado.qtdAdj = clique->qtdAdj;
        }*/
        
        return;        
    }
    
    /*percorre listaClique, a procura de cliques que contenham cada um dele*/
    Adj *novaListaClique = NULL;
    Adj *cursorLista = listaClique->prox;
    while(listaClique != NULL){
        
        novaListaClique = NULL;
        /*Adj *cursorClique = NULL;*/
        printf("Coloca no clique: %d", listaClique->id);
        /*coloca como possivel participante do clique*/
        pushAdj(clique, listaClique->id);
        clique->qtdAdj++;
        printf("\n");
        
        /*percorre o resto da lista para ver quem mais ainda eh eleito dado o clique atual + novo elemento*/
        if (cursorLista != NULL) cursorLista = cursorLista->prox;
        while(cursorLista!=NULL){
            if (verificaAmizade(lista, cursorLista->id, listaClique->id)) 
            {

                novaListaClique = (Adj *)pushAdjNo(novaListaClique, cursorLista->id);
                
            }
            cursorLista = cursorLista->prox;
        }
        printf("Coloca na novaListaClique:");
        imprimeAdjNo((Adj *) novaListaClique);
        printf("\n");
        
        
        buscaClique(lista, clique, (Adj *) novaListaClique);
        printf("Pop no %d\n", listaClique->id);
        popAdj(clique, listaClique->id);
        
        listaClique = listaClique->prox;
        
        
    
    }
    
    printf("\nFim de iteracao por exaustao de elementos, subindo\n");
    
    
}
/*
1 = amigos
0 = nao amigos
requer lista nao ordenada, com id do aluno casando com indice do vetor
*/
int verificaAmizade(Aluno *lista, int amigo1, int amigo2) {
    
    Adj *cursor = NULL;
    cursor =  (Adj *)lista[amigo1 - 1].adj;
    while(cursor!=NULL) {
        if(cursor->id == amigo2) {
            return 1;
        }   
        cursor = cursor->prox;
    }
    return 0;
}

void imprimeAdj(Aluno aluno) {
    Adj *cursor = NULL;
    cursor = (Adj *)aluno.adj;
    while(cursor != NULL) {
        printf("->%d", cursor->id);
        cursor = (Adj *)cursor->prox;
    }
}

void imprimeAdjNo(Adj *adjLista) {
    while(adjLista != NULL) {
        printf("->%d", adjLista->id);
        adjLista = adjLista->prox;
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
    while(inicial!= NULL) {
        nova = pushAdjNo(inicial, inicial->id);
        inicial = inicial->prox;
        
    }
    return nova;

}
