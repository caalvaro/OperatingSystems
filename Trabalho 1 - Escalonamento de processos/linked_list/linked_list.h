#ifndef LINKED_LIST_H_INCLUDED
#define LINKED_LIST_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>

typedef struct _LIST_NODE LIST_NODE;
typedef struct _LIST_HEAD LIST_HEAD;
typedef struct _IO IO;
typedef struct _Processo Processo;

struct _IO
{
    char tipo;
    Processo *processo;
    int tempo_entrada;
    int tempo_execucao;
    int tempo_saida;
};

struct _Processo
{
    int pid;
    int ppid;
    int tempo_chegada;
    int tempo_servico;
    int tempo_executado;
    int tempo_ciclo_atual;
    int status;
    LIST_HEAD *lista_io;
};

struct _LIST_NODE
{
    void *elemento;
    LIST_NODE *previous_node;
    LIST_NODE *next_node;
};

struct _LIST_HEAD
{
    int list_size;
    LIST_NODE *first_node;
    LIST_NODE *last_node;
};

LIST_NODE *create_node(void *elemento);
LIST_HEAD *create_head();
void append_node(LIST_HEAD *list_head, LIST_NODE *node);
void remove_node(LIST_HEAD *list_head, LIST_NODE *node);
LIST_NODE *dequeue(LIST_HEAD *list_head);
void enqueue(LIST_HEAD *list_head, void *elemento);

#endif