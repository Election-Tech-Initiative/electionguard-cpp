#ifndef __ELECTIONGUARD_CORE_CHAUM_PEDERSEN_HPP_INCLUDED__
#define __ELECTIONGUARD_CORE_CHAUM_PEDERSEN_HPP_INCLUDED__

#include "elgamal.hpp"
#include "export.h"
#include "group.hpp"

namespace electionguard
{
    struct DisjunctiveChaumPedersenProofData {
        ElementModP *proof_zero_pad;
        ElementModP *proof_zero_data;
        ElementModP *proof_one_pad;
        ElementModP *proof_one_data;
        ElementModQ *proof_zero_challenge;
        ElementModQ *proof_one_challenge;
        ElementModQ *challenge;
        ElementModQ *proof_zero_response;
        ElementModQ *proof_one_response;
    };

    class EG_API DisjunctiveChaumPedersenProof
    {
      public:
        DisjunctiveChaumPedersenProof(ElementModP *proof_zero_pad, ElementModP *proof_zero_data,
                                      ElementModP *proof_one_pad, ElementModP *proof_one_data,
                                      ElementModQ *proof_zero_challenge,
                                      ElementModQ *proof_one_challenge, ElementModQ *challenge,
                                      ElementModQ *proof_zero_response,
                                      ElementModQ *proof_one_response);
        ~DisjunctiveChaumPedersenProof();

        bool isValid(ElGamalCiphertext *message, ElementModP *k, ElementModQ *q);

        static DisjunctiveChaumPedersenProof *make(ElGamalCiphertext *message, ElementModQ *r,
                                                   ElementModP *k, ElementModQ *q,
                                                   ElementModQ *seed, uint64_t plaintext);

      private:
        DisjunctiveChaumPedersenProofData data;
        static DisjunctiveChaumPedersenProof *make_zero(ElGamalCiphertext *message, ElementModQ *r,
                                                        ElementModP *k, ElementModQ *q,
                                                        ElementModQ *seed);
        static DisjunctiveChaumPedersenProof *make_one(ElGamalCiphertext *message, ElementModQ *r,
                                                       ElementModP *k, ElementModQ *q,
                                                       ElementModQ *seed);
    };

    struct ConstantChaumPedersenProofData {
        ElementModP *pad;
        ElementModP *data;
        ElementModQ *challenge;
        ElementModQ *response;
        uint64_t constant;
    };

    class EG_API ConstantChaumPedersenProof
    {
      public:
        ConstantChaumPedersenProof(ElementModP *pad, ElementModP *data, ElementModQ *challenge,
                                   ElementModQ *response, uint64_t constant);
        ~ConstantChaumPedersenProof();

        bool isValid(ElGamalCiphertext *message, ElementModP *k, ElementModQ *q);

        static ConstantChaumPedersenProof *make(ElGamalCiphertext *message, ElementModQ *r,
                                                ElementModP *k, ElementModQ *seed,
                                                ElementModQ *hash_header, uint64_t constant);

      private:
        ConstantChaumPedersenProofData data;
    };
} // namespace electionguard

#endif /* __ELECTIONGUARD_CORE_CHAUM_PEDERSEN_HPP_INCLUDED__ */
