#ifndef __ELECTIONGUARD_CORE_BALLOT_H_INCLUDED__
#define __ELECTIONGUARD_CORE_BALLOT_H_INCLUDED__

#include "chaum_pedersen.h"
#include "elgamal.h"
#include "export.h"
#include "group.h"

#ifdef __cplusplus
extern "C" {
#endif

// PlaintextBallotSelection

struct eg_plaintext_ballot_selection_s;
typedef struct eg_plaintext_ballot_selection_s eg_plaintext_ballot_selection_t;

// TODO: accept is_placeholder_selection
EG_API eg_plaintext_ballot_selection_t *eg_plaintext_ballot_selection_create(const char *object_id,
                                                                             const char *vote);

EG_API const char *
eg_plaintext_ballot_selection_get_object_id(eg_plaintext_ballot_selection_t *selection);

// CiphertextBallotSelection

struct eg_ciphertext_ballot_selection_s;
typedef struct eg_ciphertext_ballot_selection_s eg_ciphertext_ballot_selection_t;

// no constructors defined.  use `eg_encrypt_selection`

EG_API const char *
eg_ciphertext_ballot_selection_get_object_id(eg_ciphertext_ballot_selection_t *selection);
EG_API eg_element_mod_q_t *
eg_ciphertext_ballot_selection_get_description_hash(eg_ciphertext_ballot_selection_t *selection);

EG_API eg_elgamal_ciphertext_t *
eg_ciphertext_ballot_selection_get_ciphertext(eg_ciphertext_ballot_selection_t *selection);

EG_API eg_disjunctive_chaum_pedersen_proof_t *
eg_ciphertext_ballot_selection_get_proof(eg_ciphertext_ballot_selection_t *selection);

EG_API bool eg_ciphertext_ballot_selection_is_valid_encryption(
  eg_ciphertext_ballot_selection_t *selection, eg_element_mod_q_t *seed_hash,
  eg_element_mod_p_t *public_key, eg_element_mod_q_t *crypto_extended_base_hash);

// CiphertextBallotSelection::crypto_hash_with not provided

// static CiphertextBallotSelection::make not provided

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CORE_BALLOT_H_INCLUDED__ */