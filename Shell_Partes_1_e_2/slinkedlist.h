#ifndef _slinkedlist_h
#define _slinkedlist_h

/*
	TAD Lista Ligada Circular com Sentinela
*/

typedef int elem;

typedef struct node {
	elem PID;// fork retorna
	elem JID;// eu crio
	struct node * prev;
	struct node * next;
} ListNode;

/*
	Cria uma lista vazia, contendo apenas o sentinela, e o retorna.
*/
ListNode * listCreate (void);

/*
	Aloca um nó na memória e o retorna.
	key é passada por parâmetro.
	prev e next = NULL.
*/
ListNode * listNodeAlloc (elem key);

/*
	Procura o nó com a key passada por parâmetro e o retorna.
	Se não encontrar, retorna o sentinela.
*/
ListNode * listSearch (ListNode * sentinel, int key);

/*
	Insere o nó x, passado por referência, depois do sentinela.
*/
void listInsertHead (ListNode * sentinel, ListNode * x);

/*
	Insere o nó x, passado por referência, antes do sentinela.
*/
void listInsertTail (ListNode * sentinel, ListNode * x);

/*
	Insere o nó x após o nó p, ambos passados por referência.
*/
void listInsertAfterPoint (ListNode * p, ListNode * x);

/*
	Remove o nó x da lista, se ele não for o sentinela.
*/
void listDelete (ListNode * sentinel, ListNode * x);

/*
	Remove todos os nós da lista, menos o sentinela.
*/
void listEmpty (ListNode * sentinel);

/*
	Remove todos os nós da lista, inclusive o sentinela.
*/
void listTerminate (ListNode * sentinel);

/*
	Retorna 1 caso a lista esteja vazia.
	Retorna 0 caso contrário.
*/
int listIsEmpty (ListNode * sentinel);

/*
	Retorna o número de elementos da lista.
*/
int listCount (ListNode * sentinel);

/*
	Imprime os elementos da lista.
*/
void listPrint (ListNode * sentinel);

#endif
