#ifndef __ELECTIONGUARD_CORE_GROUP_H_INCLUDED__
#define __ELECTIONGUARD_CORE_GROUP_H_INCLUDED__

#include "constants.h"
#include "export.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ElementModP

struct eg_element_mod_p_s;
typedef struct eg_element_mod_p_s eg_element_mod_p_t;

EG_API eg_element_mod_p_t *eg_element_mod_p_create(uint64_t *elem);
EG_API eg_element_mod_p_t *eg_element_mod_p_create_unchecked(uint64_t *elem);

/// returns the array size and places the getter value in the data field
EG_API uint8_t eg_element_mod_p_get(eg_element_mod_p_t *elem, uint64_t **data);

// ElementModQ

struct eg_element_mod_q_s;
typedef struct eg_element_mod_q_s eg_element_mod_q_t;

EG_API eg_element_mod_q_t *eg_element_mod_q_create(const uint64_t elem[MAX_Q_LEN]);
EG_API eg_element_mod_q_t *eg_element_mod_q_create_unchecked(const uint64_t elem[MAX_Q_LEN]);

/// returns the array size and places the getter value in the data field
EG_API uint8_t eg_element_mod_q_get(eg_element_mod_q_t *elem, uint64_t **data);

EG_API eg_element_mod_q_t *eg_rand_q();

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CORE_GROUP_H_INCLUDED__ */
