#ifndef __ELECTIONGUARD_CORE_GROUP_H_INCLUDED__
#define __ELECTIONGUARD_CORE_GROUP_H_INCLUDED__

#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

struct eg_element_mod_p_s;
typedef struct eg_element_mod_p_s eg_element_mod_p_t;

EG_API eg_element_mod_p_t *eg_element_mod_p_new(uint64_t *elem);
EG_API void eg_element_mod_p_free(eg_element_mod_p_t *selection);

struct eg_element_mod_q_s;
typedef struct eg_element_mod_q_s eg_element_mod_q_t;

EG_API eg_element_mod_q_t *eg_element_mod_q_new(uint64_t *elem);
EG_API void eg_element_mod_q_free(eg_element_mod_q_t *selection);

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CORE_GROUP_H_INCLUDED__ */