#include "electionguard/chaum_pedersen.hpp"

#include <cstdlib>
#include <cstring>

using std::string;
namespace electionguard
{
#pragma region DisjunctiveChaumPedersenProof

    // Lifecycle Methods

    DisjunctiveChaumPedersenProof::DisjunctiveChaumPedersenProof(
      ElementModP *proof_zero_pad, ElementModP *proof_zero_data, ElementModP *proof_one_pad,
      ElementModP *proof_one_data, ElementModQ *proof_zero_challenge,
      ElementModQ *proof_one_challenge, ElementModQ *challenge, ElementModQ *proof_zero_response,
      ElementModQ *proof_one_response)
    {
        data.proof_zero_pad = proof_zero_pad;
        data.proof_zero_data = proof_zero_data;
        data.proof_one_pad = proof_one_pad;
        data.proof_one_data = proof_one_data;
        data.proof_zero_challenge = proof_zero_challenge;
        data.proof_one_challenge = proof_one_challenge;
        data.challenge = challenge;
        data.proof_zero_response = proof_zero_response;
        data.proof_one_response = proof_one_response;
    }

    DisjunctiveChaumPedersenProof::~DisjunctiveChaumPedersenProof() {}

    // Property Getters

    // Public Members

    bool DisjunctiveChaumPedersenProof::isValid(ElGamalCiphertext *message, ElementModP *k,
                                                ElementModQ *q)
    {
        // TODO: implement
        return true;
    }

    DisjunctiveChaumPedersenProof *
    DisjunctiveChaumPedersenProof::make(ElGamalCiphertext *message, ElementModQ *r, ElementModP *k,
                                        ElementModQ *q, ElementModQ *seed, uint64_t plaintext)
    {
        // TODO: implement
        return nullptr;
    }

    // Private Members
    DisjunctiveChaumPedersenProof *
    DisjunctiveChaumPedersenProof::make_zero(ElGamalCiphertext *message, ElementModQ *r,
                                             ElementModP *k, ElementModQ *q, ElementModQ *seed)
    {
        // TODO: implement
        return nullptr;
    }

    DisjunctiveChaumPedersenProof *
    DisjunctiveChaumPedersenProof::make_one(ElGamalCiphertext *message, ElementModQ *r,
                                            ElementModP *k, ElementModQ *q, ElementModQ *seed)
    {
        // TODO: implement
        return nullptr;
    }

#pragma endregion

#pragma region ConstantChaumPedersenProof

    // Lifecycle Methods

    ConstantChaumPedersenProof::ConstantChaumPedersenProof(ElementModP *pad, ElementModP *data,
                                                           ElementModQ *challenge,
                                                           ElementModQ *response, uint64_t constant)
    {
        this->data.pad = pad;
        this->data.data = data;
        this->data.challenge = challenge;
        this->data.response = response;
        this->data.constant = constant;
    }

    ConstantChaumPedersenProof::~ConstantChaumPedersenProof() {}

    // Property Getters

    // Public Members

    bool ConstantChaumPedersenProof::isValid(ElGamalCiphertext *message, ElementModP *k,
                                             ElementModQ *q)
    {
        // TODO: implement
        return true;
    }

    ConstantChaumPedersenProof *
    ConstantChaumPedersenProof::make(ElGamalCiphertext *message, ElementModQ *r, ElementModP *k,
                                     ElementModQ *seed, ElementModQ *hash_header, uint64_t constant)
    {
        // TODO: implement
        return nullptr;
    }

#pragma endregion
} // namespace electionguard