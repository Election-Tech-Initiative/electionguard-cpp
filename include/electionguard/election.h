/// @file election.h
#ifndef __ELECTIONGUARD_CPP_ELECTION_H_INCLUDED__
#define __ELECTIONGUARD_CPP_ELECTION_H_INCLUDED__

#include "export.h"
#include "group.h"
#include "status.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SelectionDescription

struct eg_selection_description_s;
typedef struct eg_selection_description_s eg_selection_description_t;

EG_API eg_electionguard_status_t
eg_selection_description_new(const char *in_object_id, const char *in_candidate_id,
                             uint64_t in_sequence_order, eg_selection_description_t **out_handle);

EG_API eg_electionguard_status_t eg_selection_description_free(eg_selection_description_t *handle);

EG_API eg_electionguard_status_t eg_selection_description_get_object_id(
  eg_selection_description_t *handle, const char **out_object_id);

EG_API eg_electionguard_status_t eg_selection_description_get_candidate_id(
  eg_selection_description_t *handle, const char **out_candidate_id);

EG_API eg_electionguard_status_t eg_selection_description_get_sequence_order(
  eg_selection_description_t *handle, uint64_t *out_sequence_order);

EG_API eg_electionguard_status_t eg_selection_description_crypto_hash(
  eg_selection_description_t *handle, eg_element_mod_q_t **out_crypto_hash);

#endif

#ifndef ConstestDescription

struct eg_contest_description_s;
typedef struct eg_contest_description_s eg_contest_description_t;

// TODO: ISSUE #129: implement

#endif

#ifndef ElectionDescription

struct eg_election_description_s;

/**
* Use this entity for defining the structure of the election and associated
* information such as candidates, contests, and vote counts.  This class is
* based on the NIST Election Common Standard Data Specification.  Some deviations
* from the standard exist.
*
* This structure is considered an immutable input object and should not be changed
* through the course of an election, as it's hash representation is the basis for all
* other hash representations within an ElectionGuard election context.
*
* @see https://developers.google.com/elections-data/reference/election
*/
typedef struct eg_election_description_s eg_election_description_t;

// no constructors defined.  use `eg_election_description_from_json` or `eg_election_description_from_bson`

EG_API eg_electionguard_status_t eg_election_description_free(eg_election_description_t *handle);

EG_API eg_electionguard_status_t eg_election_description_get_election_scope_id(
  eg_election_description_t *handle, char **out_election_scope_id);

// TODO: ISSUE #129: implement missing members

/**
 * Get a hash of the metadata components of the `ElectionDescription`
 * 
 * @param[in] handle the `eg_election_description_t`
 * @param[out] out_owned_hash the `eg_element_mod_q_t` crypto hash.  
 *                            Caller is responsible fo the lifecycle and 
 *                            must call `eg_element_mod_q_free`
 */
EG_API eg_electionguard_status_t eg_election_description_crypto_hash(
  eg_election_description_t *handle, eg_element_mod_q_t **out_owned_hash);

EG_API eg_electionguard_status_t
eg_election_description_from_json(char *in_data, eg_election_description_t **out_handle);

EG_API eg_electionguard_status_t eg_election_description_from_bson(
  uint8_t *in_data, uint64_t in_length, eg_election_description_t **out_handle);

EG_API eg_electionguard_status_t eg_election_description_to_json(eg_election_description_t *handle,
                                                                 char **out_data, size_t *out_size);

// returns the size and fills out_data, caller is responsible for freeing the out_data
EG_API eg_electionguard_status_t eg_election_description_to_bson(eg_election_description_t *handle,
                                                                 uint8_t **out_data,
                                                                 size_t *out_size);

#endif

#ifndef InternalElectionDescription

struct eg_internal_election_description_s;

/**
* `InternalElectionDescription` is a subset of the `ElectionDescription` structure that specifies
* the components that ElectionGuard uses for conducting an election.  The key component is the
* `contests` collection, which applies placeholder selections to the `ElectionDescription` contests
*/
typedef struct eg_internal_election_description_s eg_internal_election_description_t;

/**
 * Constructs an `InternalElectionDescription` by passing the `ElectionDescription` to the
 * constructor
 * 
 * @param[in] in_election_description the `eg_election_description_t`
 * @param[out] out_handle An opaque pointer to the object.  Caller is responsible for lifecycle.
 */
EG_API eg_electionguard_status_t
eg_internal_election_description_new(eg_election_description_t *in_election_description,
                                     eg_internal_election_description_t **out_handle);

EG_API eg_electionguard_status_t
eg_internal_election_description_free(eg_internal_election_description_t *handle);

/**
 * The hash of the election metadata
 * 
 * @param[out] out_description_hash_ref An opaque pointer to the description hash.  
 *                                      The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_internal_election_description_get_description_hash(
  eg_internal_election_description_t *handle, eg_element_mod_q_t **out_description_hash_ref);

// TODO: ISSUE #129: implement missing members

EG_API eg_electionguard_status_t eg_internal_election_description_from_json(
  char *in_data, eg_internal_election_description_t **out_handle);

EG_API eg_electionguard_status_t eg_internal_election_description_from_bson(
  uint8_t *in_data, uint64_t in_length, eg_internal_election_description_t **out_handle);

EG_API eg_electionguard_status_t eg_internal_election_description_to_json(
  eg_internal_election_description_t *handle, char **out_data, size_t *out_size);

// returns the size and fills out_data, caller is responsible for freeing the out_data
EG_API eg_electionguard_status_t eg_internal_election_description_to_bson(
  eg_internal_election_description_t *handle, uint8_t **out_data, size_t *out_size);

#endif

#ifndef CiphertextElectionContext

struct eg_ciphertext_election_context_s;
typedef struct eg_ciphertext_election_context_s eg_ciphertext_election_context_t;

EG_API eg_electionguard_status_t
eg_ciphertext_election_context_free(eg_ciphertext_election_context_t *handle);

/**
 * The `joint public key (K)` in the [ElectionGuard Spec](https://github.com/microsoft/electionguard/wiki)
 * 
 * @param[out] out_elgamal_public_key_ref An opaque pointer to the public key.  
 *                                        The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_ciphertext_election_context_get_elgamal_public_key(
  eg_ciphertext_election_context_t *handle, eg_element_mod_p_t **out_elgamal_public_key_ref);

/**
 * The `commitment hash H(K 1,0 , K 2,0 ... , K n,0 )` of the public commitments
 * guardians make to each other in the [ElectionGuard Spec](https://github.com/microsoft/electionguard/wiki)
 * 
 * @param[out] out_commitment_hash_ref An opaque pointer to the commitment hash.  
 *                                     The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_ciphertext_election_context_get_commitment_hash(
  eg_ciphertext_election_context_t *handle, eg_element_mod_q_t **out_commitment_hash_ref);

/**
 * The hash of the election metadata
 * 
 * @param[out] out_description_hash_ref An opaque pointer to the description hash.  
 *                                      The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_ciphertext_election_context_get_description_hash(
  eg_ciphertext_election_context_t *handle, eg_element_mod_q_t **out_description_hash_ref);

/**
 * The `base hash code (𝑄)` in the [ElectionGuard Spec](https://github.com/microsoft/electionguard/wiki)
 * 
 * @param[out] out_crypto_base_hash_ref An opaque pointer to the ciphertext.  
 *                                      The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_ciphertext_election_context_get_crypto_base_hash(
  eg_ciphertext_election_context_t *handle, eg_element_mod_q_t **out_crypto_base_hash_ref);

/**
 * The `extended base hash code (𝑄')` in the [ElectionGuard Spec](https://github.com/microsoft/electionguard/wiki)
 * 
 * @param[out] out_crypto_extended_base_hash_ref An opaque pointer to the ciphertext.  
 *                                               The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_ciphertext_election_context_get_crypto_extended_base_hash(
  eg_ciphertext_election_context_t *handle, eg_element_mod_q_t **out_crypto_extended_base_hash_ref);

/**
 * Makes a CiphertextElectionContext object.
 * 
 * @param[in] in_number_of_guardians The number of guardians necessary to generate the public key
 * @param[in] in_quorum The quorum of guardians necessary to decrypt an election.  Must be less than `number_of_guardians`
 * @param[in] in_elgamal_public_key the public key of the election
 * @param[in] in_commitment_hash the hash of the commitments the guardians make to each other
 * @param[in] in_description_hash the hash of the election metadata
 * @param[out] out_handle An opaque pointer to the object.  Caller is responsible for lifecycle.
 */
EG_API eg_electionguard_status_t eg_ciphertext_election_context_make(
  uint64_t in_number_of_guardians, uint64_t in_quorum, eg_element_mod_p_t *in_elgamal_public_key,
  eg_element_mod_q_t *in_commitment_hash, eg_element_mod_q_t *in_description_hash,
  eg_ciphertext_election_context_t **out_handle);

/**
 * Makes a CiphertextElectionContext object from the hex string representations.
 * 
 * @param[in] in_number_of_guardians The number of guardians necessary to generate the public key
 * @param[in] in_quorum The quorum of guardians necessary to decrypt an election.  Must be less than `number_of_guardians`
 * @param[in] in_elgamal_public_key the public key of the election
 * @param[in] in_commitment_hash the hash of the commitments the guardians make to each other
 * @param[in] in_description_hash the hash of the election metadata
 * @param[out] out_handle An opaque pointer to the object.  Caller is responsible for lifecycle.
 */
EG_API eg_electionguard_status_t eg_ciphertext_election_context_make_from_hex(
  uint64_t in_number_of_guardians, uint64_t in_quorum, const char *in_elgamal_public_key,
  const char *in_commitment_hash, const char *in_description_hash,
  eg_ciphertext_election_context_t **out_handle);

EG_API eg_electionguard_status_t eg_ciphertext_election_context_from_json(
  char *in_data, eg_ciphertext_election_context_t **out_handle);

EG_API eg_electionguard_status_t eg_ciphertext_election_context_from_bson(
  uint8_t *in_data, uint64_t in_length, eg_ciphertext_election_context_t **out_handle);

EG_API eg_electionguard_status_t eg_ciphertext_election_context_to_json(
  eg_ciphertext_election_context_t *handle, char **out_data, size_t *out_size);

EG_API eg_electionguard_status_t eg_ciphertext_election_context_to_bson(
  eg_ciphertext_election_context_t *handle, uint8_t **out_data, size_t *out_size);

#endif

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CPP_ELECTION_H_INCLUDED__ */
