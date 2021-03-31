#ifndef __ELECTIONGUARD_CPP_BALLOT_CODE_H_INCLUDED__
#define __ELECTIONGUARD_CPP_BALLOT_CODE_H_INCLUDED__

#include "export.h"
#include "group.h"
#include "status.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* Get a hash for a specific device.  The hash includes several components
* to guarantee the uniqueness of the device, the session instance, and the election context
*
* It is up to the consuming application to convey meaning to these fields.
*
* @param[in] deviceUuid Unique identifier of device such as a hardware Id
* @param[in] sessionUuid Unique identifier of the application's session instance,
*                           can be randomly generated on startup
* @param[in] launchCode Runtime launch code associated wit ha specific election
* @param[in] location location of device, as an arbitrary string that is meaningful
*                        to the external system
* @param[out] out_handle a handle to an `eg_element_mod_q_t`. Caller is responsible for lifecycle.
* @return eg_electionguard_status_t indicating success or failure
*/
EG_API eg_electionguard_status_t eg_ballot_code_get_hash_for_device(
  uint64_t in_device_uuid, uint64_t in_session_uuid, uint64_t in_launch_code,
  const char *in_location, eg_element_mod_q_t **out_device_hash);

/**
* Get a rotating ballot code based on the previous ballot code.  Useful for chaining ballots.
*
* Though not required by the spec, it can be useful for some use cases to chain ballots together
* And prove an unbroken dataset for a specific encryption device. Typically the rotating ballot code
* Is seeded with the device hash and then each subsequent call consumes the ballot code of the
* previous encrypted ballot creating a linked hash chain between the ballots that can be traversed.
* Other liniking paradigms exists (such as tree strctures) and are beyond the scope of this implementation.
*
* @param[in] in_seed Previous code or starting hash from device
* @param[in] in_timestamp Timestamp or other incrementing integer value
* @param[in] in_ballot_hash Hash of the ballot
* @param[out] out_ballot_code a handle to an `eg_element_mod_q_t`. Caller is responsible for lifecycle.
* @return eg_electionguard_status_t indicating success or failure
*/
EG_API eg_electionguard_status_t eg_ballot_code_get_ballot_code(
  eg_element_mod_q_t *in_seed, uint64_t in_timestamp, eg_element_mod_q_t *in_ballot_hash,
  eg_element_mod_q_t **out_ballot_code);

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CPP_BALLOT_CODE_H_INCLUDED__ */