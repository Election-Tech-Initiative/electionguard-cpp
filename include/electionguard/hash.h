#ifndef __ELECTIONGUARD_CORE_HASH_H_INCLUDED__
#define __ELECTIONGUARD_CORE_HASH_H_INCLUDED__

#include "export.h"
#include "group.h"
#include "status.h"

#ifdef __cplusplus
extern "C" {
#endif

EG_API eg_electionguard_status_t eg_hash_elems_string(const char *a,
                                                      eg_element_mod_q_t **out_handle);
EG_API eg_electionguard_status_t eg_hash_elems_strings(const char **a, size_t in_length,
                                                       eg_element_mod_q_t **out_handle);
EG_API eg_electionguard_status_t eg_hash_elems_int(const uint64_t a,
                                                   eg_element_mod_q_t **out_handle);

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CORE_HASH_H_INCLUDED__ */
