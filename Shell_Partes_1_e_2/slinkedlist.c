#include <stdio.h>
#include <stdlib.h>
#include "slinkedlist.h"
/*  Nome:Claudio Marcio Cardoso Aires
    Numero USP: 7960748
    Engenharia de Computação - 012*/


/*
	Cria uma lista vazia, contendo apenas o sentinela, e o retorna.
*/
ListNode * listCreate (void){
    ListNode *a;
    a = (ListNode*) malloc(sizeof(ListNode));
    /*Faz com que o sentinela aponte como proximo elemento e como elemento anterior ele mesmo.*/
    a->prev = a;
    a->next = a;
    a->key = 0;
    return(a);
}

/*
	Aloca um nó na memória e o retorna.
	key é passada por parâmetro.
	prev e next = NULL.
*/
ListNode * listNodeAlloc (elem key){
    ListNode *a;
    a = (ListNode*) malloc(sizeof(ListNode));
    /*Apenas aloca o nó com uma key, logo esse nó não aponta para nada*/
    a->prev = NULL;
    a->next = NULL;
    a->key = key;
    return(a);
}

/*
	Procura o nó com a key passada por parâmetro e o retorna.
	Se não encontrar, retorna o sentinela.
*/
ListNode * listSearch (ListNode * sentinel, int key){
    ListNode *a;
    a = (ListNode*) malloc(sizeof(ListNode));
    a = sentinel->next;
  /*Enquanto a não for o sentinela ele vai ficar comparando as key dos elementos com a passada por parametro*/
    while(a!=sentinel)
    {
        if(key == a->key)
        {
            return(a);
        }
        else{
            a = a->next;
        }
    }
    return (sentinel);
}

/*
	Insere o nó x, passado por referência, depois do sentinela.
*/
void listInsertHead (ListNode * sentinel, ListNode * x)
{
    /*Faz com que o antecessor do nó seja o sentinela e o sucessor seja aquele que antes era o sucessor
    do sentinela.*/
    x->next = sentinel->next;
    x->prev = sentinel;
    /*Torna o sentinela o antecessor do novo nó e o novo nó o sucessor do sentinela*/
    sentinel->next->prev = x;
    sentinel->next = x;
}
/*
	Insere o nó x, passado por referência, antes do sentinela.
*/
void listInsertTail (ListNode * sentinel, ListNode * x){
    /*Coloca o novo nó como antecessor do sentinela , o sentinela como seu sucessor,o antigo antecessor
    do sentinela como antecessor do novo nó e faz o novo nó ser sucessor do antigo antecessor do sentinela.*/
    x->prev=sentinel->prev;
    x->next = sentinel;
    x->prev->next = x;
    sentinel->prev = x;
}

/*
	Insere o nó x após o nó p, ambos passados por referência.
*/
void listInsertAfterPoint (ListNode * p, ListNode * x){
    x->prev = p;
    x->next = p->next;
    p->next->prev = x;
    p->next = x;
}

/*
	Remove o nó x da lista, se ele não for o sentinela.
*/
void listDelete (ListNode * sentinel, ListNode * x){
    ListNode *a;
    a = (ListNode*) malloc(sizeof(ListNode));
    /*Procura x na lista.*/
    a = listSearch (sentinel, x->key);
    /*Se x foi encontrado entao 'a' será diferente de sentinela e entao sera deletado.*/
    if(a != sentinel)
    {
        a->next->prev = a->prev;
        a->prev->next = a->next;
        free(a);
    }
}

/*
	Remove todos os nós da lista, menos o sentinela.
*/
void listEmpty (ListNode * sentinel){
    ListNode *a;
    /*Enquanto 'a' nao for sentinela ele sera removido da lista.*/
    while(a!=sentinel)
    {
        a = (ListNode*) malloc(sizeof(ListNode));
        a = sentinel->next;
        /*Faz com que o sucessor de 'a' se torne sucessor de sentinel*/
        sentinel->next = a->next;
        /*Faz com que o antecessor do sucessor de 'a' seja o sentinel.*/
        a->next->prev = sentinel;
        free(a);
    }
}

/*
	Remove todos os nós da lista, inclusive o sentinela.
*/
void listTerminate (ListNode * sentinel){
    listEmpty(sentinel);/*Primeiro remove todos os elementos da lista.*/
    free(sentinel);/*Depois desaloca o sentinela da memoria*/
}

/*
	Retorna 1 caso a lista esteja vazia.
	Retorna 0 caso contrário.
*/
int listIsEmpty (ListNode * sentinel){
    /*Se o sucessor de sentinel for ele mesmo implica que apenas ele esta na lista,logo ela esta vazia.*/
    if(sentinel->next == sentinel)
        return 1;
    else{
        return 0;
    }
}

/*
	Retorna o número de elementos da lista.
*/
int listCount (ListNode * sentinel){
    ListNode *a;
    a = (ListNode*) malloc(sizeof(ListNode));
    /*De inicio a recebe a posiçao do primeiro elemento apos o sentinela.*/
    a= sentinel->next;
    int cont=0;
    /*Enquanto 'a' nao for o sentinel o contador sera aumentado.*/
    while(a!=sentinel)
    {
        cont++;
        a=a->next;
    }
    return (cont);
}

/*
	Imprime os elementos da lista.
*/
void listPrint (ListNode * sentinel){
    ListNode *a;
    a = (ListNode*) malloc(sizeof(ListNode));
    /*'a' recebe o endereço do primeiro nó apos o sentinel.*/
    a= sentinel->next;
    int n;
    while(a!=sentinel)
    {
        /*n sera o a key contida em 'a'*/
        n= a->key;
        printf("(%d) ",n);
        /*Faz com que 'a' seja movido para o nó seguinte*/
        a=a->next;
    }
    printf("\n");
}

