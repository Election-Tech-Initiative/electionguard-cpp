#ifndef __ELECTIONGUARD_CPP_TRACKER_H_INCLUDED__
#define __ELECTIONGUARD_CPP_TRACKER_H_INCLUDED__

#include "export.h"
#include "group.h"
#include "status.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Get starting hash for given device
 */
EG_API eg_electionguard_status_t eg_get_hash_for_device(uint64_t in_device_uuid,
                                                        uint64_t in_session_uuid,
                                                        uint64_t in_launch_code,
                                                        const char *in_location,
                                                        eg_element_mod_q_t **out_device_hash);

/**
 * Get the rotated code for a particular ballot.
 */
EG_API eg_electionguard_status_t eg_get_rotating_tracker_hash(
  eg_element_mod_q_t *in_previous, uint64_t in_timestamp, eg_element_mod_q_t *in_ballot_hash,
  eg_element_mod_q_t **out_tracker_hash);

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CPP_TRACKER_H_INCLUDED__ */