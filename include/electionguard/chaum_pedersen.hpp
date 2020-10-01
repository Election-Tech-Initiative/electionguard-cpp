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

        ElementModP *getProofZeroPad() const;
        ElementModP *getProofZeroData() const;
        ElementModP *getProofOnePad() const;
        ElementModP *getProofOneData() const;
        ElementModQ *getProofZeroChallenge() const;
        ElementModQ *getProofOneChallenge() const;
        ElementModQ *getChallenge() const;
        ElementModQ *getProofZeroResponse() const;
        ElementModQ *getProofOneResponse() const;

        static unique_ptr<DisjunctiveChaumPedersenProof>
        make(const ElGamalCiphertext &message, const ElementModQ &r, const ElementModP &k,
             const ElementModQ &q, const ElementModQ &seed, uint64_t plaintext);

        bool isValid(const ElGamalCiphertext &message, const ElementModP &k, const ElementModQ &q);

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

    class EG_API ConstantChaumPedersenProof
    {
      public:
        ConstantChaumPedersenProof(const ConstantChaumPedersenProof &other);
        ConstantChaumPedersenProof(const ConstantChaumPedersenProof &&other);
        ConstantChaumPedersenProof(unique_ptr<ElementModP> pad, unique_ptr<ElementModP> data,
                                   unique_ptr<ElementModQ> challenge,
                                   unique_ptr<ElementModQ> response, uint64_t constant);
        ~ConstantChaumPedersenProof();

        ConstantChaumPedersenProof &operator=(ConstantChaumPedersenProof other);
        ConstantChaumPedersenProof &operator=(ConstantChaumPedersenProof &&other);

        ElementModP *getPad() const;
        ElementModP *getData() const;
        ElementModQ *getChallenge() const;
        ElementModQ *getResponse() const;
        uint64_t getConstant() const;

        static unique_ptr<ConstantChaumPedersenProof>
        make(const ElGamalCiphertext &message, const ElementModQ &r, const ElementModP &k,
             const ElementModQ &seed, const ElementModQ &hash_header, uint64_t constant);

        bool isValid(const ElGamalCiphertext &message, const ElementModP &k, const ElementModQ &q);

      private:
        class Impl;
        unique_ptr<Impl> pimpl;
    };
} // namespace electionguard

#endif /* __ELECTIONGUARD_CORE_CHAUM_PEDERSEN_HPP_INCLUDED__ */
