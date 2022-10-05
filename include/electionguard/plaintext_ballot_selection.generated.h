/// @file plaintext_ballot_selection.generated.h
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

#ifndef PlaintextBallotSelection

/**
 * @brief Get the objectId of the selection which is the unique id for the selection in a specific contest described in the election manifest.
 * @param[in] handle A pointer to the `eg_plaintext_ballot_selection_t` opaque instance
 * @param[out] out_object_id A pointer to the output ObjectId.  The caller is responsible for freeing it.
 * @return eg_electionguard_status_t indicating success or failure
 * @retval ELECTIONGUARD_STATUS_SUCCESS The function was successfully executed
 * @retval ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC The function was unable to allocate memory
 */
EG_API eg_electionguard_status_t eg_plaintext_ballot_selection_get_object_id(
	eg_plaintext_ballot_selection_t *handle,
	char **out_object_id
	);

/**
 * @brief Determines if this is a placeholder selection
 * @param[in] handle A pointer to the `eg_plaintext_ballot_selection_t` opaque instance
 * @return The value of the property
 */
EG_API bool eg_plaintext_ballot_selection_get_is_placeholder(
	eg_plaintext_ballot_selection_t *handle
	);

/**
 * @brief Get the plaintext vote
 * @param[in] handle A pointer to the `eg_plaintext_ballot_selection_t` opaque instance
 * @return The value of the property
 */
EG_API uint64_t eg_plaintext_ballot_selection_get_vote(
	eg_plaintext_ballot_selection_t *handle
	);

/**
 * @brief An optional field of arbitrary data, such as the value of a write-in candidate
 * @param[in] handle A pointer to the `eg_plaintext_ballot_selection_t` opaque instance
 * @param[out] out_extended_data A pointer to the output ExtendedData.  The value is a reference and is not owned by the caller.
 * @return eg_electionguard_status_t indicating success or failure
 * @retval ELECTIONGUARD_STATUS_SUCCESS The function was successfully executed
 * @retval ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC The function was unable to allocate memory
 */
EG_API eg_electionguard_status_t eg_plaintext_ballot_selection_get_extended_data(
	eg_plaintext_ballot_selection_t *handle,
	eg_extended_data_t **out_extended_data
	);

/**
 * Given a PlaintextBallotSelection validates that the object matches an expected object and that the plaintext value can resolve to a valid representation
 */
EG_API bool eg_plaintext_ballot_selection_is_valid(
	eg_plaintext_ballot_selection_t *handle,
	char *in_expected_object_id
	);

#endif // ifndef PlaintextBallotSelection

#ifdef __cplusplus
}
#endif
