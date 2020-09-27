#ifndef __ELECTIONGUARD_CORE_ENCRYPT_H_INCLUDED__
#define __ELECTIONGUARD_CORE_ENCRYPT_H_INCLUDED__

#include "ballot.h"
#include "election.h"
#include "export.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// EncryptionDevice

struct eg_encryption_device_s;
typedef struct eg_encryption_device_s eg_encryption_device_t;

EG_API eg_encryption_device_t *eg_encryption_device_create(const uint64_t uuid,
                                                           const char *location);

EG_API eg_element_mod_q_t *eg_encryption_device_get_hash(eg_encryption_device_t *device);

// EncryptionMediator

struct eg_encryption_mediator_s;
typedef struct eg_encryption_mediator_s eg_encryption_mediator_t;

EG_API eg_encryption_mediator_t *
eg_encryption_mediator_create(eg_internal_election_description_t *metadata,
                              eg_ciphertext_election_context_t *context,
                              eg_encryption_device_t *encryption_device);

EG_API eg_ciphertext_ballot_t *eg_encryption_mediator_encrypt(eg_encryption_mediator_t *mediator,
                                                              eg_plaintext_ballot_t *ballot);

/// <summary>
/// Encrypt a specific `BallotSelection` in the context of a specific `BallotContest`
///
/// <param name="plaintext">the selection in the valid input form</param>
/// <param name="description">the `SelectionDescription` from the `ContestDescription`
///                           which defines this selection's structure</param>
/// <param name="elgamal_public_key">the public key (K) used to encrypt the ballot</param>
/// <param name="crypto_extended_base_hash">the extended base hash of the election</param>
/// <param name="nonce_seed">an `ElementModQ` used as a header to seed the `Nonce` generated
///                          for this selection. this value can be (or derived from) the
///                          BallotContest nonce, but no relationship is required</param>
/// <param name="is_placeholder">specifies if this is a placeholder selection</param>
/// <param name="should_verify_proofs">specify if the proofs should be verified prior to returning (default True)</param>
/// <returns>A `CiphertextBallotSelection`</returns>
/// </summary>
EG_API eg_ciphertext_ballot_selection_t *
eg_encrypt_selection(eg_plaintext_ballot_selection_t *plaintext,
                     eg_selection_description_t *description, eg_element_mod_p_t *public_key,
                     eg_element_mod_q_t *crypto_extended_base_hash, eg_element_mod_q_t *nonce_seed,
                     bool is_placeholder, bool should_verify_proofs);

EG_API eg_ciphertext_ballot_contest_t *
eg_encrypt_contest(eg_plaintext_ballot_contest_t *plaintext, eg_contest_description_t *description,
                   eg_element_mod_p_t *public_key, eg_element_mod_q_t *crypto_extended_base_hash,
                   eg_element_mod_q_t *nonce_seed, bool should_verify_proofs);

EG_API eg_ciphertext_ballot_t *eg_encrypt_ballot(eg_plaintext_ballot_contest_t *plaintext,
                                                 eg_internal_election_description_t *metadata,
                                                 eg_ciphertext_election_context_t *context,
                                                 eg_element_mod_q_t *seed_hash,
                                                 bool should_verify_proofs);

EG_API eg_ciphertext_ballot_t *eg_encrypt_ballot_with_nonce(
  eg_plaintext_ballot_contest_t *plaintext, eg_internal_election_description_t *metadata,
  eg_ciphertext_election_context_t *context, eg_element_mod_q_t *seed_hash,
  eg_element_mod_q_t *nonce, bool should_verify_proofs);

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CORE_ENCRYPT_H_INCLUDED__ */
