#include "electionguard/chaum_pedersen.hpp"

#include "electionguard/group.hpp"
#include "variant_cast.hpp"

extern "C" {
#include "electionguard/chaum_pedersen.h"
}

using electionguard::ConstantChaumPedersenProof;
using electionguard::DisjunctiveChaumPedersenProof;
using electionguard::ElementModP;
using electionguard::ElementModQ;
using electionguard::ElGamalCiphertext;

#pragma region DisjunctiveChaumPedersenProof

bool eg_disjunctive_chaum_pedersen_proof_is_valid(eg_disjunctive_chaum_pedersen_proof_t *proof,
                                                  eg_elgamal_ciphertext_t *message,
                                                  eg_element_mod_p_t *k, eg_element_mod_q_t *q)
{
    auto *message_ = AS_TYPE(ElGamalCiphertext, message);
    auto *k_ = AS_TYPE(ElementModP, k);
    auto *q_ = AS_TYPE(ElementModQ, q);
    return AS_TYPE(DisjunctiveChaumPedersenProof, proof)->isValid(*message_, *k_, *q_);
}

#pragma endregion

#pragma region ConstantChaumPedersenProof

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

#pragma endregion