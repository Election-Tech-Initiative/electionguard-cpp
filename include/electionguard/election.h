#ifndef __ELECTIONGUARD_CORE_ELECTION_H_INCLUDED__
#define __ELECTIONGUARD_CORE_ELECTION_H_INCLUDED__

#include "export.h"
#include "group.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// SelectionDescription

struct eg_selection_description_s;
typedef struct eg_selection_description_s eg_selection_description_t;

EG_API eg_selection_description_t *eg_selection_description_create(const char *object_id,
                                                                   const char *candidate_id,
                                                                   uint64_t sequence_order);

EG_API const char *eg_selection_description_get_object_id(eg_selection_description_t *selection);
EG_API const char *eg_selection_description_get_candidate_id(eg_selection_description_t *selection);
EG_API uint64_t eg_selection_description_get_sequence_order(eg_selection_description_t *selection);

EG_API eg_element_mod_q_t *
eg_selection_description_crypto_hash(eg_selection_description_t *selection);

// ConstestDescription

struct eg_contest_description_s;
typedef struct eg_contest_description_s eg_contest_description_t;

// TODO: implement

// InternalElectionDescription

struct eg_internal_election_description_s;
typedef struct eg_internal_election_description_s eg_internal_election_description_t;

eg_internal_election_description_t *eg_internal_election_description_from_json(char *data);
eg_internal_election_description_t *eg_internal_election_description_from_bson(uint8_t *data,
                                                                               uint64_t length);

// returns the size and fills out_data, caller is responsible for freeing the out_data
uint64_t eg_internal_election_description_to_json(eg_internal_election_description_t *metadata,
                                                  char **out_data);

// returns the size and fills out_data, caller is responsible for freeing the out_data
uint64_t eg_internal_election_description_to_bson(eg_internal_election_description_t *metadata,
                                                  uint8_t **out_data);

// TODO: implement

// CiphertextElectionContext

struct eg_ciphertext_election_context_s;
typedef struct eg_ciphertext_election_context_s eg_ciphertext_election_context_t;

EG_API eg_ciphertext_election_context_t *
eg_ciphertext_election_context_make(uint64_t number_of_guardians, uint64_t quorum,
                                    eg_element_mod_p_t *elgamal_public_key,
                                    eg_element_mod_q_t *description_hash);

EG_API eg_ciphertext_election_context_t *
eg_ciphertext_election_context_make_from_hex(uint64_t number_of_guardians, uint64_t quorum,
                                             const char *elgamal_public_key,
                                             const char *description_hash);

eg_ciphertext_election_context_t *eg_ciphertext_election_context_from_json(char *data);
eg_ciphertext_election_context_t *eg_ciphertext_election_context_from_bson(uint8_t *data,
                                                                           uint64_t length);

// returns the size and fills out_data, caller is responsible for freeing the out_data
uint64_t eg_ciphertext_election_context_to_json(eg_ciphertext_election_context_t *context,
                                                char **out_data);

// returns the size and fills out_data, caller is responsible for freeing the out_data
uint64_t eg_ciphertext_election_context_to_bson(eg_ciphertext_election_context_t *context,
                                                uint8_t **out_data);

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CORE_ELECTION_H_INCLUDED__ */