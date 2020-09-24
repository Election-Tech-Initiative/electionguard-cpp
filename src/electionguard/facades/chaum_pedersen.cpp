#include "electionguard/chaum_pedersen.hpp"

#include "electionguard/group.hpp"
#include "memory_cache.hpp"
#include "variant_cast.hpp"

extern "C" {
#include "electionguard/chaum_pedersen.h"
}

using electionguard::Cache;
using electionguard::ConstantChaumPedersenProof;
using electionguard::DisjunctiveChaumPedersenProof;
using electionguard::ElementModP;
using electionguard::ElementModQ;
using electionguard::ElGamalCiphertext;

static Cache<ConstantChaumPedersenProof> cache_constant_chaum_pedersen_proof;

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

bool eg_constant_chaum_pedersen_proof_is_valid(eg_constant_chaum_pedersen_proof_t *proof,
                                               eg_elgamal_ciphertext_t *message,
                                               eg_element_mod_p_t *k, eg_element_mod_q_t *q)
{
    auto *message_ = AS_TYPE(ElGamalCiphertext, message);
    auto *k_ = AS_TYPE(ElementModP, k);
    auto *q_ = AS_TYPE(ElementModQ, q);
    return AS_TYPE(ConstantChaumPedersenProof, proof)->isValid(*message_, *k_, *q_);
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
    auto proof =
      ConstantChaumPedersenProof::make(*message_, *r_, *k_, *seed_, *hashHeader, constant);
    auto *reference = cache_constant_chaum_pedersen_proof.retain(move(proof));
    return AS_TYPE(eg_constant_chaum_pedersen_proof_t, reference);
}

#pragma endregion