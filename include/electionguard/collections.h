/// @file collections.h
#ifndef __ELECTIONGUARD_CPP_COLLECTIONS_H_INCLUDED__
#define __ELECTIONGUARD_CPP_COLLECTIONS_H_INCLUDED__

#include "export.h"
#include "status.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

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

EG_API eg_electionguard_status_t eg_linked_list_new(eg_linked_list_t **out_handle);

EG_API eg_electionguard_status_t eg_linked_list_free(eg_linked_list_t *list);

EG_API eg_electionguard_status_t eg_linked_list_append(eg_linked_list_t *list, char *key,
                                                       char *value);

EG_API eg_electionguard_status_t eg_linked_list_delete_last(eg_linked_list_t *list);

EG_API eg_electionguard_status_t eg_linked_list_get_value_at(eg_linked_list_t *list,
                                                             uint64_t position, char **out_result);

#ifdef __cplusplus
}
#endif

#endif /* __ELECTIONGUARD_CPP_COLLECTIONS_H_INCLUDED__ */
