#ifndef __ELECTIONGUARD_CORE_BALLOT_H_INCLUDED__
#define __ELECTIONGUARD_CORE_BALLOT_H_INCLUDED__

#include "export.h"
#include "group.h"

#ifdef __cplusplus
extern "C" {
#endif

// PlaintextBallotSelection
struct eg_plaintext_ballot_selection_s;
typedef struct eg_plaintext_ballot_selection_s eg_plaintext_ballot_selection_t;

EG_API eg_plaintext_ballot_selection_t *eg_plaintext_ballot_selection_new(const char *object_id,
                                                                          const char *vote);
EG_API void eg_plaintext_ballot_selection_free(eg_plaintext_ballot_selection_t *selection);

EG_API char *
eg_plaintext_ballot_selection_get_object_id(eg_plaintext_ballot_selection_t *selection);

// CiphertextBallotSelection
struct eg_ciphertext_ballot_selection_s;
typedef struct eg_ciphertext_ballot_selection_s eg_ciphertext_ballot_selection_t;

EG_API eg_ciphertext_ballot_selection_t *
eg_ciphertext_ballot_selection_new(const char *object_id, eg_element_mod_q_t *description_hash);
EG_API void eg_ciphertext_ballot_selection_free(eg_ciphertext_ballot_selection_t *selection);

EG_API char *
eg_ciphertext_ballot_selection_get_object_id(eg_ciphertext_ballot_selection_t *selection);
EG_API eg_element_mod_q_t *
eg_ciphertext_ballot_selection_get_description_hash(eg_ciphertext_ballot_selection_t *selection);

// TODO: add other fields

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CORE_BALLOT_H_INCLUDED__ */