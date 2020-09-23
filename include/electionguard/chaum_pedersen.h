#ifndef __ELECTIONGUARD_CHAUM_PEDERSEN_H_INCLUDED__
#define __ELECTIONGUARD_CHAUM_PEDERSEN_H_INCLUDED__

#include "elgamal.h"
#include "export.h"
#include "group.h"

#ifdef __cplusplus
extern "C" {
#endif

// DisjunctiveChaumPedersenProof

struct eg_disjunctive_chaum_pedersen_proof_s;
typedef struct eg_disjunctive_chaum_pedersen_proof_s eg_disjunctive_chaum_pedersen_proof_t;

bool eg_disjunctive_chaum_pedersen_proof_is_valid(eg_disjunctive_chaum_pedersen_proof_t *proof,
                                                  eg_elgamal_ciphertext_t *message,
                                                  eg_element_mod_p_t *k, eg_element_mod_q_t *q);

// ConstantChaumPedersenProof

struct eg_constant_chaum_pedersen_proof_s;
typedef struct eg_constant_chaum_pedersen_proof_s eg_constant_chaum_pedersen_proof_t;

EG_API eg_constant_chaum_pedersen_proof_t *
eg_constant_chaum_pedersen_proof_new(eg_element_mod_p_t *pad, eg_element_mod_p_t *data,
                                     eg_element_mod_q_t *challenge, eg_element_mod_q_t *response,
                                     uint64_t constant);
EG_API void eg_constant_chaum_pedersen_proof_free(eg_constant_chaum_pedersen_proof_t *proof);

bool eg_constant_chaum_pedersen_proof_is_valid(eg_constant_chaum_pedersen_proof_t *proof,
                                               eg_elgamal_ciphertext_t *message,
                                               eg_element_mod_p_t *k, eg_element_mod_q_t *q);

eg_constant_chaum_pedersen_proof_t *
eg_constant_chaum_pedersen_proof_make(eg_elgamal_ciphertext_t *message, eg_element_mod_q_t *r,
                                      eg_element_mod_p_t *k, eg_element_mod_q_t *seed,
                                      eg_element_mod_q_t *hash_header, uint64_t constant);

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CHAUM_PEDERSEN_H_INCLUDED__ */
