#ifndef __ELECTIONGUARD_CORE_HASH_H_INCLUDED__
#define __ELECTIONGUARD_CORE_HASH_H_INCLUDED__

#include "export.h"
#include "group.h"

#ifdef __cplusplus
extern "C" {
#endif

EG_API eg_element_mod_q_t *eg_hash_elems_string(const char *a);
EG_API eg_element_mod_q_t *eg_hash_elems_strings(const char **a, int size);
EG_API eg_element_mod_q_t *eg_hash_elems_int(const uint64_t a);

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CORE_HASH_H_INCLUDED__ */