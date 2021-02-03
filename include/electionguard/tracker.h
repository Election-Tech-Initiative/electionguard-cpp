#ifndef __ELECTIONGUARD_CPP_TRACKER_H_INCLUDED__
#define __ELECTIONGUARD_CPP_TRACKER_H_INCLUDED__

#include "export.h"
#include "group.h"
#include "status.h"

#ifdef __cplusplus
extern "C" {
#endif

EG_API eg_electionguard_status_t eg_get_hash_for_device(uint64_t in_uuid, const char *in_location,
                                                        eg_element_mod_q_t **out_device_hash);

EG_API eg_electionguard_status_t eg_get_rotating_tracker_hash(
  eg_element_mod_q_t *in_previous, uint64_t in_timestamp, eg_element_mod_q_t *in_ballot_hash,
  eg_element_mod_q_t **out_tracker_hash);

EG_API eg_electionguard_status_t
eg_hash_to_words_with_default_separator(eg_element_mod_q_t *in_tracker_hash, char **out_hash_words);
EG_API eg_electionguard_status_t eg_hash_to_words(eg_element_mod_q_t *in_tracker_hash,
                                                  const char *in_separator, char **out_hash_words);

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CPP_TRACKER_H_INCLUDED__ */