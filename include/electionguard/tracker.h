#ifndef __ELECTIONGUARD_CORE_TRACKER_H_INCLUDED__
#define __ELECTIONGUARD_CORE_TRACKER_H_INCLUDED__

#include "export.h"
#include "group.h"

#ifdef __cplusplus
extern "C" {
#endif

EG_API eg_element_mod_q_t *eg_get_hash_for_device(uint64_t uuid, const char *location);
EG_API eg_element_mod_q_t *eg_get_rotating_tracker_hash(eg_element_mod_q_t *previous,
                                                        uint64_t timestamp,
                                                        eg_element_mod_q_t *ballot_hash);

EG_API const char *eg_hash_to_words_with_default_separator(eg_element_mod_q_t *tracker_hash);
EG_API const char *eg_hash_to_words(eg_element_mod_q_t *tracker_hash, const char *separator);

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CORE_TRACKER_H_INCLUDED__ */