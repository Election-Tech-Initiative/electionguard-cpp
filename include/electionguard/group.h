#ifndef __ELECTIONGUARD_CORE_GROUP_H_INCLUDED__
#define __ELECTIONGUARD_CORE_GROUP_H_INCLUDED__

#include "export.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct eg_element_mod_p_s;
typedef struct eg_element_mod_p_s eg_element_mod_p_t;

EG_API eg_element_mod_p_t *eg_element_mod_p_new(uint64_t *elem);
EG_API eg_element_mod_p_t *eg_element_mod_p_new_unchecked(uint64_t *elem);

EG_API void eg_element_mod_p_free(eg_element_mod_p_t *selection);

/// returns the array size and places the getter value in the data field
EG_API uint8_t eg_element_mod_p_get(eg_element_mod_p_t *selection, uint64_t **data);

struct eg_element_mod_q_s;
typedef struct eg_element_mod_q_s eg_element_mod_q_t;

EG_API eg_element_mod_q_t *eg_element_mod_q_new(uint64_t *elem);
EG_API eg_element_mod_q_t *eg_element_mod_q_new_unchecked(uint64_t *elem);

EG_API void eg_element_mod_q_free(eg_element_mod_q_t *selection);

/// returns the array size and places the getter value in the data field
EG_API uint8_t eg_element_mod_q_get(eg_element_mod_q_t *selection, uint64_t **data);

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CORE_GROUP_H_INCLUDED__ */