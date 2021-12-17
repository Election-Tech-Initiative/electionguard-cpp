#ifndef __ELECTIONGUARD_CPP_CONSTANTS_H_INCLUDED__
#define __ELECTIONGUARD_CPP_CONSTANTS_H_INCLUDED__

#include "status.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    char *value;
    char *key;
    struct Node *previous;
    struct Node *next;
};

struct LinkedList {
    uint64_t count;
    struct Node *head;
    struct Node *tail;
};

void eg_electionguard_linked_list_delete_last(struct LinkedList *list);

struct LinkedList *eg_electionguard_linked_list_new()
{
    struct LinkedList *list = (struct LinkedList *)malloc(sizeof(struct LinkedList));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
    return list;
}

void eg_electionguard_linked_list_free(struct LinkedList *list)
{
    if (list == NULL) {
        return;
    }

    while (list->count > 0) {
        eg_electionguard_linked_list_delete_last(list);
    }

    free(list);
}

void eg_electionguard_linked_list_append(struct LinkedList *list, char *key, char *value)
{
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    node->key = key;
    node->value = value;
    node->previous = NULL;
    node->next = NULL;

    // if it is the first one, set it to the head
    if (list->head == NULL) {
        list->head = node;
    }

    if (list->tail != NULL) {
        node->previous = list->tail;
        list->tail->next = node;
    }

    list->tail = node;
    list->count++;
}

void eg_electionguard_linked_list_delete_last(struct LinkedList *list)
{
    if (list->tail == NULL) {
        // list is empty
        return;
    }

    struct Node *to_remove = list->tail;

    // check if it is the only one
    if (list->head == to_remove) {
        list->head == NULL;
    }

    list->tail = to_remove->previous;

    if (list->tail != NULL) {
        list->tail->next = NULL;
    }

    list->count--;
    free(to_remove);
}

#endif /* __ELECTIONGUARD_CPP_CONSTANTS_H_INCLUDED__ */