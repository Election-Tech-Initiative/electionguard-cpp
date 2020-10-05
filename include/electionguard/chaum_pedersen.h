#ifndef __ELECTIONGUARD_CHAUM_PEDERSEN_H_INCLUDED__
#define __ELECTIONGUARD_CHAUM_PEDERSEN_H_INCLUDED__

#include "elgamal.h"
#include "export.h"
#include "group.h"
#include "status.h"

#ifdef __cplusplus
extern "C" {
#endif

// DisjunctiveChaumPedersenProof

struct eg_disjunctive_chaum_pedersen_proof_s;
typedef struct eg_disjunctive_chaum_pedersen_proof_s eg_disjunctive_chaum_pedersen_proof_t;

// No constructor provided.  Use eg_encrypt_selection`

EG_API bool
eg_disjunctive_chaum_pedersen_proof_is_valid(eg_disjunctive_chaum_pedersen_proof_t *handle,
                                             eg_elgamal_ciphertext_t *in_ciphertext,
                                             eg_element_mod_p_t *in_k, eg_element_mod_q_t *in_q);

// ConstantChaumPedersenProof

struct eg_constant_chaum_pedersen_proof_s;
typedef struct eg_constant_chaum_pedersen_proof_s eg_constant_chaum_pedersen_proof_t;

// No constructor provided.  Use `eg_constant_chaum_pedersen_make
EG_API eg_electionguard_status_t
eg_constant_chaum_pedersen_proof_free(eg_constant_chaum_pedersen_proof_t *handle);

EG_API bool eg_constant_chaum_pedersen_proof_is_valid(eg_constant_chaum_pedersen_proof_t *handle,
                                                      eg_elgamal_ciphertext_t *in_ciphertext,
                                                      eg_element_mod_p_t *in_k,
                                                      eg_element_mod_q_t *in_q);

EG_API eg_electionguard_status_t eg_constant_chaum_pedersen_proof_make(
  eg_elgamal_ciphertext_t *in_ciphertext, eg_element_mod_q_t *in_r, eg_element_mod_p_t *in_k,
  eg_element_mod_q_t *in_seed, eg_element_mod_q_t *in_hash_header, uint64_t in_constant,
  eg_constant_chaum_pedersen_proof_t **out_handle);

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CHAUM_PEDERSEN_H_INCLUDED__ */
