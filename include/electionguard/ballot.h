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

#ifndef BallotBoxState

/**
 * Enumeration used when marking a ballot as cast or spoiled
 */
typedef enum eg_ballot_box_state_e {
    /**
     * A ballot that has been explicitly cast
     */
    ELECTIONGUARD_BALLOT_BOX_STATE_CAST = 1,
    /**
     * A ballot that has been explicitly spoiled
     */
    ELECTIONGUARD_BALLOT_BOX_STATE_SPOILED = 2,

    /**
     * A ballot whose state is unknown to ElectionGuard and will not be included in any election results
     */
    ELECTIONGUARD_BALLOT_BOX_STATE_UNKNOWN = 999
} eg_ballot_box_state_t;

#endif

#ifndef ExtendedData

struct eg_extended_data_s;

/**
 * ExtendedData represents any arbitrary data expressible as a string with a length.
 * 
 * This class is used primarily as a field on a selection to indicate a write-in candidate text value
 */
typedef struct eg_extended_data_s eg_extended_data_t;

EG_API eg_electionguard_status_t eg_extended_data_new(char *in_value, uint64_t in_length,
                                                      eg_extended_data_t **out_handle);

EG_API eg_electionguard_status_t eg_extended_data_free(eg_extended_data_t *handle);

EG_API eg_electionguard_status_t eg_extended_data_get_value(eg_extended_data_t *handle,
                                                            char **out_value);

EG_API uint64_t eg_extended_data_get_length(eg_extended_data_t *handle);

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

/**
 * Get the plaintext vote
 */
EG_API uint64_t eg_plaintext_ballot_selection_get_vote(eg_plaintext_ballot_selection_t *handle);

/**
 * Determines if this is a placeholder selection
 */
EG_API bool
eg_plaintext_ballot_selection_get_is_placeholder(eg_plaintext_ballot_selection_t *handle);

/**
 * an optional field of arbitrary data, such as the value of a write-in candidate
 * 
 * @param[in] handle A poiner to the `eg_plaintext_ballot_selection_t` opaque instance
 * @param[out] out_extended_data_ref A pointer to the extended data.  
                                     The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_plaintext_ballot_selection_get_extended_data(
  eg_plaintext_ballot_selection_t *handle, eg_extended_data_t **out_extended_data_ref);

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

/*
 * Get the sequence order of the selection
 */
EG_API uint64_t
eg_ciphertext_ballot_selection_get_sequence_order(eg_ciphertext_ballot_selection_t *handle);

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
 * Given an encrypted BallotSelection, generates a hash, suitable for rolling up
 * into a hash / tracking code for an entire ballot. Of note, this particular hash examines
 * the `encryptionSeed` and `message`, but not the proof.
 * This is deliberate, allowing for the possibility of ElectionGuard variants running on
 * much more limited hardware, wherein the Disjunctive Chaum-Pedersen proofs might be computed
 * later on.
 * 
 * @param[in] in_encryption_seed In most cases the encryption_seed should match the `description_hash`
 * @param[out] out_crypto_hash An opaque pointer to the nonce.  
 *                             The caller is responsible for lifecycle.
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_selection_crypto_hash_with(
  eg_ciphertext_ballot_selection_t *handle, eg_element_mod_q_t *in_encryption_seed,
  eg_element_mod_q_t **out_crypto_hash);

// TODO: add eg_ciphertext_ballot_selection_make

/**
* Given an encrypted BallotSelection, validates the encryption state against a specific seed hash and public key.
* Calling this function expects that the object is in a well-formed encrypted state
* with the elgamal encrypted `message` field populated along with
* the DisjunctiveChaumPedersenProof`proof` populated.
* the ElementModQ `description_hash` and the ElementModQ `crypto_hash` are also checked.
*
* @param[in] encryption_seed The hash of the SelectionDescription, or
*                      whatever `ElementModQ` was used to populate the `description_hash` field.
* @param[in] in_public_key The election public key
* @param[in] in_crypto_extended_base_hash The extended base hash of the election
*/
EG_API bool eg_ciphertext_ballot_selection_is_valid_encryption(
  eg_ciphertext_ballot_selection_t *handle, eg_element_mod_q_t *in_encryption_seed,
  eg_element_mod_p_t *in_public_key, eg_element_mod_q_t *in_crypto_extended_base_hash);

// CiphertextBallotSelection::crypto_hash_with not provided
// static CiphertextBallotSelection::make not provided, use eg_encrypt_selection

#endif

#ifndef PlaintextBallotContest

struct eg_plaintext_ballot_contest_s;

/**
 * A PlaintextBallotContest represents the selections made by a voter for a specific ContestDescription
 *
 * This class can be either a partial or a complete representation of a contest dataset.  Specifically,
 * a partial representation must include at a minimum the "affirmative" selections of a contest.
 * A complete representation of a ballot must include both affirmative and negative selections of
 * the contest, AND the placeholder selections necessary to satisfy the ConstantChaumPedersen proof
 * in the CiphertextBallotContest.
 *
 * Typically partial contests may be passed into Electionguard for memory constrained systems,
 * while complete contests are passed into ElectionGuard when running encryption on an existing dataset.
 */
typedef struct eg_plaintext_ballot_contest_s eg_plaintext_ballot_contest_t;

EG_API eg_electionguard_status_t eg_plaintext_ballot_contest_new(
  char *object_id, eg_plaintext_ballot_selection_t *in_selections[],
  const size_t in_selections_size, eg_plaintext_ballot_contest_t **out_handle);

EG_API eg_electionguard_status_t
eg_plaintext_ballot_contest_free(eg_plaintext_ballot_contest_t *handle);

/**
 * Get the objectId of the contest, which is the unique id for
 * the contest in a specific ballot style described in the election manifest.
 */
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

/**
 * Given a PlaintextBallotContest returns true if the state is representative of the expected values.
 * 
 * Note: because this class supports partial representations, undervotes are considered a valid state.
 */
EG_API bool eg_plaintext_ballot_contest_is_valid(eg_plaintext_ballot_contest_t *handle,
                                                 char *in_expected_object_id,
                                                 uint64_t in_expected_num_selections,
                                                 uint64_t in_expected_num_elected,
                                                 uint64_t in_votes_allowed);

#endif

#ifndef CiphertextBallotContest

struct eg_ciphertext_ballot_contest_s;

/**
 * A CiphertextBallotContest represents the selections made by a voter for a specific ContestDescription
 *
 * CiphertextBallotContest can only be a complete representation of a contest dataset.  While
 * PlaintextBallotContest supports a partial representation, a CiphertextBallotContest includes all data
 * necessary for a verifier to verify the contest.  Specifically, it includes both explicit affirmative
 * and negative selections of the contest, as well as the placeholder selections that satisfy
 * the ConstantChaumPedersen proof.
 *
 * Similar to `CiphertextBallotSelection` the consuming application can choose to discard or keep both
 * the `nonce` and the `proof` in some circumstances.  For deterministic nonce's derived from the
 * seed nonce, both values can be regenerated.  If the `nonce` for this contest is completely random,
 * then it is required in order to regenerate the proof.
 */
typedef struct eg_ciphertext_ballot_contest_s eg_ciphertext_ballot_contest_t;

// no constructors defined.  use `eg_encrypt_contest` in encrypt.h

EG_API eg_electionguard_status_t
eg_ciphertext_ballot_contest_free(eg_ciphertext_ballot_contest_t *handle);

/**
 * Get the objectId of the contest, which is the unique id for
 * the contest in a specific ballot style described in the election manifest.
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_contest_get_object_id(
  eg_ciphertext_ballot_contest_t *handle, char **out_object_id);

/**
 * Get the sequence order of the contest
 */
EG_API uint64_t
eg_ciphertext_ballot_contest_get_sequence_order(eg_ciphertext_ballot_contest_t *handle);

/**
 * The hash of the string representation of the Contest Description from the election manifest
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
 * @param[out] out_ciphertext_accumulation_ref An opaque pointer to the ElGamalCiphertext accumulation.  
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

/**
 * Given an encrypted Ballotcontest, generates a hash, suitable for rolling up
 * into a hash / tracking code for an entire ballot. Of note, this particular hash examines
 * the `encryptionSeed` and `ballot_selections`, but not the proof.
 * This is deliberate, allowing for the possibility of ElectionGuard variants running on
 * much more limited hardware, wherein the Constant Chaum-Pedersen proofs might be computed
 * later on.
 * 
 * @param[in] in_encryption_seed In most cases the encryption_seed should match the `description_hash`
 * @param[out] out_crypto_hash An opaque pointer to the nonce.  
 *                             The caller is responsible for lifecycle.
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_contest_crypto_hash_with(
  eg_ciphertext_ballot_selection_t *handle, eg_element_mod_q_t *in_encryption_seed,
  eg_element_mod_q_t **out_crypto_hash);

// TODO: eg_ciphertext_ballot_contest_make

/**
 * An aggregate nonce for the contest composed of the nonces of the selections.
 * Used when constructing the proof of selection limit.
 * 
 * Sensitive & should be treated as a secret
 * 
 * @param[out] out_nonce_ref An opaque pointer to the ElementModQ nonce.  
 *                           The caller is responsible for the object lifecycle.
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_contest_aggregate_nonce(
  eg_ciphertext_ballot_selection_t *handle, eg_element_mod_q_t **out_aggregate_nonce);

/**
 * Add the individual ballot_selections `message` fields together, suitable for use
 * when constructing the proof of selection limit.
 * 
 * @param[out] out_ciphertext_accumulation An opaque pointer to the ElGamalCiphertext accumulation.
 *                                         The caller is responsible for the object lifecycle.
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_contest_elgamal_accumulate(
  eg_ciphertext_ballot_selection_t *handle, eg_elgamal_ciphertext_t **out_ciphertext_accumulation);

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
  eg_ciphertext_ballot_contest_t *handle, eg_element_mod_q_t *in_encryption_seed,
  eg_element_mod_p_t *in_public_key, eg_element_mod_q_t *in_crypto_extended_base_hash);

#endif

#ifndef PlaintextBallot

struct eg_plaintext_ballot_s;

/**
 * A PlaintextBallot represents a voters selections for a given ballot and ballot style.
 *
 * This class can be either a partial or a complete representation of the expected values of a ballot dataset.
 * Specifically, a partial representation must include at a minimum the "affirmative" selections
 * of every contest where a selection is made.  A partial representation may exclude contests for which
 * no selection is made.
 *
 * A complete representation of a ballot must include both affirmative and negative selections of
 * every contest, AND the placeholder selections necessary to satisfy the NIZKPs for each contest and selection.
 */
typedef struct eg_plaintext_ballot_s eg_plaintext_ballot_t;

EG_API eg_electionguard_status_t eg_plaintext_ballot_new(
  char *in_object_id, char *in_style_id, eg_plaintext_ballot_contest_t *in_contests[],
  const size_t in_contests_size, eg_plaintext_ballot_t **out_handle);

EG_API eg_electionguard_status_t eg_plaintext_ballot_free(eg_plaintext_ballot_t *handle);

/**
 * A unique Ballot ID that is relevant to the external system and must be unique
 * within the dataset of the election.
 */
EG_API eg_electionguard_status_t eg_plaintext_ballot_get_object_id(eg_plaintext_ballot_t *handle,
                                                                   char **out_object_id);

/**
 * The Object Id of the ballot style in the election manifest.  This value is used
 * to determine which contests to expect on the ballot, to fill in missing values,
 * and to validate that the ballot is well-formed
 */
EG_API eg_electionguard_status_t eg_plaintext_ballot_get_style_id(eg_plaintext_ballot_t *handle,
                                                                  char **out_style_id);

/**
 * Get the size of the contest collection
 */
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

/**
 * Import the ballot representation from JSON
 */
EG_API eg_electionguard_status_t eg_plaintext_ballot_from_json(char *in_data,
                                                               eg_plaintext_ballot_t **out_handle);

/**
 * Import the ballot representation from BSON
 */
EG_API eg_electionguard_status_t eg_plaintext_ballot_from_bson(uint8_t *in_data, uint64_t in_length,
                                                               eg_plaintext_ballot_t **out_handle);

/**
 * Import the ballot representation from MsgPack
 */
EG_API eg_electionguard_status_t eg_plaintext_ballot_from_msgpack(
  uint8_t *in_data, uint64_t in_length, eg_plaintext_ballot_t **out_handle);

/**
 * Export the ballot representation as JSON
 */
EG_API eg_electionguard_status_t eg_plaintext_ballot_to_json(eg_plaintext_ballot_t *handle,
                                                             char **out_data, size_t *out_size);

/**
 * Export the ballot representation as BSON
 */
EG_API eg_electionguard_status_t eg_plaintext_ballot_to_bson(eg_plaintext_ballot_t *handle,
                                                             uint8_t **out_data, size_t *out_size);

/**
 * Export the ballot representation as MsgPack
 */
EG_API eg_electionguard_status_t eg_plaintext_ballot_to_msgpack(eg_plaintext_ballot_t *handle,
                                                                uint8_t **out_data,
                                                                size_t *out_size);

#endif

#ifndef CiphertextBallot

struct eg_ciphertext_ballot_s;

/**
 * A CiphertextBallot represents a voters encrypted selections for a given ballot and ballot style.
 *
 * When a ballot is in it's complete, encrypted state, the `nonce` is the seed nonce
 * from which all other nonces can be derived to encrypt the ballot.  Allong with the `nonce`
 * fields on `Ballotcontest` and `BallotSelection`, this value is sensitive.
 *
 * Don't make this directly. Use `make_ciphertext_ballot` instead, or call `eg_encrypt_ballot` in encrypt.h.
 */
typedef struct eg_ciphertext_ballot_s eg_ciphertext_ballot_t;

// no constructors defined.  use `eg_encrypt_ballot` in encrypt.h

EG_API eg_electionguard_status_t eg_ciphertext_ballot_free(eg_ciphertext_ballot_t *handle);

/**
 * The unique ballot id that is meaningful to the consuming application.
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_get_object_id(eg_ciphertext_ballot_t *handle,
                                                                    char **out_object_id);

/**
 * The Object Id of the ballot style in the election manifest.  This value is used
 * to determine which contests to expect on the ballot, to fill in missing values,
 * and to validate that the ballot is well-formed
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_get_style_id(eg_ciphertext_ballot_t *handle,
                                                                   char **out_style_id);

/**
 * Hash of the complete Election Manifest to which this ballot belongs
 * 
 * @param[out] out_manifest_hash_ref An opaque pointer to the description hash.  
 *                          The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_get_manifest_hash(
  eg_ciphertext_ballot_t *handle, eg_element_mod_q_t **out_manifest_hash_ref);

/**
 * The seed hash for the ballot.  It may be the encryption device hash,
 * the hash of a previous ballot or the hash of some other value
 * that is meaningful to the consuming application.
 * 
 * @param[out] out_ballot_code_seed_ref An opaque pointer to the previous tracking or seed hash.  
 *                          The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_get_ballot_code_seed(
  eg_ciphertext_ballot_t *handle, eg_element_mod_q_t **out_ballot_code_seed_ref);

/**
 * Get the size of the contests collection
 */
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

/**
 * The timestamp indicating when the ballot was encrypted
 * as measured by the encryption device.  This value does not
 * provide units as it is up to the consuming system to indicate the scale.
 * Typically a consumer may use seconds since epoch or ticks since epoch
 */
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
 * The hash of the encrypted values on this ballot in sequence order
 * 
 * @param[out] out_hash_ref An opaque pointer to the crypto hash.  
 *                          The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_get_crypto_hash(
  eg_ciphertext_ballot_t *handle, eg_element_mod_q_t **out_hash_ref);

/**
 * Given an encrypted Ballot, generates a hash, suitable for rolling up
 * into a hash for an entire ballot / ballot code. Of note, this particular hash examines
 * the `manifest_hash` and `ballot_selections`, but not the proof.
 * This is deliberate, allowing for the possibility of ElectionGuard variants running on
 * much more limited hardware, wherein the Disjunctive Chaum-Pedersen proofs might be computed
 * later on.
 * 
 * @param[in] in_manifest_hash The manifest hash
 * @param[out] out_crypto_hash An opaque pointer to the crypto hash.  
 *                          The caller is responsible for object lifecycle
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_crypto_hash_with(
  eg_ciphertext_ballot_t *handle, eg_element_mod_q_t *in_manifest_hash,
  eg_element_mod_q_t **out_crypto_hash);

// TODO: eg_ciphertext_ballot_make

EG_API bool eg_ciphertext_ballot_is_valid_encryption(
  eg_ciphertext_ballot_t *handle, eg_element_mod_q_t *in_manifest_hash,
  eg_element_mod_p_t *in_public_key, eg_element_mod_q_t *in_crypto_extended_base_hash);

/**
 * Import the ballot representation from JSON
 */
EG_API eg_electionguard_status_t
eg_ciphertext_ballot_from_json(char *in_data, eg_ciphertext_ballot_t **out_handle);

/**
 * Import the ballot representation from BSON
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_from_bson(
  uint8_t *in_data, uint64_t in_length, eg_ciphertext_ballot_t **out_handle);

/**
 * Import the ballot representation from MsgPack
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_from_msgpack(
  uint8_t *in_data, uint64_t in_length, eg_ciphertext_ballot_t **out_handle);

/**
 * Export the ballot representation as JSON
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_to_json(eg_ciphertext_ballot_t *handle,
                                                              char **out_data, uint64_t *out_size);

/**
 * Export the ballot representation as JSON with nonce values
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_to_json_with_nonces(
  eg_ciphertext_ballot_t *handle, char **out_data, uint64_t *out_size);

/**
 * Export the ballot representation as BSON
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_to_bson(eg_ciphertext_ballot_t *handle,
                                                              uint8_t **out_data,
                                                              uint64_t *out_size);

/**
 * Export the ballot representation as BSON with nonce values
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_to_bson_with_nonces(
  eg_ciphertext_ballot_t *handle, uint8_t **out_data, uint64_t *out_size);

/**
 * Export the ballot representation as MsgPack
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_to_msgpack(eg_ciphertext_ballot_t *handle,
                                                                 uint8_t **out_data,
                                                                 uint64_t *out_size);

/**
 * Export the ballot representation as MsgPack with nonce values
 */
EG_API eg_electionguard_status_t eg_ciphertext_ballot_to_msgpack_with_nonces(
  eg_ciphertext_ballot_t *handle, uint8_t **out_data, uint64_t *out_size);

#endif

#ifndef SubmittedBallot

struct eg_submitted_ballot_s;

/**
 * A `SubmittedBallot` represents a ballot that is submitted for inclusion in election results.
 * A submitted ballot is or is about to be either cast or spoiled.
 * The state supports the `BallotBoxState.UNKNOWN` enumeration to indicate that this object is mutable
 * and has not yet been explicitly assigned a specific state.
 *
 * Note, additionally, this ballot includes all proofs but no nonces.
 *
 * Do not make this class directly. Use `eg_submitted_ballot_from` instead.
 */
typedef struct eg_submitted_ballot_s eg_submitted_ballot_t;

// no constructors defined.  use `eg_submitted_ballot_from` in encrypt.h

EG_API eg_electionguard_status_t eg_submitted_ballot_free(eg_submitted_ballot_t *handle);

/**
 * The state of the ballot (cast, spoiled, or unknown)
 */
EG_API eg_ballot_box_state_t eg_submitted_ballot_get_state(eg_submitted_ballot_t *handle);

/**
 * Convert a `CiphertextBallot` into a `SubmittedBallot`, with all nonces removed.
 * 
 * @param[in] in_ciphertext: the ciphertext ballot
 * @param[in] in_state: The ballot box state (cast, spoiled, unknown)
 * @param[out] out_handle a handle to an `eg_submitted_ballot_t` instance.  Caller is responsible for lifecycle.
 */
EG_API eg_electionguard_status_t eg_submitted_ballot_from(eg_ciphertext_ballot_t *in_ciphertext,
                                                          eg_ballot_box_state_t in_state,
                                                          eg_submitted_ballot_t **out_handle);

/**
 * Import the ballot representation from JSON
 */
EG_API eg_electionguard_status_t eg_submitted_ballot_from_json(char *in_data,
                                                               eg_submitted_ballot_t **out_handle);

/**
 * Import the ballot representation from BSON
 */
EG_API eg_electionguard_status_t eg_submitted_ballot_from_bson(uint8_t *in_data, uint64_t in_length,
                                                               eg_submitted_ballot_t **out_handle);

/**
 * Import the ballot representation from MsgPack
 */
EG_API eg_electionguard_status_t eg_submitted_ballot_from_msgpack(
  uint8_t *in_data, uint64_t in_length, eg_submitted_ballot_t **out_handle);

/**
 * Export the ballot representation as JSON
 */
EG_API eg_electionguard_status_t eg_submitted_ballot_to_json(eg_submitted_ballot_t *handle,
                                                             char **out_data, uint64_t *out_size);

/**
 * Export the ballot representation as BSON
 */
EG_API eg_electionguard_status_t eg_submitted_ballot_to_bson(eg_submitted_ballot_t *handle,
                                                             uint8_t **out_data,
                                                             uint64_t *out_size);

/**
 * Export the ballot representation as MsgPack
 */
EG_API eg_electionguard_status_t eg_submitted_ballot_to_msgpack(eg_submitted_ballot_t *handle,
                                                                uint8_t **out_data,
                                                                uint64_t *out_size);

#endif

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CPP_BALLOT_H_INCLUDED__ */
