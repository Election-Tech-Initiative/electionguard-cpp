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

eg_electionguard_status_t eg_linked_list_free(eg_linked_list_t *handle)
{
    if (handle == NULL) {
        return ELECTIONGUARD_STATUS_SUCCESS;
    }

    while (handle->count > 0) {
        eg_linked_list_delete_last(handle);
    }

    free(handle);
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_linked_list_append(eg_linked_list_t *handle, char *in_key,
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
    if (handle->tail != NULL) {
        node->previous = handle->tail;
        handle->tail->next = node;
    }

    // if it is the first one, set it to the head
    if (handle->head == NULL) {
        handle->head = node;
    }

    // set the tail to the node
    handle->tail = node;
    handle->count++;

    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_linked_list_delete_last(eg_linked_list_t *handle)
{
    if (handle->tail == NULL) {
        // list is empty
        return ELECTIONGUARD_STATUS_SUCCESS;
    }

    eg_linked_list_node_t *to_remove = handle->tail;

    // check if it is the only one
    if (handle->head == to_remove) {
        handle->head = NULL;
    }

    handle->tail = to_remove->previous;

    if (handle->tail != NULL) {
        handle->tail->next = NULL;
    }

    handle->count--;
    free(to_remove);
    return ELECTIONGUARD_STATUS_SUCCESS;
}

uint64_t eg_linked_list_get_count(eg_linked_list_t *handle) { return handle->count; }

eg_electionguard_status_t eg_linked_list_get_element_at(eg_linked_list_t *handle, uint64_t position,
                                                        char **out_key, char **out_value)
{
    if (handle->count == 0 || position >= handle->count) {
        return ELECTIONGUARD_STATUS_ERROR_OUT_OF_RANGE;
    }

    eg_linked_list_node_t *node = handle->head;

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

eg_electionguard_status_t eg_linked_list_get_value_at(eg_linked_list_t *handle, uint64_t position,
                                                      char **out_value)
{
    if (handle->count == 0 || position >= handle->count) {
        return ELECTIONGUARD_STATUS_ERROR_OUT_OF_RANGE;
    }

    eg_linked_list_node_t *node = handle->head;

    for (uint64_t i = 1; i <= position; i++) {
        node = node->next;
    }

    if (node == NULL) {
        return ELECTIONGUARD_STATUS_ERROR_BAD_ACCESS;
    }

    *out_value = node->value;

    return ELECTIONGUARD_STATUS_SUCCESS;
}
