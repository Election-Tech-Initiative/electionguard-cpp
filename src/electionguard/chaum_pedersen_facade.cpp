#include "electionguard/chaum_pedersen.hpp"
#include "electionguard/group.hpp"
#include "variant_cast.hpp"

extern "C" {
#include "electionguard/chaum_pedersen.h"
}

using namespace electionguard;

// DisjunctiveChaumPedersenProof
EG_API eg_disjunctive_chaum_pedersen_proof_t *eg_disjunctive_chaum_pedersen_proof_new(
  eg_element_mod_p_t *proof_zero_pad, eg_element_mod_p_t *proof_zero_data,
  eg_element_mod_p_t *proof_one_pad, eg_element_mod_p_t *proof_one_data,
  eg_element_mod_q_t *proof_zero_challenge, eg_element_mod_q_t *proof_one_challenge,
  eg_element_mod_q_t *challenge, eg_element_mod_q_t *proof_zero_response,
  eg_element_mod_q_t *proof_one_response)
{
    auto *proof_zero_pad_ = AS_TYPE(ElementModP, proof_zero_pad);
    auto *proof_zero_data_ = AS_TYPE(ElementModP, proof_zero_data);
    auto *proof_one_pad_ = AS_TYPE(ElementModP, proof_one_pad);
    auto *proof_one_data_ = AS_TYPE(ElementModP, proof_one_data);
    auto *proof_zero_challenge_ = AS_TYPE(ElementModQ, proof_zero_challenge);
    auto *proof_one_challenge_ = AS_TYPE(ElementModQ, proof_one_challenge);
    auto *challenge_ = AS_TYPE(ElementModQ, challenge);
    auto *proof_zero_response_ = AS_TYPE(ElementModQ, proof_zero_response);
    auto *proof_one_response_ = AS_TYPE(ElementModQ, proof_one_response);
    return AS_TYPE(eg_disjunctive_chaum_pedersen_proof_t,
                   new DisjunctiveChaumPedersenProof(
                     proof_zero_pad_, proof_zero_data_, proof_one_pad_, proof_one_data_,
                     proof_zero_challenge_, proof_one_challenge_, challenge_, proof_zero_response_,
                     proof_one_response_));
}
EG_API void eg_disjunctive_chaum_pedersen_proof_free(eg_disjunctive_chaum_pedersen_proof_t *proof)
{
    if (proof == nullptr) {
        return;
    }
    // TODO: safety
    delete AS_TYPE(DisjunctiveChaumPedersenProof, proof);
}

bool eg_disjunctive_chaum_pedersen_proof_is_valid(eg_disjunctive_chaum_pedersen_proof_t *proof,
                                                  eg_elgamal_ciphertext_t *message,
                                                  eg_element_mod_p_t *k, eg_element_mod_q_t *q)
{
    auto *message_ = AS_TYPE(ElGamalCiphertext, message);
    auto *k_ = AS_TYPE(ElementModP, k);
    auto *q_ = AS_TYPE(ElementModQ, q);
    return AS_TYPE(DisjunctiveChaumPedersenProof, proof)->isValid(message_, k_, q_);
}

eg_disjunctive_chaum_pedersen_proof_t *
eg_disjunctive_chaum_pedersen_proof_make(eg_elgamal_ciphertext_t *message, eg_element_mod_q_t *r,
                                         eg_element_mod_p_t *k, eg_element_mod_q_t *q,
                                         eg_element_mod_q_t *seed, uint64_t plaintext)
{
    auto *message_ = AS_TYPE(ElGamalCiphertext, message);
    auto *r_ = AS_TYPE(ElementModQ, r);
    auto *k_ = AS_TYPE(ElementModP, k);
    auto *q_ = AS_TYPE(ElementModQ, q);
    auto *seed_ = AS_TYPE(ElementModQ, seed);
    return AS_TYPE(eg_disjunctive_chaum_pedersen_proof_t,
                   DisjunctiveChaumPedersenProof::make(message_, r_, k_, q_, seed_, plaintext));
}

// ConstantChaumPedersenProof
EG_API eg_constant_chaum_pedersen_proof_t *
eg_constant_chaum_pedersen_proof_new(eg_element_mod_p_t *pad, eg_element_mod_p_t *data,
                                     eg_element_mod_q_t *challenge, eg_element_mod_q_t *response,
                                     uint64_t constant)
{
    auto *pad_ = AS_TYPE(ElementModP, pad);
    auto *data_ = AS_TYPE(ElementModP, data);
    auto *challenge_ = AS_TYPE(ElementModQ, challenge);
    auto *response_ = AS_TYPE(ElementModQ, response);
    return AS_TYPE(eg_constant_chaum_pedersen_proof_t,
                   new ConstantChaumPedersenProof(pad_, data_, challenge_, response_, constant));
}
EG_API void eg_constant_chaum_pedersen_proof_free(eg_constant_chaum_pedersen_proof_t *proof)
{
    if (proof == nullptr) {
        return;
    }
    // TODO: safety
    delete AS_TYPE(ConstantChaumPedersenProof, proof);
}

bool eg_constant_chaum_pedersen_proof_is_valid(eg_constant_chaum_pedersen_proof_t *proof,
                                               eg_elgamal_ciphertext_t *message,
                                               eg_element_mod_p_t *k, eg_element_mod_q_t *q)
{
    auto *message_ = AS_TYPE(ElGamalCiphertext, message);
    auto *k_ = AS_TYPE(ElementModP, k);
    auto *q_ = AS_TYPE(ElementModQ, q);
    return AS_TYPE(ConstantChaumPedersenProof, proof)->isValid(message_, k_, q_);
}

eg_constant_chaum_pedersen_proof_t *
eg_constant_chaum_pedersen_proof_make(eg_elgamal_ciphertext_t *message, eg_element_mod_q_t *r,
                                      eg_element_mod_p_t *k, eg_element_mod_q_t *seed,
                                      eg_element_mod_q_t *hash_header, uint64_t constant)
{
    auto *message_ = AS_TYPE(ElGamalCiphertext, message);
    auto *r_ = AS_TYPE(ElementModQ, r);
    auto *k_ = AS_TYPE(ElementModP, k);
    auto *seed_ = AS_TYPE(ElementModQ, seed);
    auto *hashHeader = AS_TYPE(ElementModQ, hash_header);
    return AS_TYPE(eg_constant_chaum_pedersen_proof_t,
                   ConstantChaumPedersenProof::make(message_, r_, k_, seed_, hashHeader, constant));
}