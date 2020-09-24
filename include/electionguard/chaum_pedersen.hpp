#ifndef __ELECTIONGUARD_CORE_CHAUM_PEDERSEN_HPP_INCLUDED__
#define __ELECTIONGUARD_CORE_CHAUM_PEDERSEN_HPP_INCLUDED__

#include "elgamal.hpp"
#include "export.h"
#include "group.hpp"

#include <memory>

namespace electionguard
{

    class EG_API DisjunctiveChaumPedersenProof
    {
      public:
        DisjunctiveChaumPedersenProof(const DisjunctiveChaumPedersenProof &other);
        DisjunctiveChaumPedersenProof(const DisjunctiveChaumPedersenProof &&other);
        DisjunctiveChaumPedersenProof(
          unique_ptr<ElementModP> proof_zero_pad, unique_ptr<ElementModP> proof_zero_data,
          unique_ptr<ElementModP> proof_one_pad, unique_ptr<ElementModP> proof_one_data,
          unique_ptr<ElementModQ> proof_zero_challenge, unique_ptr<ElementModQ> proof_one_challenge,
          unique_ptr<ElementModQ> challenge, unique_ptr<ElementModQ> proof_zero_response,
          unique_ptr<ElementModQ> proof_one_response);
        ~DisjunctiveChaumPedersenProof();

        DisjunctiveChaumPedersenProof &operator=(DisjunctiveChaumPedersenProof other);
        DisjunctiveChaumPedersenProof &operator=(DisjunctiveChaumPedersenProof &&other);

        bool isValid(const ElGamalCiphertext &message, const ElementModP &k, const ElementModQ &q);

        static unique_ptr<DisjunctiveChaumPedersenProof>
        make(const ElGamalCiphertext &message, const ElementModQ &r, const ElementModP &k,
             const ElementModQ &q, const ElementModQ &seed, uint64_t plaintext);

      protected:
        static unique_ptr<DisjunctiveChaumPedersenProof>
        make_zero(const ElGamalCiphertext &message, const ElementModQ &r, const ElementModP &k,
                  const ElementModQ &q, const ElementModQ &seed);
        static unique_ptr<DisjunctiveChaumPedersenProof>
        make_one(const ElGamalCiphertext &message, const ElementModQ &r, const ElementModP &k,
                 const ElementModQ &q, const ElementModQ &seed);

      private:
        class Impl;
        unique_ptr<Impl> pimpl;
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
