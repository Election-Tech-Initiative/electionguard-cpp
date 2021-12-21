/// @file election.h
#ifndef __ELECTIONGUARD_CPP_ELECTION_H_INCLUDED__
#define __ELECTIONGUARD_CPP_ELECTION_H_INCLUDED__

#include "collections.h"
#include "export.h"
#include "group.h"
#include "status.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
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
EG_API eg_electionguard_status_t eg_ciphertext_election_context_get_manifest_hash(
  eg_ciphertext_election_context_t *handle, eg_element_mod_q_t **out_manifest_hash_ref);

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
 * The `extended base hash code (𝑄')` in the [ElectionGuard Spec](https://github.com/microsoft/electionguard/wiki)
 * 
 * @param[out] out_extended_data A linked list of the extended data.  
 *                               The caller is responsible for freeing the linked list
 */
EG_API eg_electionguard_status_t eg_ciphertext_election_context_get_extended_data(
  eg_ciphertext_election_context_t *handle, eg_linked_list_t **out_extended_data);

/**
 * Makes a CiphertextElectionContext object.
 * 
 * @param[in] in_number_of_guardians The number of guardians necessary to generate the public key
 * @param[in] in_quorum The quorum of guardians necessary to decrypt an election.  Must be less than `number_of_guardians`
 * @param[in] in_elgamal_public_key the public key of the election
 * @param[in] in_commitment_hash the hash of the commitments the guardians make to each other
 * @param[in] in_manifest_hash the hash of the election metadata
 * @param[out] out_handle An opaque pointer to the object.  Caller is responsible for lifecycle.
 */
EG_API eg_electionguard_status_t eg_ciphertext_election_context_make(
  uint64_t in_number_of_guardians, uint64_t in_quorum, eg_element_mod_p_t *in_elgamal_public_key,
  eg_element_mod_q_t *in_commitment_hash, eg_element_mod_q_t *in_manifest_hash,
  eg_ciphertext_election_context_t **out_handle);

/**
 * Makes a CiphertextElectionContext object.
 * 
 * @param[in] in_number_of_guardians The number of guardians necessary to generate the public key
 * @param[in] in_quorum The quorum of guardians necessary to decrypt an election.  Must be less than `number_of_guardians`
 * @param[in] in_elgamal_public_key the public key of the election
 * @param[in] in_commitment_hash the hash of the commitments the guardians make to each other
 * @param[in] in_manifest_hash the hash of the election metadata
 * @param[in] in_extended_data the extended data
 * @param[out] out_handle An opaque pointer to the object.  Caller is responsible for lifecycle.
 */
EG_API eg_electionguard_status_t eg_ciphertext_election_context_make_with_extended_data(
  uint64_t in_number_of_guardians, uint64_t in_quorum, eg_element_mod_p_t *in_elgamal_public_key,
  eg_element_mod_q_t *in_commitment_hash, eg_element_mod_q_t *in_manifest_hash,
  eg_linked_list_t *in_extended_data, eg_ciphertext_election_context_t **out_handle);

/**
 * Makes a CiphertextElectionContext object from the hex string representations.
 * 
 * @param[in] in_number_of_guardians The number of guardians necessary to generate the public key
 * @param[in] in_quorum The quorum of guardians necessary to decrypt an election.  Must be less than `number_of_guardians`
 * @param[in] in_elgamal_public_key the public key of the election
 * @param[in] in_commitment_hash the hash of the commitments the guardians make to each other
 * @param[in] in_manifest_hash the hash of the election metadata
 * @param[out] out_handle An opaque pointer to the object.  Caller is responsible for lifecycle.
 */
EG_API eg_electionguard_status_t eg_ciphertext_election_context_make_from_hex(
  uint64_t in_number_of_guardians, uint64_t in_quorum, const char *in_elgamal_public_key,
  const char *in_commitment_hash, const char *in_manifest_hash,
  eg_ciphertext_election_context_t **out_handle);

/**
 * Makes a CiphertextElectionContext object from the hex string representations.
 * 
 * @param[in] in_number_of_guardians The number of guardians necessary to generate the public key
 * @param[in] in_quorum The quorum of guardians necessary to decrypt an election.  Must be less than `number_of_guardians`
 * @param[in] in_elgamal_public_key the public key of the election
 * @param[in] in_commitment_hash the hash of the commitments the guardians make to each other
 * @param[in] in_manifest_hash the hash of the election metadata
 * @param[in] in_extended_data the extended data
 * @param[out] out_handle An opaque pointer to the object.  Caller is responsible for lifecycle.
 */
EG_API eg_electionguard_status_t eg_ciphertext_election_context_make_from_hex_with_extended_data(
  uint64_t in_number_of_guardians, uint64_t in_quorum, const char *in_elgamal_public_key,
  const char *in_commitment_hash, const char *in_manifest_hash, eg_linked_list_t *in_extended_data,
  eg_ciphertext_election_context_t **out_handle);

EG_API eg_electionguard_status_t eg_ciphertext_election_context_from_json(
  char *in_data, eg_ciphertext_election_context_t **out_handle);

EG_API eg_electionguard_status_t eg_ciphertext_election_context_from_bson(
  uint8_t *in_data, uint64_t in_length, eg_ciphertext_election_context_t **out_handle);

EG_API eg_electionguard_status_t eg_ciphertext_election_context_to_json(
  eg_ciphertext_election_context_t *handle, char **out_data, uint64_t *out_size);

EG_API eg_electionguard_status_t eg_ciphertext_election_context_to_bson(
  eg_ciphertext_election_context_t *handle, uint8_t **out_data, uint64_t *out_size);

#endif

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CPP_ELECTION_H_INCLUDED__ */
