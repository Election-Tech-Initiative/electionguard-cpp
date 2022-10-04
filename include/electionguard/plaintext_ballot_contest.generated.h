/// @file plaintext_ballot_contest.generated.h
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
 * Get the objectId of the contest, which is the unique id for the contest in a specific ballot style described in the election manifest.
 */
EG_API eg_electionguard_status_t eg_plaintext_ballot_contest_get_object_id(
	eg_plaintext_ballot_contest_t *handle,
	char **out_object_id
	);

/**
 * Get the Size of the selections collection
 */
EG_API uint64_t eg_plaintext_ballot_contest_get_selections_size(
	eg_plaintext_ballot_contest_t *handle
	);


#ifdef __cplusplus
}
#endif
