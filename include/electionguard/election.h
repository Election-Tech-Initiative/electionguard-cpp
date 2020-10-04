#ifndef __ELECTIONGUARD_CORE_ELECTION_H_INCLUDED__
#define __ELECTIONGUARD_CORE_ELECTION_H_INCLUDED__

#include "export.h"
#include "group.h"
#include "status.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// SelectionDescription

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

// ConstestDescription

struct eg_contest_description_s;
typedef struct eg_contest_description_s eg_contest_description_t;

// TODO: implement

// InternalElectionDescription

struct eg_internal_election_description_s;
typedef struct eg_internal_election_description_s eg_internal_election_description_t;

EG_API eg_electionguard_status_t
eg_internal_election_description_free(eg_internal_election_description_t *handle);

EG_API eg_electionguard_status_t eg_internal_election_description_get_description_hash(
  eg_internal_election_description_t *handle, eg_element_mod_q_t **out_description_hash);

EG_API eg_electionguard_status_t eg_internal_election_description_from_json(
  char *in_data, eg_internal_election_description_t **out_handle);

EG_API eg_electionguard_status_t eg_internal_election_description_from_bson(
  uint8_t *in_data, uint64_t in_length, eg_internal_election_description_t **out_handle);

EG_API eg_electionguard_status_t eg_internal_election_description_to_json(
  eg_internal_election_description_t *handle, char **out_data, size_t *out_size);

// returns the size and fills out_data, caller is responsible for freeing the out_data
EG_API eg_electionguard_status_t eg_internal_election_description_to_bson(
  eg_internal_election_description_t *handle, uint8_t **out_data, size_t *out_size);

// TODO: implement

// CiphertextElectionContext

struct eg_ciphertext_election_context_s;
typedef struct eg_ciphertext_election_context_s eg_ciphertext_election_context_t;

EG_API eg_electionguard_status_t
eg_ciphertext_election_context_free(eg_ciphertext_election_context_t *handle);

EG_API eg_electionguard_status_t eg_ciphertext_election_context_get_elgamal_public_key(
  eg_ciphertext_election_context_t *handle, eg_element_mod_p_t **out_elgamal_public_key);

EG_API eg_electionguard_status_t eg_ciphertext_election_context_get_description_hash(
  eg_ciphertext_election_context_t *handle, eg_element_mod_q_t **out_description_hash);

EG_API eg_electionguard_status_t eg_ciphertext_election_context_get_crypto_base_hash(
  eg_ciphertext_election_context_t *handle, eg_element_mod_q_t **out_crypto_base_hash);

EG_API eg_electionguard_status_t eg_ciphertext_election_context_get_crypto_extended_base_hash(
  eg_ciphertext_election_context_t *handle, eg_element_mod_q_t **out_crypto_extended_base_hash);

EG_API eg_electionguard_status_t eg_ciphertext_election_context_make(
  uint64_t in_number_of_guardians, uint64_t in_quorum, eg_element_mod_p_t *in_elgamal_public_key,
  eg_element_mod_q_t *in_description_hash, eg_ciphertext_election_context_t **out_handle);

EG_API eg_electionguard_status_t eg_ciphertext_election_context_make_from_hex(
  uint64_t in_number_of_guardians, uint64_t in_quorum, const char *in_elgamal_public_key,
  const char *in_description_hash, eg_ciphertext_election_context_t **out_handle);

EG_API eg_electionguard_status_t eg_ciphertext_election_context_from_json(
  char *in_data, eg_ciphertext_election_context_t **out_handle);

EG_API eg_electionguard_status_t eg_ciphertext_election_context_from_bson(
  uint8_t *in_data, uint64_t in_length, eg_ciphertext_election_context_t **out_handle);

EG_API eg_electionguard_status_t eg_ciphertext_election_context_to_json(
  eg_ciphertext_election_context_t *handle, char **out_data, size_t *out_size);

EG_API eg_electionguard_status_t eg_ciphertext_election_context_to_bson(
  eg_ciphertext_election_context_t *handle, uint8_t **out_data, size_t *out_size);

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CORE_ELECTION_H_INCLUDED__ */
