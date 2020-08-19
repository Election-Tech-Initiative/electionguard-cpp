#ifndef __ELECTIONGUARD_CORE_ENCRYPT_H_INCLUDED__
#define __ELECTIONGUARD_CORE_ENCRYPT_H_INCLUDED__

#include "ballot.h"
#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

struct eg_encryption_mediator_s;
typedef struct eg_encryption_mediator_s eg_encryption_mediator_t;

EG_API eg_encryption_mediator_t *eg_encryption_mediator_new();
EG_API void eg_encryption_mediator_free(eg_encryption_mediator_t *mediator);
EG_API int eg_encryption_mediator_encrypt(eg_encryption_mediator_t *mediator);

EG_API int eg_encrypt_selection(eg_plaintext_ballot_selection_t *plaintext,
                                eg_ciphertext_ballot_selection_t *ciphertext);

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CORE_ENCRYPT_H_INCLUDED__ */
