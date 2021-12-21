/// @file collections.h
#ifndef __ELECTIONGUARD_CPP_COLLECTIONS_H_INCLUDED__
#define __ELECTIONGUARD_CPP_COLLECTIONS_H_INCLUDED__

#include "export.h"
#include "status.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief A singly-linked list data structure composed of strings for keys and strings for values.
 */
typedef struct eg_linked_list_s eg_linked_list_t;

/**
* @brief Create a new linked list
* @param[out] handle A pointer to the `eg_linked_list_t` opaque instance
                     the caller is responsible for freeing the string.
* @return eg_electionguard_status_t indicating success or failure
* @retval ELECTIONGUARD_STATUS_SUCCESS The function was successfully executed
* @retval ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC The function was unable to allocate memory
*/
EG_API eg_electionguard_status_t eg_linked_list_new(eg_linked_list_t **out_handle);

EG_API eg_electionguard_status_t eg_linked_list_free(eg_linked_list_t *handle);

/**
* @brief Append the key value pair to the end of the linked list
* @param[in] handle A pointer to the `eg_linked_list_t` opaque instance
* @param[in] in_key A string value to be used as a key
* @param[in] in_value A string value to be used as a value
* @return eg_electionguard_status_t indicating success or failure
* @retval ELECTIONGUARD_STATUS_SUCCESS The function was successfully executed
* @retval ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC The function was unable to allocate memory
*/
EG_API eg_electionguard_status_t eg_linked_list_append(eg_linked_list_t *handle, char *in_key,
                                                       char *in_value);

/**
* @brief Delete the last value in the list
* @param[in] handle A pointer to the `eg_linked_list_t` opaque instance
* @return eg_electionguard_status_t indicating success or failure
* @retval ELECTIONGUARD_STATUS_SUCCESS The function was successfully executed
*/
EG_API eg_electionguard_status_t eg_linked_list_delete_last(eg_linked_list_t *handle);

/**
* @brief Get the count of items in the list
* @param[in] handle A pointer to the `eg_linked_list_t` opaque instance
* @return uint64_t the count
*/
EG_API uint64_t eg_linked_list_get_count(eg_linked_list_t *list);

/**
* @brief Get the element at the designated position
* @param[in] handle A pointer to the `eg_linked_list_t` opaque instance
* @param[in] position the position in the list
* @param[out] out_key the key at the position
* @param[out] out_value the value at the position
* @return eg_electionguard_status_t indicating success or failure
* @retval ELECTIONGUARD_STATUS_SUCCESS The function was successfully executed
* @retval ELECTIONGUARD_STATUS_ERROR_OUT_OF_RANGE The requested position does not exist
* @retval ELECTIONGUARD_STATUS_ERROR_BAD_ACCESS The node at the position does not exist
*/
EG_API eg_electionguard_status_t eg_linked_list_get_element_at(eg_linked_list_t *handle,
                                                               uint64_t position, char **out_key,
                                                               char **out_value);

/**
* @brief Get the value at the designated position
* @param[in] handle A pointer to the `eg_linked_list_t` opaque instance
* @param[in] position the position in the list
* @param[out] out_value the value at the position
* @return eg_electionguard_status_t indicating success or failure
* @retval ELECTIONGUARD_STATUS_SUCCESS The function was successfully executed
* @retval ELECTIONGUARD_STATUS_ERROR_OUT_OF_RANGE The requested position does not exist
* @retval ELECTIONGUARD_STATUS_ERROR_BAD_ACCESS The node at the position does not exist
*/
EG_API eg_electionguard_status_t eg_linked_list_get_value_at(eg_linked_list_t *handle,
                                                             uint64_t position, char **out_value);

#ifdef __cplusplus
}
#endif

#endif /* __ELECTIONGUARD_CPP_COLLECTIONS_H_INCLUDED__ */
