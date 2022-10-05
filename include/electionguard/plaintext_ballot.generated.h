/// @file plaintext_ballot.generated.h
#pragma once

#include "chaum_pedersen.h"
#include "elgamal.h"
#include "export.h"
#include "group.h"
#include "status.h"
#include "ballot.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PlaintextBallot

/**
 * @brief A unique Ballot ID that is relevant to the external system and must be unique within the dataset of the election.
 * @param[in] handle A pointer to the `eg_plaintext_ballot_selection_t` opaque instance
 * @param[out] out_object_id A pointer to the output ObjectId.  The caller is responsible for freeing it.
 * @return eg_electionguard_status_t indicating success or failure
 * @retval ELECTIONGUARD_STATUS_SUCCESS The function was successfully executed
 * @retval ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC The function was unable to allocate memory
 */
EG_API eg_electionguard_status_t eg_plaintext_ballot_get_object_id(
	eg_plaintext_ballot_t *handle,
	char **out_object_id
	);

#endif // ifndef PlaintextBallot

#ifdef __cplusplus
}
#endif
