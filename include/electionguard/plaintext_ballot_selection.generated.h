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

/**
 * Get the objectId of the selection which is the unique id for the selection in a specific contest described in the election manifest.
 */
EG_API eg_electionguard_status_t eg_plaintext_ballot_selection_get_object_id(
	eg_plaintext_ballot_selection_t *handle,
	char **out_object_id
	);

/**
 * Determines if this is a placeholder selection
 */
EG_API bool eg_plaintext_ballot_selection_get_is_placeholder(
	eg_plaintext_ballot_selection_t *handle
	);

/**
 * Get the plaintext vote
 */
EG_API uint64_t eg_plaintext_ballot_selection_get_vote(
	eg_plaintext_ballot_selection_t *handle
	);

/**
 * An optional field of arbitrary data, such as the value of a write-in candidate
 */
EG_API eg_electionguard_status_t eg_plaintext_ballot_selection_get_extended_data(
	eg_plaintext_ballot_selection_t *handle,
	eg_extended_data_t **out_extended_data
	);


#ifdef __cplusplus
}
#endif
