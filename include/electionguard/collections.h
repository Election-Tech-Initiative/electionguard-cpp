#ifndef __ELECTIONGUARD_CPP_CONSTANTS_H_INCLUDED__
#define __ELECTIONGUARD_CPP_CONSTANTS_H_INCLUDED__

#include "status.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct eg_electionguard_linked_list_node_t {
    char *value;
    char *key;
    struct eg_electionguard_linked_list_node_t *previous;
    struct eg_electionguard_linked_list_node_t *next;
} eg_electionguard_linked_list_node_t;

typedef struct eg_electionguard_linked_list_t {
    uint64_t count;
    eg_electionguard_linked_list_node_t *head;
    eg_electionguard_linked_list_node_t *tail;
} eg_electionguard_linked_list_t;

void eg_electionguard_linked_list_delete_last(eg_electionguard_linked_list_t *list);

eg_electionguard_linked_list_t *eg_electionguard_linked_list_new()
{
    eg_electionguard_linked_list_t *list =
      (eg_electionguard_linked_list_t *)malloc(sizeof(eg_electionguard_linked_list_t));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
    return list;
}

void eg_electionguard_linked_list_free(eg_electionguard_linked_list_t *list)
{
    if (list == NULL) {
        return;
    }

    while (list->count > 0) {
        eg_electionguard_linked_list_delete_last(list);
    }

    free(list);
}

void eg_electionguard_linked_list_append(eg_electionguard_linked_list_t *list, char *key,
                                         char *value)
{
    eg_electionguard_linked_list_node_t *node =
      (eg_electionguard_linked_list_node_t *)malloc(sizeof(eg_electionguard_linked_list_node_t));
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

void eg_electionguard_linked_list_delete_last(eg_electionguard_linked_list_t *list)
{
    if (list->tail == NULL) {
        // list is empty
        return;
    }

    eg_electionguard_linked_list_node_t *to_remove = list->tail;

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