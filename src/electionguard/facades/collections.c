#include "electionguard/collections.h"

typedef struct eg_linked_list_node_t {
    char *key;
    char *value;
    struct eg_linked_list_node_t *previous;
    struct eg_linked_list_node_t *next;
} eg_linked_list_node_t;

struct eg_linked_list_s {
    uint64_t count;
    eg_linked_list_node_t *head;
    eg_linked_list_node_t *tail;
};

eg_electionguard_status_t eg_linked_list_new(eg_linked_list_t **out_handle)
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

eg_electionguard_status_t eg_linked_list_free(eg_linked_list_t *list)
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

eg_electionguard_status_t eg_linked_list_append(eg_linked_list_t *in_list, char *in_key,
                                                char *in_value)
{
    eg_linked_list_node_t *node = (eg_linked_list_node_t *)malloc(sizeof(eg_linked_list_node_t));
    if (node == NULL) {
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
    node->key = strdup(in_key);
    node->value = strdup(in_value);
    node->previous = NULL;
    node->next = NULL;

    // if a tail is already set
    // then link the previous to this one
    if (in_list->tail != NULL) {
        node->previous = in_list->tail;
        in_list->tail->next = node;
    }

    // if it is the first one, set it to the head
    if (in_list->head == NULL) {
        in_list->head = node;
    }

    // set the tail to the node
    in_list->tail = node;
    in_list->count++;

    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_linked_list_delete_last(eg_linked_list_t *list)
{
    if (list->tail == NULL) {
        // list is empty
        return ELECTIONGUARD_STATUS_SUCCESS;
    }

    eg_linked_list_node_t *to_remove = list->tail;

    // check if it is the only one
    if (list->head == to_remove) {
        list->head = NULL;
    }

    list->tail = to_remove->previous;

    if (list->tail != NULL) {
        list->tail->next = NULL;
    }

    list->count--;
    free(to_remove);
    return ELECTIONGUARD_STATUS_SUCCESS;
}

uint64_t eg_linked_list_get_count(eg_linked_list_t *list) { return list->count; }

eg_electionguard_status_t eg_linked_list_get_element_at(eg_linked_list_t *list, uint64_t position,
                                                        char **out_key, char **out_value)
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

    *out_key = node->key;
    *out_value = node->value;

    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_linked_list_get_value_at(eg_linked_list_t *list, uint64_t position,
                                                      char **out_value)
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

    *out_value = node->value;

    return ELECTIONGUARD_STATUS_SUCCESS;
}
