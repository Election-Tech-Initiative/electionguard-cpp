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

typedef struct eg_linked_list_s eg_linked_list_t;

EG_API eg_electionguard_status_t eg_linked_list_new(eg_linked_list_t **out_handle);

EG_API eg_electionguard_status_t eg_linked_list_free(eg_linked_list_t *in_list);

EG_API eg_electionguard_status_t eg_linked_list_append(eg_linked_list_t *in_list, char *in_key,
                                                       char *in_value);

EG_API eg_electionguard_status_t eg_linked_list_delete_last(eg_linked_list_t *list);

EG_API uint64_t eg_linked_list_get_count(eg_linked_list_t *list);

EG_API eg_electionguard_status_t eg_linked_list_get_element_at(eg_linked_list_t *list,
                                                               uint64_t position, char **out_key,
                                                               char **out_value);

EG_API eg_electionguard_status_t eg_linked_list_get_value_at(eg_linked_list_t *list,
                                                             uint64_t position, char **out_value);

#ifdef __cplusplus
}
#endif

#endif /* __ELECTIONGUARD_CPP_COLLECTIONS_H_INCLUDED__ */
