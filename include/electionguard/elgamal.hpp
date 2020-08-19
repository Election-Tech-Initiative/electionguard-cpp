#ifndef __ELECTIONGUARD__CORE_ELGAMAL_HPP_INCLUDED__
#define __ELECTIONGUARD__CORE_ELGAMAL_HPP_INCLUDED__
#include "export.h"
#include "group.hpp"

namespace electionguard
{
    struct ElGamalCiphertextData {
        ElementModP *pad;
        ElementModP *data;
    };

    class ElGamalCiphertext
    {
      public:
        ElGamalCiphertext(ElementModP *pad, ElementModP *data);
        ~ElGamalCiphertext();

        ElementModP *getPad();
        ElementModP *getData();

      private:
        ElGamalCiphertextData data;
    };

    ElGamalCiphertext *elgamalEncrypt(uint64_t m, ElementModQ *nonce, ElementModP *publicKey);
} // namespace electionguard

#endif /* __ELECTIONGUARD__CORE_ELGAMAL_HPP_INCLUDED__ */