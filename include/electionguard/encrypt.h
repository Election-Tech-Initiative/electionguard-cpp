/// @file encrypt.h
#ifndef __ELECTIONGUARD_CPP_ENCRYPT_H_INCLUDED__
#define __ELECTIONGUARD_CPP_ENCRYPT_H_INCLUDED__

#include "ballot.h"
#include "election.h"
#include "export.h"
#include "status.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// EncryptionDevice

struct eg_encryption_device_s;
typedef struct eg_encryption_device_s eg_encryption_device_t;

EG_API eg_electionguard_status_t eg_encryption_device_new(const uint64_t in_uuid,
                                                          const char *in_location,
                                                          eg_encryption_device_t **out_handle);

EG_API eg_electionguard_status_t eg_encryption_device_free(eg_encryption_device_t *handle);

EG_API eg_electionguard_status_t eg_encryption_device_get_hash(eg_encryption_device_t *handle,
                                                               eg_element_mod_q_t **out_hash);

// EncryptionMediator

struct eg_encryption_mediator_s;
typedef struct eg_encryption_mediator_s eg_encryption_mediator_t;

EG_API eg_electionguard_status_t eg_encryption_mediator_new(
  eg_internal_election_description_t *in_metadata, eg_ciphertext_election_context_t *in_context,
  eg_encryption_device_t *in_encryption_device, eg_encryption_mediator_t **out_handle);

EG_API eg_electionguard_status_t eg_encryption_mediator_free(eg_encryption_mediator_t *handle);

EG_API eg_electionguard_status_t eg_encryption_mediator_encrypt_ballot(
  eg_encryption_mediator_t *handle, eg_plaintext_ballot_t *in_plaintext,
  eg_ciphertext_ballot_t **out_ciphertext_handle);

EG_API eg_electionguard_status_t eg_encryption_mediator_encrypt_ballot_verify_proofs(
  eg_encryption_mediator_t *handle, eg_plaintext_ballot_t *in_plaintext,
  eg_ciphertext_ballot_t **out_ciphertext_handle);

/**
* Encrypt a specific `BallotSelection` in the context of a specific `BallotContest`
*
* @param[in] in_plaintext the selection in the valid input form
* @param[in] in_description the `eg_selection_description_t` from the `ContestDescription`
*                           which defines this selection's structure
* @param[in] in_public_key the `eg_element_mod_p_t` public key (K) used to encrypt the ballot
* @param[in] in_crypto_extended_base_hash the `eg_element_mod_q_t` extended base hash of the election
* @param[in] in_nonce_seed an `eg_element_mod_q_t` used as a header to seed the `Nonce` generated
*                          for this selection. this value can be (or derived from) the
*                          BallotContest nonce, but no relationship is required
* @param[in] in_is_placeholder specifies if this is a placeholder selection
* @param[in] in_should_verify_proofs specify if the proofs should be verified prior to returning (default True)
* @param[out] out_handle a handle to a 1eg_ciphertext_ballot_selection_t`
* @return eg_electionguard_status_t indicating success or failure
* @retval ELECTIONGUARD_STATUS_SUCCESS The function was successfully executed
* @retval ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC The function was unable to allocate memory
*/
EG_API eg_electionguard_status_t eg_encrypt_selection(
  eg_plaintext_ballot_selection_t *in_plaintext, eg_selection_description_t *in_description,
  eg_element_mod_p_t *in_public_key, eg_element_mod_q_t *in_crypto_extended_base_hash,
  eg_element_mod_q_t *in_nonce_seed, bool in_is_placeholder, bool in_should_verify_proofs,
  eg_ciphertext_ballot_selection_t **out_handle);

// TODO: ISSUE #129: eg_encrypt_contest

EG_API eg_electionguard_status_t eg_encrypt_ballot(eg_plaintext_ballot_t *in_plaintext,
                                                   eg_internal_election_description_t *in_metadata,
                                                   eg_ciphertext_election_context_t *in_context,
                                                   eg_element_mod_q_t *in_seed_hash,
                                                   bool in_should_verify_proofs,
                                                   eg_ciphertext_ballot_t **out_handle);

EG_API eg_electionguard_status_t eg_encrypt_ballot_with_nonce(
  eg_plaintext_ballot_t *in_plaintext, eg_internal_election_description_t *in_metadata,
  eg_ciphertext_election_context_t *in_context, eg_element_mod_q_t *in_seed_hash,
  eg_element_mod_q_t *in_nonce, bool in_should_verify_proofs, eg_ciphertext_ballot_t **out_handle);

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CPP_ENCRYPT_H_INCLUDED__ */
