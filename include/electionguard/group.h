#ifndef __ELECTIONGUARD_CORE_GROUP_H_INCLUDED__
#define __ELECTIONGUARD_CORE_GROUP_H_INCLUDED__

#include "constants.h"
#include "export.h"
#include "status.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

// ElementModP

struct eg_element_mod_p_s;
typedef struct eg_element_mod_p_s eg_element_mod_p_t;

EG_API eg_electionguard_status_t eg_element_mod_p_new(const uint64_t in_data[MAX_P_LEN],
                                                      eg_element_mod_p_t **out_handle);
EG_API eg_electionguard_status_t eg_element_mod_p_new_unchecked(const uint64_t in_data[MAX_P_LEN],
                                                                eg_element_mod_p_t **out_handle);

EG_API eg_electionguard_status_t eg_element_mod_p_free(eg_element_mod_p_t *handle);

EG_API eg_electionguard_status_t eg_element_mod_p_get_data(eg_element_mod_p_t *handle,
                                                           uint64_t **out_data, size_t *out_size);

EG_API eg_electionguard_status_t eg_element_mod_p_to_hex(eg_element_mod_p_t *handle,
                                                         char **out_hex);

// TODO: ISSUE #129: finish implementation

// ElementModQ

struct eg_element_mod_q_s;
typedef struct eg_element_mod_q_s eg_element_mod_q_t;

EG_API eg_electionguard_status_t eg_element_mod_q_new(const uint64_t in_data[MAX_Q_LEN],
                                                      eg_element_mod_q_t **out_handle);
EG_API eg_electionguard_status_t eg_element_mod_q_new_unchecked(const uint64_t in_data[MAX_Q_LEN],
                                                                eg_element_mod_q_t **out_handle);

EG_API eg_electionguard_status_t eg_element_mod_q_free(eg_element_mod_q_t *handle);

EG_API eg_electionguard_status_t eg_element_mod_q_get_data(eg_element_mod_q_t *handle,
                                                           uint64_t **out_data, size_t *out_size);

EG_API eg_electionguard_status_t eg_element_mod_q_to_hex(eg_element_mod_q_t *handle,
                                                         char **out_hex);

EG_API eg_electionguard_status_t eg_element_mod_q_rand_q_new(eg_element_mod_q_t **out_handle);

// TODO: ISSUE #129: finish implementation

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CORE_GROUP_H_INCLUDED__ */
