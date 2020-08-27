#ifndef __ELECTIONGUARD_CORE_ENCRYPT_H_INCLUDED__
#define __ELECTIONGUARD_CORE_ENCRYPT_H_INCLUDED__

#include "ballot.h"
#include "election.h"
#include "export.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct eg_encryption_mediator_s;
typedef struct eg_encryption_mediator_s eg_encryption_mediator_t;

EG_API eg_encryption_mediator_t *eg_encryption_mediator_new();
EG_API void eg_encryption_mediator_free(eg_encryption_mediator_t *mediator);
EG_API int eg_encryption_mediator_encrypt(eg_encryption_mediator_t *mediator);

/// returns a stubbed success code and places the result in the ciphertext field
EG_API eg_ciphertext_ballot_selection_t *
eg_encrypt_selection(eg_plaintext_ballot_selection_t *plaintext,
                     eg_selection_description_t *description, eg_element_mod_p_t *public_key,
                     eg_element_mod_q_t *crypto_extended_base_hash, eg_element_mod_q_t *nonce_seed,
                     bool is_placeholder, bool should_verify_proofs);

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CORE_ENCRYPT_H_INCLUDED__ */
