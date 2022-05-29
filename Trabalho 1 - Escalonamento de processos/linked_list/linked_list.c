#include "linked_list.h"

LIST_NODE* create_node(void* elemento) {
    LIST_NODE* node;

    node = (LIST_NODE*) malloc(sizeof(LIST_NODE));
    if (node == NULL) {
        printf("Error allocating memory for list node\n");
        return NULL;
    }

    node->elemento = elemento;
    node->previous_node = NULL;
    node->next_node = NULL;

    return node;
}

LIST_HEAD* create_head() {
    LIST_HEAD* head;

    head = (LIST_HEAD*) malloc(sizeof(LIST_HEAD));
    if (head == NULL) {
        printf("Error allocating memory for list head\n");
        return NULL;
    }

    head->first_node = NULL;

    return head;
}

void append_node(LIST_HEAD* list_head, LIST_NODE* node) {
    list_head->list_size += 1;

    if (list_head->first_node == NULL) {
        list_head->first_node = node;
        list_head->last_node = node;
        return;
    }

    list_head->last_node->next_node = node;
    node->previous_node = list_head->last_node;
    list_head->last_node = node;

    return;
}

void remove_node(LIST_HEAD* list_head, LIST_NODE* node) {
    list_head->list_size -= 1;

    if (node == list_head->last_node) {
        list_head->last_node = list_head->last_node->previous_node;
    }
    else {
        node->next_node->previous_node = node->previous_node;
    }

    if (node == list_head->first_node) {
        list_head->first_node = list_head->first_node->next_node;
    }
    else {
        node->previous_node->next_node = node->next_node;
    }

    return;
}

LIST_NODE* dequeue(LIST_HEAD* list_head) {
    LIST_NODE* first_node;

    first_node = list_head->first_node;
    remove_node(list_head, list_head->first_node);

    return first_node;
}

void enqueue(LIST_HEAD* list_head, void *element) {
    append_node(list_head, create_node((void *) element));
}