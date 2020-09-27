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

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CORE_ELECTION_H_INCLUDED__ */