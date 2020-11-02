#ifndef __ELECTIONGUARD_CORE_BALLOT_H_INCLUDED__
#define __ELECTIONGUARD_CORE_BALLOT_H_INCLUDED__

#include "chaum_pedersen.h"
#include "elgamal.h"
#include "export.h"
#include "group.h"
#include "status.h"

#ifdef __cplusplus
extern "C" {
#endif

// PlaintextBallotSelection

struct eg_plaintext_ballot_selection_s;
typedef struct eg_plaintext_ballot_selection_s eg_plaintext_ballot_selection_t;

// TODO: accept is_placeholder_selection
EG_API eg_electionguard_status_t eg_plaintext_ballot_selection_new(
  const char *in_object_id, const char *in_vote, eg_plaintext_ballot_selection_t **out_handle);

EG_API eg_electionguard_status_t
eg_plaintext_ballot_selection_free(eg_plaintext_ballot_selection_t *handle);

EG_API eg_electionguard_status_t eg_plaintext_ballot_selection_get_object_id(
  eg_plaintext_ballot_selection_t *handle, const char **out_object_id);

// CiphertextBallotSelection

struct eg_ciphertext_ballot_selection_s;
typedef struct eg_ciphertext_ballot_selection_s eg_ciphertext_ballot_selection_t;

// no constructors defined.  use `eg_encrypt_selection` in encrypt.h

EG_API eg_electionguard_status_t
eg_ciphertext_ballot_selection_free(eg_ciphertext_ballot_selection_t *handle);

EG_API eg_electionguard_status_t eg_ciphertext_ballot_selection_get_object_id(
  eg_ciphertext_ballot_selection_t *handle, const char **out_object_id);

EG_API eg_electionguard_status_t eg_ciphertext_ballot_selection_get_description_hash(
  eg_ciphertext_ballot_selection_t *handle, eg_element_mod_q_t **out_hash);

EG_API eg_electionguard_status_t eg_ciphertext_ballot_selection_get_ciphertext(
  eg_ciphertext_ballot_selection_t *handle, eg_elgamal_ciphertext_t **out_ciphertext);

EG_API eg_electionguard_status_t eg_ciphertext_ballot_selection_get_crypto_hash(
  eg_ciphertext_ballot_selection_t *handle, eg_element_mod_q_t **out_hash);

EG_API eg_electionguard_status_t eg_ciphertext_ballot_selection_get_proof(
  eg_ciphertext_ballot_selection_t *handle, eg_disjunctive_chaum_pedersen_proof_t **out_proof);

EG_API bool eg_ciphertext_ballot_selection_is_valid_encryption(
  eg_ciphertext_ballot_selection_t *handle, eg_element_mod_q_t *in_seed_hash,
  eg_element_mod_p_t *in_public_key, eg_element_mod_q_t *in_crypto_extended_base_hash);

// CiphertextBallotSelection::crypto_hash_with not provided
// static CiphertextBallotSelection::make not provided

// PlaintextBallotContest

// TODO: implement

struct eg_plaintext_ballot_contest_s;
typedef struct eg_plaintext_ballot_contest_s eg_plaintext_ballot_contest_t;

// CiphertextBallotContest

// TODO: implement

struct eg_ciphertext_ballot_contest_s;
typedef struct eg_ciphertext_ballot_contest_s eg_ciphertext_ballot_contest_t;

// no constructors defined.  use `eg_encrypt_contest` in encrypt.h

// PlaintextBallot

// TODO: implement missing methods

struct eg_plaintext_ballot_s;
typedef struct eg_plaintext_ballot_s eg_plaintext_ballot_t;

EG_API eg_electionguard_status_t eg_plaintext_ballot_free(eg_plaintext_ballot_t *handle);

EG_API eg_electionguard_status_t eg_plaintext_ballot_get_object_id(eg_plaintext_ballot_t *handle,
                                                                   const char **out_object_id);

EG_API eg_electionguard_status_t eg_plaintext_ballot_from_json(char *in_data,
                                                               eg_plaintext_ballot_t **out_handle);
EG_API eg_electionguard_status_t eg_plaintext_ballot_from_bson(uint8_t *in_data, uint64_t in_length,
                                                               eg_plaintext_ballot_t **out_handle);

EG_API eg_electionguard_status_t eg_plaintext_ballot_to_json(eg_plaintext_ballot_t *handle,
                                                             char **out_data, size_t *out_size);

EG_API eg_electionguard_status_t eg_plaintext_ballot_to_bson(eg_plaintext_ballot_t *handle,
                                                             uint8_t **out_data, size_t *out_size);

// CiphertextBallot

// TODO: implement missing methods

struct eg_ciphertext_ballot_s;
typedef struct eg_ciphertext_ballot_s eg_ciphertext_ballot_t;

// no constructors defined.  use `eg_encrypt_ballot` in encrypt.h

EG_API eg_electionguard_status_t eg_ciphertext_ballot_free(eg_ciphertext_ballot_t *handle);

EG_API eg_electionguard_status_t eg_ciphertext_ballot_get_object_id(eg_ciphertext_ballot_t *handle,
                                                                    const char **out_object_id);

EG_API eg_electionguard_status_t eg_ciphertext_ballot_get_tracking_hash(
  eg_ciphertext_ballot_t *handle, eg_element_mod_q_t **out_tracking_hash);

EG_API eg_electionguard_status_t eg_ciphertext_ballot_get_tracking_code(
  eg_ciphertext_ballot_t *handle, const char **out_tracking_code);

EG_API eg_electionguard_status_t eg_ciphertext_ballot_get_timestamp(eg_ciphertext_ballot_t *handle,
                                                                    uint64_t *out_timestamp);

EG_API bool eg_ciphertext_ballot_is_valid_encryption(
  eg_ciphertext_ballot_t *handle, eg_element_mod_q_t *in_seed_hash,
  eg_element_mod_p_t *in_public_key, eg_element_mod_q_t *in_crypto_extended_base_hash);

EG_API eg_electionguard_status_t
eg_ciphertext_ballot_from_json(char *in_data, eg_ciphertext_ballot_t **out_handle);

EG_API eg_electionguard_status_t eg_ciphertext_ballot_from_bson(
  uint8_t *in_data, uint64_t in_length, eg_ciphertext_ballot_t **out_handle);

EG_API eg_electionguard_status_t eg_ciphertext_ballot_to_json(eg_ciphertext_ballot_t *handle,
                                                              char **out_data, uint64_t *out_size);

EG_API eg_electionguard_status_t eg_ciphertext_ballot_to_json_with_nonces(
  eg_ciphertext_ballot_t *handle, char **out_data, uint64_t *out_size);

EG_API eg_electionguard_status_t eg_ciphertext_ballot_to_bson(eg_ciphertext_ballot_t *handle,
                                                              uint8_t **out_data,
                                                              uint64_t *out_size);

EG_API eg_electionguard_status_t eg_ciphertext_ballot_to_bson_with_nonces(
  eg_ciphertext_ballot_t *handle, uint8_t **out_data, uint64_t *out_size);

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CORE_BALLOT_H_INCLUDED__ */
