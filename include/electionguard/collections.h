/// @file collections.h
#ifndef __ELECTIONGUARD_CPP_COLLECTIONS_H_INCLUDED__
#define __ELECTIONGUARD_CPP_COLLECTIONS_H_INCLUDED__

#include "export.h"
#include "status.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct eg_linked_list_node_t {
    char *key;
    char *value;
    struct eg_linked_list_node_t *previous;
    struct eg_linked_list_node_t *next;
} eg_linked_list_node_t;

typedef struct eg_linked_list_t {
    uint64_t count;
    eg_linked_list_node_t *head;
    eg_linked_list_node_t *tail;
} eg_linked_list_t;

EG_API inline eg_electionguard_status_t eg_linked_list_delete_last(eg_linked_list_t *list);

EG_API inline eg_electionguard_status_t eg_linked_list_new(eg_linked_list_t **out_handle)
{
    eg_linked_list_t *list = (eg_linked_list_t *)malloc(sizeof(eg_linked_list_t));
    if (list == NULL) {
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
    *out_handle = list;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

EG_API inline eg_electionguard_status_t eg_linked_list_free(eg_linked_list_t *list)
{
    if (list == NULL) {
        return ELECTIONGUARD_STATUS_SUCCESS;
    }

    while (list->count > 0) {
        eg_linked_list_delete_last(list);
    }

    free(list);
    return ELECTIONGUARD_STATUS_SUCCESS;
}

EG_API inline eg_electionguard_status_t eg_linked_list_append(eg_linked_list_t *list, char *key,
                                                              char *value)
{
    eg_linked_list_node_t *node = (eg_linked_list_node_t *)malloc(sizeof(eg_linked_list_node_t));
    if (node == NULL) {
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
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
    return ELECTIONGUARD_STATUS_SUCCESS;
}

inline eg_electionguard_status_t eg_linked_list_delete_last(eg_linked_list_t *list)
{
    if (list->tail == NULL) {
        // list is empty
        return ELECTIONGUARD_STATUS_SUCCESS;
    }

    eg_linked_list_node_t *to_remove = list->tail;

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
    return ELECTIONGUARD_STATUS_SUCCESS;
}

EG_API inline eg_electionguard_status_t
eg_linked_list_get_value_at(eg_linked_list_t *list, uint64_t position, char **out_result)
{
    if (list->count == 0 || position >= list->count) {
        return ELECTIONGUARD_STATUS_ERROR_OUT_OF_RANGE;
    }

    eg_linked_list_node_t *node = list->head;

    for (uint64_t i = 1; i <= position; i++) {
        node = node->next;
    }

    if (node == NULL) {
        return ELECTIONGUARD_STATUS_ERROR_BAD_ACCESS;
    }

    *out_result = node->value;

    return ELECTIONGUARD_STATUS_SUCCESS;
}

#endif /* __ELECTIONGUARD_CPP_COLLECTIONS_H_INCLUDED__ */
