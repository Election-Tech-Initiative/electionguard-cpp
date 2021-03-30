/// @file ballot.h
#ifndef __ELECTIONGUARD_CPP_BALLOT_H_INCLUDED__
#define __ELECTIONGUARD_CPP_BALLOT_H_INCLUDED__

#include "chaum_pedersen.h"
#include "elgamal.h"
#include "export.h"
#include "group.h"
#include "status.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PlaintextBallotSelection

struct eg_plaintext_ballot_selection_s;

/**
* A BallotSelection represents an individual selection on a ballot.
*
* This class accepts a `vote` string field which has no constraints
* in the ElectionGuard Data Specification, but is constrained logically
* in the application to resolve to `True` or `False`.  This implies that the
* data specification supports passing any string that can be represented as
* an integer, however only 0 and 1 is supported for now.
*
* This class can also be designated as `is_placeholder_selection` which has no
* context to the data specification but is useful for running validity checks internally
*
* an `extended_data` field exists to support any arbitrary data to be associated
* with the selection.  In practice, this field is the cleartext representation
* of a write-in candidate value.  In the current implementation these values are
* discarded when encrypting.
*/
typedef struct eg_plaintext_ballot_selection_s eg_plaintext_ballot_selection_t;

EG_API eg_electionguard_status_t eg_plaintext_ballot_selection_new(
  const char *in_object_id, const uint64_t in_vote, bool in_is_placeholder_selection,
  eg_plaintext_ballot_selection_t **out_handle);

EG_API eg_electionguard_status_t eg_plaintext_ballot_selection_new_with_extended_data(
  const char *in_object_id, const uint64_t in_vote, bool in_is_placeholder_selection,
  const char *in_extended_data_value, uint64_t in_extended_data_length,
  eg_plaintext_ballot_selection_t **out_handle);

EG_API eg_electionguard_status_t
eg_plaintext_ballot_selection_free(eg_plaintext_ballot_selection_t *handle);

/**
* @brief Get the ObjectId
* @param[in] handle A poiner to the `eg_plaintext_ballot_selection_t` opaque instance
* @param[out] out_object_id A pointer to the output object id.  
                            The caller is responsible for freeing the string.
* @return eg_electionguard_status_t indicating success or failure
* @retval ELECTIONGUARD_STATUS_SUCCESS The function was successfully executed
* @retval ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC The function was unable to allocate memory
*/
EG_API eg_electionguard_status_t eg_plaintext_ballot_selection_get_object_id(
  eg_plaintext_ballot_selection_t *handle, char **out_object_id);

// getVote() not provided.

/**
 * Determines if this is a placeholder selection
 */
EG_API bool
eg_plaintext_ballot_selection_get_is_placeholder(eg_plaintext_ballot_selection_t *handle);

/**
 * Given a PlaintextBallotSelection validates that the object matches an expected object
 * and that the plaintext value can resolve to a valid representation
 */
EG_API bool eg_plaintext_ballot_selection_is_valid(eg_plaintext_ballot_selection_t *handle,
                                                   char *in_object_id);

#endif

#ifndef CiphertextBallotSelection

struct eg_ciphertext_ballot_selection_s;

/**
* A CiphertextBallotSelection represents an individual encrypted selection on a ballot.
*
* This class accepts a `description_hash` and a `ciphertext` as required parameters
* in its constructor.
*
* When a selection is encrypted, the `description_hash` and `ciphertext` required fields must
* be populated at construction however the `nonce` is also usually provided by convention.
*
* After construction, the `crypto_hash` field is populated automatically in the `__post_init__` cycle
*
* A consumer of this object has the option to discard the `nonce` and/or discard the `proof`,
* or keep both values.
*
* By discarding the `nonce`, the encrypted representation and `proof`
* can only be regenerated if the nonce was derived from the ballot's seed nonce.  If the nonce
* used for this selection is truly random, and it is discarded, then the proofs cannot be regenerated.
*
* By keeping the `nonce`, or deriving the selection nonce from the ballot nonce, an external system can
* regenerate the proofs on demand.  This is useful for storage or memory constrained systems.
*
* By keeping the `proof` the nonce is not required fotor verify the encrypted selection.
*/
typedef struct eg_ciphertext_ballot_selection_s eg_ciphertext_ballot_selection_t;

// no constructors defined.  use `eg_encrypt_selection` in encrypt.h

EG_API eg_electionguard_status_t
eg_ciphertext_ballot_selection_free(eg_ciphertext_ballot_selection_t *handle);

EG_API eg_electionguard_status_t eg_ciphertext_ballot_selection_get_object_id(
  eg_ciphertext_ballot_selection_t *handle, char **out_object_id);

/**
 * The SelectionDescription hash
 * 
 * @param[out] out_hash_ref An opaque pointer to the description hash.  
 *                          The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_selection_get_description_hash(
  eg_ciphertext_ballot_selection_t *handle, eg_element_mod_q_t **out_hash_ref);

/**
 * Determines if this is a placeholder selection
 */
EG_API bool
eg_ciphertext_ballot_selection_get_is_placeholder(eg_ciphertext_ballot_selection_t *handle);

/**
 * The encrypted representation of the vote field
 * 
 * @param[out] out_ciphertext_ref An opaque pointer to the ciphertext.  
 *                          The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_selection_get_ciphertext(
  eg_ciphertext_ballot_selection_t *handle, eg_elgamal_ciphertext_t **out_ciphertext_ref);

/**
 * The hash of the encrypted values
 * 
 * @param[out] out_hash_ref An opaque pointer to the crypto hash.  
 *                          The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_selection_get_crypto_hash(
  eg_ciphertext_ballot_selection_t *handle, eg_element_mod_q_t **out_hash_ref);

/**
 * The nonce used to generate the encryption. Sensitive &amp; should be treated as a secret
 * 
 * @param[out] out_nonce_ref An opaque pointer to the nonce.  
 *                          The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_selection_get_nonce(
  eg_ciphertext_ballot_selection_t *handle, eg_element_mod_q_t **out_nonce_ref);

/**
 * The proof that demonstrates the selection is an encryption of 0 or 1, 
 * and was encrypted using the `nonce`
 * 
 * @param[out] out_proof_ref An opaque pointer to the proof.  
 *                          The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_selection_get_proof(
  eg_ciphertext_ballot_selection_t *handle, eg_disjunctive_chaum_pedersen_proof_t **out_proof_ref);

/**
* Given an encrypted BallotSelection, validates the encryption state against a specific seed hash and public key.
* Calling this function expects that the object is in a well-formed encrypted state
* with the elgamal encrypted `message` field populated along with
* the DisjunctiveChaumPedersenProof`proof` populated.
* the ElementModQ `description_hash` and the ElementModQ `crypto_hash` are also checked.
*
* @param[in] seed_hash The hash of the SelectionDescription, or
*                      whatever `ElementModQ` was used to populate the `description_hash` field.
* @param[in] in_public_key The election public key
* @param[in] in_crypto_extended_base_hash The extended base hash of the election
*/
EG_API bool eg_ciphertext_ballot_selection_is_valid_encryption(
  eg_ciphertext_ballot_selection_t *handle, eg_element_mod_q_t *in_seed_hash,
  eg_element_mod_p_t *in_public_key, eg_element_mod_q_t *in_crypto_extended_base_hash);

// CiphertextBallotSelection::crypto_hash_with not provided
// static CiphertextBallotSelection::make not provided, use eg_encrypt_selection

#endif

#ifndef PlaintextBallotContest

struct eg_plaintext_ballot_contest_s;
typedef struct eg_plaintext_ballot_contest_s eg_plaintext_ballot_contest_t;

// no constructors defined.  TODO: implement serialization if needed

EG_API eg_electionguard_status_t
eg_plaintext_ballot_contest_free(eg_plaintext_ballot_contest_t *handle);

EG_API eg_electionguard_status_t eg_plaintext_ballot_contest_get_object_id(
  eg_plaintext_ballot_contest_t *handle, char **out_object_id);

/**
 * Get the Size of the selections collection
 */
EG_API size_t
eg_plaintext_ballot_contest_get_selections_size(eg_plaintext_ballot_contest_t *handle);

/**
 * Get a selection at a specific index.
 * 
 * @param[in] in_index The index of the selection
 * @param[out] out_selection_ref An opaque pointer to the selection.  
 *                               The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_plaintext_ballot_contest_get_selection_at_index(
  eg_plaintext_ballot_contest_t *handle, size_t in_index,
  eg_plaintext_ballot_selection_t **out_selection_ref);

#endif

#ifndef CiphertextBallotContest

struct eg_ciphertext_ballot_contest_s;
typedef struct eg_ciphertext_ballot_contest_s eg_ciphertext_ballot_contest_t;

// no constructors defined.  use `eg_encrypt_contest` in encrypt.h

EG_API eg_electionguard_status_t
eg_ciphertext_ballot_contest_free(eg_ciphertext_ballot_contest_t *handle);

EG_API eg_electionguard_status_t eg_ciphertext_ballot_contest_get_object_id(
  eg_ciphertext_ballot_contest_t *handle, char **out_object_id);

/**
 * Hash from contestDescription
 * 
 * @param[out] out_hash_ref An opaque pointer to the description hash.  
 *                          The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_contest_get_description_hash(
  eg_ciphertext_ballot_contest_t *handle, eg_element_mod_q_t **out_hash_ref);

/**
 * Get the Size of the selections collection
 */
EG_API size_t
eg_ciphertext_ballot_contest_get_selections_size(eg_ciphertext_ballot_contest_t *handle);

/**
 * Get a selection at a specific index.
 * 
 * @param[in] in_index The index of the selection
 * @param[out] out_selection_ref An opaque pointer to the selection.  
 *                               The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_contest_get_selection_at_index(
  eg_ciphertext_ballot_contest_t *handle, size_t in_index,
  eg_ciphertext_ballot_selection_t **out_selection_ref);

/**
 * The nonce used to generate the encryption. Sensitive & should be treated as a secret
 * 
 * @param[out] out_nonce_ref An opaque pointer to the ElementModQ nonce.  
 *                           The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_contest_get_nonce(
  eg_ciphertext_ballot_contest_t *handle, eg_element_mod_q_t **out_nonce_ref);

/**
 * The encrypted representation of all of the vote fields (the contest total)
 * 
 * @param[out] out_ciphertext_accumulation_ref An opaque pointer to the ElementModQ nonce.  
 *                                             The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_contest_get_ciphertext_accumulation(
  eg_ciphertext_ballot_contest_t *handle,
  eg_elgamal_ciphertext_t **out_ciphertext_accumulation_ref);

/**
 * Hash of the encrypted values
 * 
 * @param[out] out_hash_ref An opaque pointer to the ElementModQ crypto hash.  
 *                           The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_contest_get_crypto_hash(
  eg_ciphertext_ballot_contest_t *handle, eg_element_mod_q_t **out_hash_ref);

/**
 * The proof demonstrates the sum of the selections does not exceed the maximum
 * available selections for the contest, and that the proof was generated with the nonce
 * 
 * @param[out] out_proof_ref An opaque pointer to the ConstantChaumPedersenProof proof.  
 *                           The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_contest_get_proof(
  eg_ciphertext_ballot_contest_t *handle, eg_disjunctive_chaum_pedersen_proof_t **out_proof_ref);

// CiphertextBallotContest::crypto_hash_with not provided
// static CiphertextBallotContest::make not provided, eg_encrypt_contest

/**
 * Given an encrypted BallotContest, validates the encryption state against a specific seed hash and public key
* by verifying the accumulated sum of selections match the proof.
* Calling this function expects that the object is in a well-formed encrypted state
* with the `ballot_selections` populated with valid encrypted ballot selections,
* the ElementModQ `description_hash`, the ElementModQ `crypto_hash`,
* and the ConstantChaumPedersenProof all populated.
* Specifically, the seed hash in this context is the hash of the ContestDescription,
* or whatever `ElementModQ` was used to populate the `description_hash` field.
 */
EG_API bool eg_ciphertext_ballot_contest_is_valid_encryption(
  eg_ciphertext_ballot_contest_t *handle, eg_element_mod_q_t *in_seed_hash,
  eg_element_mod_p_t *in_public_key, eg_element_mod_q_t *in_crypto_extended_base_hash);

#endif

#ifndef PlaintextBallot

struct eg_plaintext_ballot_s;
typedef struct eg_plaintext_ballot_s eg_plaintext_ballot_t;

// no constructors defined.  Use `eg_plaintext_ballot_from_json` or `eg_plaintext_ballot_from_bson`

EG_API eg_electionguard_status_t eg_plaintext_ballot_free(eg_plaintext_ballot_t *handle);

EG_API eg_electionguard_status_t eg_plaintext_ballot_get_object_id(eg_plaintext_ballot_t *handle,
                                                                   char **out_object_id);

EG_API eg_electionguard_status_t eg_plaintext_ballot_get_ballot_style(eg_plaintext_ballot_t *handle,
                                                                      char **out_ballot_style);

EG_API size_t eg_plaintext_ballot_get_contests_size(eg_plaintext_ballot_t *handle);

/**
 * Get a contest at a specific index.
 * 
 * @param[in] in_index The index of the contest
 * @param[out] out_contest_ref An opaque pointer to the contest.  
 *                               The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_plaintext_ballot_get_contest_at_index(
  eg_plaintext_ballot_t *handle, size_t in_index, eg_plaintext_ballot_contest_t **out_contest_ref);

EG_API eg_electionguard_status_t eg_plaintext_ballot_from_json(char *in_data,
                                                               eg_plaintext_ballot_t **out_handle);
EG_API eg_electionguard_status_t eg_plaintext_ballot_from_bson(uint8_t *in_data, uint64_t in_length,
                                                               eg_plaintext_ballot_t **out_handle);

EG_API eg_electionguard_status_t eg_plaintext_ballot_to_json(eg_plaintext_ballot_t *handle,
                                                             char **out_data, size_t *out_size);

EG_API eg_electionguard_status_t eg_plaintext_ballot_to_bson(eg_plaintext_ballot_t *handle,
                                                             uint8_t **out_data, size_t *out_size);

#endif

#ifndef CiphertextBallot

struct eg_ciphertext_ballot_s;
typedef struct eg_ciphertext_ballot_s eg_ciphertext_ballot_t;

// no constructors defined.  use `eg_encrypt_ballot` in encrypt.h

EG_API eg_electionguard_status_t eg_ciphertext_ballot_free(eg_ciphertext_ballot_t *handle);

EG_API eg_electionguard_status_t eg_ciphertext_ballot_get_object_id(eg_ciphertext_ballot_t *handle,
                                                                    char **out_object_id);

EG_API eg_electionguard_status_t
eg_ciphertext_ballot_get_ballot_style(eg_ciphertext_ballot_t *handle, char **out_ballot_style);

/**
 * Hash of the election manifest
 * 
 * @param[out] out_manifest_hash_ref An opaque pointer to the description hash.  
 *                          The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_get_manifest_hash(
  eg_ciphertext_ballot_t *handle, eg_element_mod_q_t **out_manifest_hash_ref);

/**
 * Previous tracking hash or seed hash
 * 
 * @param[out] out_ballot_code_seed_ref An opaque pointer to the previous tracking or seed hash.  
 *                          The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_get_ballot_code_seed(
  eg_ciphertext_ballot_t *handle, eg_element_mod_q_t **out_ballot_code_seed_ref);

EG_API size_t eg_ciphertext_ballot_get_contests_size(eg_ciphertext_ballot_t *handle);

/**
 * Get a contest at a specific index.
 * 
 * @param[in] in_index The index of the contest
 * @param[out] out_contest_ref An opaque pointer to the contest.  
 *                             The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t
eg_ciphertext_ballot_get_contest_at_index(eg_ciphertext_ballot_t *handle, size_t in_index,
                                          eg_ciphertext_ballot_contest_t **out_contest_ref);

/**
 * Unique ballot tracking code for this ballot
 * 
 * @param[out] out_ballot_code_ref An opaque pointer to the ballot code.  
 *                                   The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_get_ballot_code(
  eg_ciphertext_ballot_t *handle, eg_element_mod_q_t **out_ballot_code_ref);

EG_API eg_electionguard_status_t eg_ciphertext_ballot_get_timestamp(eg_ciphertext_ballot_t *handle,
                                                                    uint64_t *out_timestamp);

/**
 * The nonce used to encrypt this ballot. Sensitive & should be treated as a secret
 * 
 * @param[out] out_nonce_ref An opaque pointer to the seed ballot nonce.  
 *                           The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_get_nonce(eg_ciphertext_ballot_t *handle,
                                                                eg_element_mod_q_t **out_nonce_ref);

/**
 * The hash of the encrypted ballot representation
 * 
 * @param[out] out_hash_ref An opaque pointer to the crypto hash.  
 *                          The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_get_crypto_hash(
  eg_ciphertext_ballot_t *handle, eg_element_mod_q_t **out_hash_ref);

// CiphertextBallot::crypto_hash_with not provided
// static CiphertextBallot::make not provided, use eg_encrypt_ballot

EG_API bool eg_ciphertext_ballot_is_valid_encryption(
  eg_ciphertext_ballot_t *handle, eg_element_mod_q_t *in_manifest_hash,
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

#endif

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CPP_BALLOT_H_INCLUDED__ */
