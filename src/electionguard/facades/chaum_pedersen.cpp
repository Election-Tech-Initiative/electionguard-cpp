#include "electionguard/chaum_pedersen.hpp"

#include "../log.hpp"
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
using electionguard::Log;

#pragma region DisjunctiveChaumPedersenProof

eg_electionguard_status_t
eg_disjunctive_chaum_pedersen_proof_free(eg_disjunctive_chaum_pedersen_proof_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(DisjunctiveChaumPedersenProof,
                   handle); // NOLINT(cppcoreguidelines-owning-memory)
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_disjunctive_chaum_pedersen_proof_get_zero_pad(eg_disjunctive_chaum_pedersen_proof_t *handle,
                                                 eg_element_mod_p_t **out_element_ref)
{
    auto *element = AS_TYPE(DisjunctiveChaumPedersenProof, handle)->getProofZeroPad();
    *out_element_ref = AS_TYPE(eg_element_mod_p_t, element);
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_disjunctive_chaum_pedersen_proof_get_zero_data(eg_disjunctive_chaum_pedersen_proof_t *handle,
                                                  eg_element_mod_p_t **out_element_ref)
{
    auto *element = AS_TYPE(DisjunctiveChaumPedersenProof, handle)->getProofZeroData();
    *out_element_ref = AS_TYPE(eg_element_mod_p_t, element);
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_disjunctive_chaum_pedersen_proof_get_one_pad(eg_disjunctive_chaum_pedersen_proof_t *handle,
                                                eg_element_mod_p_t **out_element_ref)
{
    auto *element = AS_TYPE(DisjunctiveChaumPedersenProof, handle)->getProofOnePad();
    *out_element_ref = AS_TYPE(eg_element_mod_p_t, element);
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_disjunctive_chaum_pedersen_proof_get_one_data(eg_disjunctive_chaum_pedersen_proof_t *handle,
                                                 eg_element_mod_p_t **out_element_ref)
{
    auto *element = AS_TYPE(DisjunctiveChaumPedersenProof, handle)->getProofOneData();
    *out_element_ref = AS_TYPE(eg_element_mod_p_t, element);
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_disjunctive_chaum_pedersen_proof_get_zero_challenge(
  eg_disjunctive_chaum_pedersen_proof_t *handle, eg_element_mod_q_t **out_element_ref)
{
    auto *element = AS_TYPE(DisjunctiveChaumPedersenProof, handle)->getProofZeroChallenge();
    *out_element_ref = AS_TYPE(eg_element_mod_q_t, element);
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_disjunctive_chaum_pedersen_proof_get_one_challenge(eg_disjunctive_chaum_pedersen_proof_t *handle,
                                                      eg_element_mod_q_t **out_element_ref)
{
    auto *element = AS_TYPE(DisjunctiveChaumPedersenProof, handle)->getProofOneChallenge();
    *out_element_ref = AS_TYPE(eg_element_mod_q_t, element);
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_disjunctive_chaum_pedersen_proof_get_challenge(eg_disjunctive_chaum_pedersen_proof_t *handle,
                                                  eg_element_mod_q_t **out_element_ref)
{
    auto *element = AS_TYPE(DisjunctiveChaumPedersenProof, handle)->getChallenge();
    *out_element_ref = AS_TYPE(eg_element_mod_q_t, element);
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_disjunctive_chaum_pedersen_proof_get_zero_response(eg_disjunctive_chaum_pedersen_proof_t *handle,
                                                      eg_element_mod_q_t **out_element_ref)
{
    auto *element = AS_TYPE(DisjunctiveChaumPedersenProof, handle)->getProofZeroResponse();
    *out_element_ref = AS_TYPE(eg_element_mod_q_t, element);
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_disjunctive_chaum_pedersen_proof_get_one_response(eg_disjunctive_chaum_pedersen_proof_t *handle,
                                                     eg_element_mod_q_t **out_element_ref)
{
    auto *element = AS_TYPE(DisjunctiveChaumPedersenProof, handle)->getProofOneResponse();
    *out_element_ref = AS_TYPE(eg_element_mod_q_t, element);
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_disjunctive_chaum_pedersen_proof_make(
  eg_elgamal_ciphertext_t *in_message, eg_element_mod_q_t *in_r, eg_element_mod_p_t *in_k,
  eg_element_mod_q_t *in_q, eg_element_mod_q_t *in_seed, uint64_t in_plaintext,
  eg_disjunctive_chaum_pedersen_proof_t **out_handle)
{
    if (in_message == nullptr || in_r == nullptr || in_k == nullptr || in_q == nullptr ||
        in_seed == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    auto *message = AS_TYPE(ElGamalCiphertext, in_message);
    auto *r = AS_TYPE(ElementModQ, in_r);
    auto *k = AS_TYPE(ElementModP, in_k);
    auto *q = AS_TYPE(ElementModQ, in_q);
    auto *seed = AS_TYPE(ElementModQ, in_seed);

    try {
        auto proof = DisjunctiveChaumPedersenProof::make(*message, *r, *k, *q, *seed, in_plaintext);
        *out_handle = AS_TYPE(eg_disjunctive_chaum_pedersen_proof_t, proof.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_disjunctive_chaum_pedersen_proof_make", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

bool eg_disjunctive_chaum_pedersen_proof_is_valid(eg_disjunctive_chaum_pedersen_proof_t *handle,
                                                  eg_elgamal_ciphertext_t *in_ciphertext,
                                                  eg_element_mod_p_t *in_k,
                                                  eg_element_mod_q_t *in_q)
{
    if (handle == nullptr || in_ciphertext == nullptr || in_k == nullptr || in_q == nullptr) {
        return false;
    }
    auto *ciphertext = AS_TYPE(ElGamalCiphertext, in_ciphertext);
    auto *k = AS_TYPE(ElementModP, in_k);
    auto *q = AS_TYPE(ElementModQ, in_q);
    return AS_TYPE(DisjunctiveChaumPedersenProof, handle)->isValid(*ciphertext, *k, *q);
}

#pragma endregion

#pragma region ConstantChaumPedersenProof

eg_electionguard_status_t
eg_constant_chaum_pedersen_proof_free(eg_constant_chaum_pedersen_proof_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(ConstantChaumPedersenProof, handle); // NOLINT(cppcoreguidelines-owning-memory)
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

EG_API eg_electionguard_status_t eg_constant_chaum_pedersen_proof_get_pad(
  eg_constant_chaum_pedersen_proof_t *handle, eg_element_mod_p_t **out_element_ref)
{
    auto *element = AS_TYPE(ConstantChaumPedersenProof, handle)->getPad();
    *out_element_ref = AS_TYPE(eg_element_mod_p_t, element);
    return ELECTIONGUARD_STATUS_SUCCESS;
}

EG_API eg_electionguard_status_t eg_constant_chaum_pedersen_proof_get_data(
  eg_constant_chaum_pedersen_proof_t *handle, eg_element_mod_p_t **out_element_ref)
{
    auto *element = AS_TYPE(ConstantChaumPedersenProof, handle)->getData();
    *out_element_ref = AS_TYPE(eg_element_mod_p_t, element);
    return ELECTIONGUARD_STATUS_SUCCESS;
}

EG_API eg_electionguard_status_t eg_constant_chaum_pedersen_proof_get_challenge(
  eg_constant_chaum_pedersen_proof_t *handle, eg_element_mod_q_t **out_element_ref)
{
    auto *element = AS_TYPE(ConstantChaumPedersenProof, handle)->getChallenge();
    *out_element_ref = AS_TYPE(eg_element_mod_q_t, element);
    return ELECTIONGUARD_STATUS_SUCCESS;
}

EG_API eg_electionguard_status_t eg_constant_chaum_pedersen_proof_get_response(
  eg_constant_chaum_pedersen_proof_t *handle, eg_element_mod_q_t **out_element_ref)
{
    auto *element = AS_TYPE(ConstantChaumPedersenProof, handle)->getResponse();
    *out_element_ref = AS_TYPE(eg_element_mod_q_t, element);
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_constant_chaum_pedersen_proof_make(eg_elgamal_ciphertext_t *in_message, eg_element_mod_q_t *in_r,
                                      eg_element_mod_p_t *in_k, eg_element_mod_q_t *in_seed,
                                      eg_element_mod_q_t *in_hash_header, uint64_t in_constant,
                                      eg_constant_chaum_pedersen_proof_t **out_handle)
{
    if (in_message == nullptr || in_r == nullptr || in_k == nullptr || in_seed == nullptr ||
        in_hash_header == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    auto *ciphertext = AS_TYPE(ElGamalCiphertext, in_message);
    auto *r = AS_TYPE(ElementModQ, in_r);
    auto *k = AS_TYPE(ElementModP, in_k);
    auto *seed = AS_TYPE(ElementModQ, in_seed);
    auto *hashHeader = AS_TYPE(ElementModQ, in_hash_header);

    try {
        auto proof =
          ConstantChaumPedersenProof::make(*ciphertext, *r, *k, *seed, *hashHeader, in_constant);
        *out_handle = AS_TYPE(eg_constant_chaum_pedersen_proof_t, proof.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_constant_chaum_pedersen_proof_make", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

bool eg_constant_chaum_pedersen_proof_is_valid(eg_constant_chaum_pedersen_proof_t *handle,
                                               eg_elgamal_ciphertext_t *in_ciphertext,
                                               eg_element_mod_p_t *in_k, eg_element_mod_q_t *in_q)
{
    if (handle == nullptr || in_ciphertext == nullptr || in_k == nullptr || in_q == nullptr) {
        return false;
    }
    auto *ciphertext = AS_TYPE(ElGamalCiphertext, in_ciphertext);
    auto *k = AS_TYPE(ElementModP, in_k);
    auto *q = AS_TYPE(ElementModQ, in_q);
    return AS_TYPE(ConstantChaumPedersenProof, handle)->isValid(*ciphertext, *k, *q);
}

#pragma endregion